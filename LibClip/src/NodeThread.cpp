#include "NodeThread.h"
#include "Event.h"
#include "EventFactory.h"
#include "Peer.h"
#include "Beacon.h"
#include "Logger.h"
#include "MessageFactory.h"
#include "ZyreCPP.h"

#include <sstream>
#include <vector>

#include "boost/uuid/uuid_io.hpp"

using zyre::Event;

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

	start();
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
		else if (socket == m_inbox)
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
				MessagePing* msg = MessageFactory::generatePing();
				peer->sendMesg(msg);
				delete msg;
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
	//Get the whole message before parsing
	zmsg_t *request = zmsg_recv (m_pipe);
	char *text = zmsg_popstr (request);
	std::string command(text);
	zstr_free(&text);

	LOG() << "Node received command: " << command << std::endl;
	if (command == "SET")
	{
		char* key = zmsg_popstr(request);
		char* value = zmsg_popstr(request);
		setKeyValue(key, value);
		zstr_free(&key);
		zstr_free(&value);
	} else if (command == "START")
	{
		//start();
		zstr_send(m_pipe, "OK");
	} else if (command == "SHOUT")
	{
		char* group = zmsg_popstr(request);
		MessageShout* msg = MessageFactory::generateShout();
		msg->setContent(request);
		msg->setGroup(std::string(group));
		sendToPeers(msg);
		delete msg;
		zstr_free(&group);
	} else if (command == "WHISPER")
	{
		ByteStream bs(zmsg_pop(request));
		if (bs.size() != 16)
		{
			LOG() << "Invalid packet" << std::endl;
		}

		boost::uuids::uuid uuid = bs.getUUID();

		MessageWhisper* msg = MessageFactory::generateWhisper();
		msg->setContent(request);
		Peer* target = getPeer(uuid);
		assert(target);

		target->sendMesg(msg);
		delete msg;
	} else if (command == "TERMINATE")
	{
		m_terminated = true;
		zstr_send(m_pipe, "OK");
	}

	zmsg_destroy(&request);
}

void NodeThread::handlePeers()
{
	Message* msg = MessageFactory::parse(m_inbox);

	boost::uuids::uuid uuid = msg->getAddress();

	if (msg->getID() == MSG_HELLO)
	{
		LOG() << "Received a hello from " << uuid << std::endl;
		MessageHello* hello = dynamic_cast<MessageHello*> (msg);
		//Update the peer
		Peer* peer = getPeer(uuid);
		if (peer == NULL)
			peer = createPeer(uuid, hello->getIP(), hello->getMailbox());

		assert(peer);

		peer->setHeaders(hello->getHeaders());
		peer->setGroups(hello->getGroups());

		delete hello;

		return;
	}
	
	Peer* peer = NULL;
	auto it = m_peers.find(uuid);
	if (it != m_peers.end())
		peer = it->second;

	assert (peer);

	if (msg->getID() == MSG_SHOUT) 
	{
		MessageShout* shout = dynamic_cast<MessageShout*> (msg);
		//if (inGroup(shout->getGroup()))
		{
			Event* event = EventFactory::generateShout(msg->getAddress(), shout->getContent());
			event->send(m_pipe);
			delete event;
		}
		delete shout;
	} else if (msg->getID() == MSG_WHISPER)
	{
		MessageWhisper* whisper = dynamic_cast<MessageWhisper*> (msg);
		Event* event = EventFactory::generateWhisper(msg->getAddress(), whisper->getContent());
		event->send(m_pipe);
		delete event;
		delete whisper;
	} else if (msg->getID() == MSG_PING)
	{
		MessagePing* ping = dynamic_cast<MessagePing*> (msg);
		//Reply with ping ok
		peer->sendMesg(MessageFactory::generatePingOk());
		delete ping;
	} else {
		LOG() << "Didn't handle message" << std::endl;
		delete msg;
	}

	if (peer)
		peer->seen();
}

void NodeThread::handleBeacon()
{
	beacon_t beacon;

	if (m_beacon->getPacket(beacon))
	{
		boost::uuids::uuid peerUUID;
		memcpy(&peerUUID, beacon.packet.uuid, 16);
		
		//Heard from an existing peer
		Peer* peer = getPeer(peerUUID);
		if (peer == NULL)
			peer = createPeer(peerUUID, beacon.ipAddress, beacon.packet.port);

		assert(peer);

		peer->seen();
	}
}

Peer* NodeThread::getPeer( boost::uuids::uuid peerUUID)
{
	auto it = m_peers.find(peerUUID);

	if (it == m_peers.end())
		return NULL;
	else 
		return it->second;
}

Peer* NodeThread::createPeer(boost::uuids::uuid peerUUID, std::string ip, uint16_t port)
{
	//Create a new peer
	Peer* peer = new Peer(m_context, m_uuid, peerUUID);

	//Clear out any existing peers with the same endpoint
	std::stringstream ss;
	ss << "tcp://" << ip << ":" << port;
	std::string endpoint = ss.str();

	for (auto pr = m_peers.begin(); pr != m_peers.end();)
	{
		if (pr->second->getEndpoint() == ss.str())
		{
			delete pr->second;
			m_peers.erase(pr++);
		} else 
			++pr;
	}

	//Send a message about this node
	MessageHello* msg = MessageFactory::generateHello();
	msg->setMailbox(m_port);
	msg->setHeaders(m_headers);
	msg->setStatus(1);

	peer->connect(endpoint, (Message*)msg);
	m_peers[peerUUID] = peer;

	delete msg;

	return peer;
}

/**
 \fn	void NodeThread::sendToPeers(Message* msg)

 \brief	Sends a message to all connected peers

 \param	msg	The Message
 */
void NodeThread::sendToPeers(Message* msg)
{
	for (auto it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		Peer* peer = it->second;
		peer->sendMesg(msg);
	}
}

void NodeThread::setKeyValue(std::string key, std::string value)
{
	//Update m_headers
	m_headers[key] = value;

	MessageHeader* msg = MessageFactory::generateHeader(key, value);

	//Inform other peers of the change
	for (auto it = m_peers.begin(); it != m_peers.end(); ++it)
	{
		Peer* peer = it->second;
		peer->sendMesg(msg);
	}
	delete msg;
}

void NodeThread::start()
{
	//Generate the packet for the beacon
	ByteStream stream(sizeof(beacon_packet_t));
	stream.putByte('Z');
	stream.putByte('R');
	stream.putByte('E');
	stream.putByte(BEACON_VERSION);
	stream.putUUID(m_uuid);
	stream.putUINT16(htons(m_port));

	//Start pub and subbing
	m_beacon->publish(stream);
	m_beacon->subscribe("ZRE");
}