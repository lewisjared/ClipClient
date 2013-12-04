#pragma once

#include "czmq.h"

class ZRE_Msg;

class Peer
{
public:
	Peer(void);
	~Peer(void);

	void sendMesg(ZRE_Msg* msg);

	/**
	 \fn	int64_t Peer::lastSeen();
	
	 \brief	Last time the Peer was seen.
	
	 \return	systemtime that the peer was last seen.
	 */

	int64_t lastSeen();
	void seen();
private:
	int64_t m_lastSeen;
};

