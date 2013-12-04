#pragma once

#include "czmq.h"

#include <string>

class ByteStream;

//Encapsulates a zBeacon
class Beacon
{
public:
	Beacon(int portNumber);
	~Beacon(void);

	const std::string& getHostname();
	void setNoEcho();
	void setInterval(int millis);

	void publish(ByteStream packet);
	void silence();

	void subscribe(ByteStream packet);
	void unsubscribe();

	void* getSocket();
private:
	zbeacon_t *m_beacon;
	int m_portNumber;
};

