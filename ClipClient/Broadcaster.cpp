#include "StdAfx.h"
#include "Broadcaster.h"
#include "log/logger.h"
#include <boost/chrono.hpp>



Broadcaster::Broadcaster(void)
	:m_socket(5000)
{
	m_thread = boost::thread(&Broadcaster::loop, this);
}


Broadcaster::~Broadcaster(void)
{
	m_thread.interrupt();
}

/**
 \fn	std::vector<Client> Broadcaster::getActiveClients()

 \brief	Gets active clients.
 This function can be called from outside of the thread so the internal data is explicitly copied.

 \return	The active clients.
 */

std::vector<Client> Broadcaster::getActiveClients()
{
	boost::lock_guard<boost::mutex> guard(m_mutex);
	//Explict copy of data
	std::vector<Client> result(m_clientList);
	return result;
}

void Broadcaster::loop()
{
	typedef boost::chrono::duration<long long, boost::milli> milliseconds;
	typedef boost::chrono::duration<long, boost::ratio<60> > minutes;
	// Specifies the delay between shouting
	auto shoutDelay = minutes(1);
	//Ensure that the first loop is a shout
	auto start = boost::chrono::steady_clock::now()-shoutDelay;
	while (1)
	{
		if (boost::chrono::steady_clock::now() - start > shoutDelay)
		{
			shout();
			start = boost::chrono::steady_clock::now();
		}

		checkForReplies();
		boost::this_thread::sleep_for(milliseconds(1000));
	}
}


void Broadcaster::shout()
{
	LOG("Looking for other clients");
	m_socket.send("Whos Alive?",12);
}

void Broadcaster::checkForReplies()
{
	char buff[40];
	char sender[40];
	while(m_socket.recv(buff,40,sender) != -1)
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		Client client;
		client.ip = sender;
		time(&client.lastActiveTime);

		m_clientList.push_back(client);
		LOG("Found client: " + client.ip);

		if (strcmp(buff, "Whos Alive?") == 0)
		{
			m_socket.send("Me", 3);
		}
	}
}