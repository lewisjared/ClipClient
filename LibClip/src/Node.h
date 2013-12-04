#pragma once



#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <map>

/**
 \class	Node

 \brief	Defines a single node in zyre scheme
 */
 
class Beacon;
class Peer;

typedef std::map<std::string, std::string> KeyValuePair;
typedef std::map<boost::uuids::uuid, Peer *> Peers;

#define REAP_INTERVAL 1000
#define EXPIRED_TIME 10000
#define EVAISIVE_TIME 5000

class Node
{
public:
	Node(void* pipe);
	~Node(void);


	void run();
private:
	void handleAPI();
	void handlePeers();
	void handleBeacon();
	void checkPeersHealth();

	Beacon* m_beacon;
	void* m_pipe;
	KeyValuePair m_headers;
	Peers m_peers;
	boost::uuids::uuid m_uid;
	bool m_terminated;
};

