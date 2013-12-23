#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <map>
#include "czmq.h"

/**
 \class	Node

 \brief	Defines a single node in zyre scheme
 */
 
class Beacon;
class Peer;

typedef std::map<std::string, std::string> KeyValuePair;
typedef std::map<boost::uuids::uuid, Peer *> Peers;

#define ZRE_PORT 888

class Node
{
public:
	Node(zctx_t* context, void* pipe);
	~Node(void);


	void run();
private:
	void handleAPI();
	void handlePeers();
	void handleBeacon();
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

