#pragma once

#include "UdpSocket.h"

#include <vector>
#include <boost/thread.hpp>

struct Client
{
	std::string ip;
	time_t lastActiveTime;
};

class Broadcaster
{
public:
	Broadcaster(void);
	~Broadcaster(void);

	std::vector<Client> getActiveClients();
private:
	void shout();
	void checkForReplies();
	void loop();
	UDP_Socket m_socket;
	boost::thread m_thread;
	std::vector<Client> m_clientList;

	boost::mutex m_mutex;
};

