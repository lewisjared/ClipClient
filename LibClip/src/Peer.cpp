#include "Peer.h"
#include "Message.h"
#include "MessageFactory.h"

#include "boost/uuid/uuid_io.hpp"

DEFINE_LOGGER(Peer);


Peer::Peer(zctx_t* context, boost::uuids::uuid nodeUUID, boost::uuids::uuid peerUUID)
	: m_context(context), m_nodeUUID(nodeUUID)
{
	assert (nodeUUID != peerUUID);
	m_uuid = peerUUID;
	m_mailbox = NULL;

	LOG() << "Creating peer for " << peerUUID;
	m_lastSeen = zclock_time();
}


Peer::~Peer(void)
{
	LOG() << "Destroying peer for " << m_uuid;
	if (m_mailbox)
		zsocket_destroy(m_context, m_mailbox);
}

bool Peer::connect(const std::string& endpoint, Message* hello)
{
	LOG() << "Connecting peer " << m_uuid << " to " << endpoint;
	//Create mailbox
	if (m_mailbox)
	{
		LOG_WARN() << "Peer " << m_uuid << " already has a mailbox Peer::connect";
		zsocket_destroy( m_context, m_mailbox);
	}

	//  Create new outgoing socket (drop any messages in transit)
	m_mailbox = zsocket_new ( m_context, ZMQ_DEALER);

	if (m_mailbox)
	{
		//  Set our own identity on the socket so that receiving node
		//  knows who each message came from.
		zmq_setsockopt (m_mailbox, ZMQ_IDENTITY,
			m_nodeUUID.begin(), m_nodeUUID.size());

		zsocket_set_sndhwm (m_mailbox, EXPIRED_TIME * 100);

		//  Send messages immediately or return EAGAIN
		zsocket_set_sndtimeo (m_mailbox, 0);

		//  Connect through to peer node
		int rc = zsocket_connect (m_mailbox, endpoint.c_str());
		assert (rc == 0);
		m_endpoint = endpoint;
		m_connected = true;

		// Send a hello msg
		sendMesg(hello);

		return true;
	}
	return false;

}

std::string Peer::getEndpoint() const
{
	return m_endpoint;
}

int64_t Peer::lastSeen()
{
	return m_lastSeen;
}

void Peer::seen()
{
	m_lastSeen = zclock_time();
}

int Peer::sendMesg(Message* msg)
{
	if (m_connected) {
		LOG() << "Sending msg to " << m_uuid;
		//zre_msg_set_sequence (*msg_p, ++(self->sent_sequence));
		if (msg->send(m_mailbox) && errno == EAGAIN) {
			LOG_WARN() << "Sending message to peer " << m_uuid << " failed";
			m_closed = true;
			return -1;
		}
	}

	return 0;
}

bool Peer::isConnected()
{
	return m_connected;
}

void Peer::setHeaders(KeyValuePair val) 
{ 
	m_headers = val;

	LOG() << "New Headers for peer " << m_uuid;
	m_headers.log();
}

TStringVector Peer::getGroups() const 
{ 
	return m_groups; 
}

void Peer::setGroups(TStringVector val) 
{ 
	m_groups = val; 
}