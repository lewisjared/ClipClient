#include "Peer.h"


Peer::Peer(void)
{
}


Peer::~Peer(void)
{
}

int64_t Peer::lastSeen()
{
	return m_lastSeen;
}

void Peer::seen()
{
	m_lastSeen = zclock_time();
}