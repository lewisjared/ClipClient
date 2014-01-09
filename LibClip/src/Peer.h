#pragma once

#include <string>
#include "czmq.h"
#include <boost/uuid/uuid.hpp>

#include "Zyre.h"
#include "KeyValuePair.h"
#include "User.h"

class Message;


class Peer : public CUser
{
public:
	Peer (zctx_t* context, boost::uuids::uuid nodeUUID, boost::uuids::uuid peerUUID);
	~Peer(void);

	int sendMesg(Message* msg);

	bool connect(const std::string& endpoint, Message* hello);

	std::string getEndpoint() const;

	/**
	 \fn	int64_t Peer::lastSeen();
	
	 \brief	Last time the Peer was seen.
	
	 \return	systemtime that the peer was last seen.
	 */

	int64_t lastSeen();
	void seen();

	bool isConnected();
	TStringVector getGroups() const;
	void setGroups(TStringVector val);
	void setHeaders(KeyValuePair val);
private:
	zctx_t* m_context;
	std::string m_endpoint;
	bool m_connected;
	bool m_closed;
	int64_t m_lastSeen;
	boost::uuids::uuid m_nodeUUID;
	void* m_mailbox;
	TStringVector m_groups;
	DECLARE_LOGGER();
};