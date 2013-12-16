#include "Node.h"
#include "Peer.h"
#include "Beacon.h"
#include "Logger.h"
#include "MessageFactory.h"
#include "ZyreCPP.h"

#include "boost/uuid/uuid_io.hpp"

Node::Node(void* pipe)
	:m_pipe(pipe)
{
}


Node::~Node(void)
{
	for (auto it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		delete it->second;
	}
}


void Node::run()
{
	zstr_send(m_pipe, "OK");

	uint64_t reap_at = zclock_time() + REAP_INTERVAL;
	zpoller_t *poller = zpoller_new(m_pipe, m_beacon->getSocket(), NULL);

	while(!zpoller_terminated(poller))
	{
		int timeout = (int) (reap_at - zclock_time());

		assert(timeout <= REAP_INTERVAL);
		if (timeout < 0)
			timeout = 0;
		void* socket = zpoller_wait(poller,timeout);

		if (socket == m_pipe)
			handleAPI();
		else if (socket == m_beacon->getSocket())
			handleBeacon();
		else if (0)
			handlePeers();

		if (zclock_time() >= reap_at)
		{
			reap_at = zclock_time() + REAP_INTERVAL;
			checkPeersHealth();
		}
		if (m_terminated)
			break;
	}
	zpoller_destroy(&poller);
}

void Node::checkPeersHealth()
{
	for (auto it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		Peer* peer = it->second;

		if (peer->isClosed())
		{
			delete it->second;
			m_peers.erase(it);
		}

		int64_t expiredAt = peer->lastSeen() + EXPIRED_TIME;
		int64_t evaisiveAt = peer->lastSeen() + EVAISIVE_TIME;
		if (zclock_time() >= expiredAt)
		{
			//Remove the Node
			LOG() << "Node " << it->first << " expired" << std::endl;
			zstr_sendm(m_pipe,"EXIT");
			zstr_send(m_pipe, boost::uuids::to_string(it->first).c_str());

			delete it->second;
			m_peers.erase(it);
		} else if (zclock_time() > evaisiveAt)
		{
			//Ping the node
			Message* msg = MessageFactory::generatePing();
			peer->sendMesg(msg);
		}
	}
}

void handleAPI()
{
	//To Write
	assert( false);
}

void Node::handlePeers()
{
	Message* msg = MessageFactory::parse(m_inbox);

	boost::uuids::uuid uuid = msg->getAddress();

	Peer* peer = NULL;
	//Find peer in list
	auto it = m_peers.find(uuid);
	if (it != m_peers.end())
		peer = it->second;

	assert( msg->getID() != MSG_HELLO && peer);

	//Process the command
	if (msg->getID() == MSG_HELLO)
	{
		MessageHello* hello = reinterpret_cast<MessageHello*> (msg);

	} else if (msg->getID() == MSG_SHOUT) 
	{
		zstr_sendm(m_pipe, "SHOUT");
		zstr_sendm(m_pipe, boost::uuids::to_string(msg->getAddress()).c_str());
	} else if (msg->getID() == MSG_PING)
	{
		//Reply with ping ok
		peer->sendMesg(MessageFactory::generatePingOk());
	}

	peer->seen();

	//Consume message
	delete msg;
}

void Node::handleBeacon()
{
	beacon_t beacon;

	if (m_beacon->getPacket(beacon))
	{
		boost::uuids::uuid uid;
		memcpy(&uid, beacon.packet.uuid, 16);
		auto it = m_peers.find(uid);

		if (it == m_peers.end())
		{
			//Found a new peer
			Peer* peer = new Peer(m_uid, uid);
			m_peers[uid] = peer;
		} else {
			//Heard from an existing peer
			Peer* peer = it->second;
			peer->seen();
		}
	}
}