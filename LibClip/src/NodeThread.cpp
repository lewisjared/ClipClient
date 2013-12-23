#include "NodeThread.h"
#include "Peer.h"
#include "Beacon.h"
#include "Logger.h"
#include "MessageFactory.h"
#include "ZyreCPP.h"

#include <sstream>
#include <vector>

#include "boost/uuid/uuid_io.hpp"

NodeThread::NodeThread(zctx_t* context)
	:ZThread(context), m_context(context), m_pipe(NULL)
{
	LOG() << "Creating new node" << std::endl;
	m_uuid = boost::uuids::random_generator() ();
	LOG() << "Node UUID: " << boost::uuids::to_string(m_uuid) << std::endl;

	//Create and bind the inbox
	m_inbox = zsocket_new(m_context, ZMQ_ROUTER);
	m_port = zsocket_bind(m_inbox, "tcp://*:*");
	LOG() << "Inbox bound to port " << m_port << std::endl;

	//Create the beacon on the correct port
	m_beacon = new Beacon(m_context, ZRE_PORT);
	m_beacon->setInterval(500);
	m_beacon->setNoEcho();

	//Generate the packet for the beacon
	ByteStream stream(sizeof(beacon_packet_t));
	stream.putByte('Z');
	stream.putByte('R');
	stream.putByte('E');
	stream.putByte(BEACON_VERSION);
	stream.putUUID(m_uuid);
	stream.putUINT16(htons(m_port));
	m_beacon->publish(stream);
	m_beacon->subscribe("ZRE");
}


NodeThread::~NodeThread(void)
{
	LOG() << "Destroying Node" << std::endl;

	//Terminate the worker thread
	terminate();

	zsocket_destroy(m_context, m_inbox);
	
	delete m_beacon;

	for (auto it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		delete it->second;
	}
}


void NodeThread::eventLoop(void *pipe)
{
	LOG() << "Beginning Node eventLoop" << std::endl;
	m_pipe = pipe;
	zstr_send(m_pipe, "OK");

	int64_t reap_at = zclock_time() + REAP_INTERVAL;
	zpoller_t *poller = zpoller_new(m_pipe, m_beacon->getSocket(), m_inbox, NULL);

	m_terminated = false;

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
	LOG() << "Node event loop terminated" << std::endl;
	zpoller_destroy(&poller);
}

/**
 \fn	void NodeThread::checkPeersHealth()

 \brief	Check peers health and deletes or queries them if necessary
 */
void NodeThread::checkPeersHealth()
{
	std::vector< Peers::iterator> toDelete;

	for (auto it = m_peers.begin(); it != m_peers.end();++it)
	{
		Peer* peer = it->second;

		if (!peer->isConnected())
		{
			//Remove any unconnected peers
			toDelete.push_back(it);
		} else {

			int64_t expiredAt = peer->lastSeen() + EXPIRED_TIME;
			int64_t evaisiveAt = peer->lastSeen() + EVAISIVE_TIME;
			int64_t time = zclock_time();
			if (time >= expiredAt)
			{
				//Remove the Node
				LOG() << "Peer " << it->first << " expired" << std::endl;
				zstr_sendm(m_pipe,"EXIT");
				zstr_send(m_pipe, boost::uuids::to_string(it->first).c_str());

				toDelete.push_back(it);
			} else if (time > evaisiveAt)
			{
				LOG() << "Peer " << it->first << " evasive" << std::endl;
				//Ping the node
				Message* msg = MessageFactory::generatePing();
				peer->sendMesg(msg);
			}
		}
	}

	//Finally delete the peers and iterators in m_peers
	for (auto it = toDelete.begin(); it != toDelete.end(); ++it)
	{
		delete (*it)->second;
		m_peers.erase(*it);
	}
}

void NodeThread::handleAPI()
{
	char* text = zstr_recv(m_pipe);
	std::string command(text);
	zstr_free(&text);

	LOG() << "Node received command: " << command << std::endl;

	if (command == "TERMINATE")
	{
		m_terminated = true;
		zstr_send(m_pipe, "OK");
	}
}

void NodeThread::handlePeers()
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

void NodeThread::handleBeacon()
{
	beacon_t beacon;

	if (m_beacon->getPacket(beacon))
	{
		boost::uuids::uuid peerUUID;
		memcpy(&peerUUID, beacon.packet.uuid, 16);
		auto it = m_peers.find(peerUUID);

		if (it == m_peers.end())
		{
			//Found a new peer
			Peer* peer = new Peer(m_context, m_uuid, peerUUID);

			//Clear out any existing peers with the same endpoint
			std::stringstream ss;
			ss << "tcp://" << beacon.ipAddress << ":" << beacon.packet.port;
			std::string endpoint = ss.str();

			for (auto pr = m_peers.begin(); pr != m_peers.end();)
			{
				if (pr->second->getEndpoint() == ss.str())
				{
					delete it->second;
					m_peers.erase(it++);
				} else 
					++it;
			}

			peer->connect(endpoint);
			m_peers[peerUUID] = peer;
		} else {
			//Heard from an existing peer
			Peer* peer = it->second;
			peer->seen();
			LOG() << "Seen peer " << peerUUID << std::endl;
		}
	}
}