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