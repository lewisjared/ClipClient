#include "CZMQContext.h"
#include "Peer.h"
#include "Logger.h"
#include "Message.h"
#include "ZyreCPP.h"
#include "boost/uuid/uuid_io.hpp"


Peer::Peer(boost::uuids::uuid nodeUUID, boost::uuids::uuid peerUUID)
	: m_self(nodeUUID), m_uuid(peerUUID)
{
	assert (nodeUUID == peerUUID);

	LOG() << "Creating peer for " << peerUUID << std::endl;
}


Peer::~Peer(void)
{
	LOG() << "Destroying peer for " << m_uuid << std::endl;
	if (m_mailbox)
		zsocket_destroy(CZMQContext::getContext(), m_mailbox);
}

bool Peer::connect(const std::string& target)
{
	//Create mailbox
	if (m_mailbox)
	{
		LOG_WARN() << "Peer " << m_uuid << " already has a mailbox Peer::connect" << std::endl;
		zsocket_destroy( CZMQContext::getContext(), m_mailbox);
	}

	//  Create new outgoing socket (drop any messages in transit)
	m_mailbox = zsocket_new ( CZMQContext::getContext(), ZMQ_DEALER);

	if (m_mailbox)
	{
		//  Set our own identity on the socket so that receiving node
		//  knows who each message came from.
		zmq_setsockopt (m_mailbox, ZMQ_IDENTITY,
			m_self.begin(), m_self.size());

		zsocket_set_sndhwm (m_mailbox, EXPIRED_TIME * 100);

		//  Send messages immediately or return EAGAIN
		zsocket_set_sndtimeo (m_mailbox, 0);

		//  Connect through to peer node
		int rc = zsocket_connect (m_mailbox, target.c_str());
		assert (rc == 0);
		m_endpoint = target;
		m_connected = true;

		return true;
	}
	return false;

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
		//zre_msg_set_sequence (*msg_p, ++(self->sent_sequence));
		if (msg->send(m_mailbox) && errno == EAGAIN) {
			m_closed = true;
			return -1;
		}
	}

	return 0;
}

bool Peer::isClosed()
{
	return m_closed;
}