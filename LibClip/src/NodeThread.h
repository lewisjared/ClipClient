#pragma once

#include "ZThreaded.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <map>

#include "czmq.h"

class Beacon;
class Peer;

typedef std::map<std::string, std::string> KeyValuePair;
typedef std::map<boost::uuids::uuid, Peer *> Peers;

#define ZRE_PORT 888

/**
 \class	NodeThread

 \brief	The worker thread for a Node.

		This class is spawned as a thread and eventloop is executed. The class then communicates 
		over its pipe.
 */
class NodeThread : public ZThread
{
public:
	NodeThread(zctx_t* context);
	~NodeThread(void);

private:
	void eventLoop(void *pipe);
	void handleAPI();
	void handlePeers();
	void handleBeacon();

	Peer* getPeer( boost::uuids::uuid peerUUID, std::string ip, uint16_t port);

	void checkPeersHealth();

	zctx_t* m_context;
	Beacon* m_beacon;
	void* m_pipe;
	void* m_inbox;
	int m_port;
	KeyValuePair m_headers;
	Peers m_peers;
	boost::uuids::uuid m_uuid;
	bool m_terminated;
};

