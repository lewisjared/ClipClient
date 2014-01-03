#pragma once

#include "czmq.h"
#include "Logger.h"

#include <string>

class ByteStream;

#define  BEACON_VERSION 0x01

typedef struct {
	byte protocol [3];
	byte version;
	byte uuid [16];
	uint16_t port;
} beacon_packet_t;

typedef struct
{
	std::string ipAddress;
	beacon_packet_t packet;
}beacon_t;



//Encapsulates a zBeacon
class Beacon
{
public:
	Beacon(zctx_t* context, int portNumber);
	~Beacon(void);

	const std::string& getHostname();
	void setNoEcho();
	void setInterval(int millis);

	void publish(const ByteStream& packet);
	void publish(const std::string& packet);
	void silence();

	void subscribe(const ByteStream& packet);
	void subscribe(const std::string& packet);
	void unsubscribe();

	void* getSocket();
	bool getPacket(beacon_t& beacon);
private:
	zctx_t* m_context;
	zbeacon_t *m_beacon;
	int m_portNumber;
	std::string m_hostname;
	DECLARE_LOGGER();
};

