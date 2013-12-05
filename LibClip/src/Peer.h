#pragma once

#include <string>
#include "czmq.h"
#include <boost/uuid/uuid.hpp>

class Message;

class Peer
{
public:
	Peer (boost::uuids::uuid nodeUUID, boost::uuids::uuid peerUUID);
	~Peer(void);

	int sendMesg(Message* msg);

	bool connect(const std::string& target);

	/**
	 \fn	int64_t Peer::lastSeen();
	
	 \brief	Last time the Peer was seen.
	
	 \return	systemtime that the peer was last seen.
	 */

	int64_t lastSeen();
	void seen();

	bool isClosed();
private:
	std::string m_endpoint;
	bool m_connected;
	bool m_closed;
	int64_t m_lastSeen;
	boost::uuids::uuid m_uuid;
	boost::uuids::uuid m_self;
	void* m_mailbox;
};

