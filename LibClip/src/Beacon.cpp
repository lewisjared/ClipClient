#include "Beacon.h"
#include "ByteStream.h"
#include "CZMQContext.h"
#include "Logger.h"


Beacon::Beacon(int portNumber)
	:m_portNumber(portNumber)
{
	m_beacon = zbeacon_new(CZMQContext::getContext(), portNumber);
	LOG() << "Creating new beacon on port " << portNumber << std::endl;
	m_hostname = zbeacon_hostname(m_beacon);
}


Beacon::~Beacon(void)
{
	zbeacon_destroy(&m_beacon);
}

const std::string& Beacon::getHostname()
{
	return m_hostname;
}

void Beacon::setInterval(int millis)
{
	zbeacon_set_interval(m_beacon,millis);
}

void Beacon::publish(const ByteStream& packet)
{
	zbeacon_publish(m_beacon, (byte*)packet.data(), packet.size());
}

void Beacon::silence()
{
	zbeacon_silence(m_beacon);
}

void Beacon::subscribe(const ByteStream& packet)
{
	zbeacon_subscribe(m_beacon,(byte*) packet.data(), packet.size());
}

void* Beacon::getSocket()
{
	return zbeacon_socket(m_beacon);
}

bool Beacon::getPacket(beacon_t& beacon)
{
	char* ipaddress = zstr_recv(getSocket());
	zframe_t* frame = zframe_recv(getSocket());
	beacon.ipAddress = std::string(ipaddress);

	bool valid = true;

	if (zframe_size(frame) == sizeof(beacon_packet_t))
	{
		memcpy(&beacon, zframe_data(frame), zframe_size(frame));
		assert(beacon.packet.version == BEACON_VERSION);
	} else 
		valid = false;

	free (ipaddress);
	zframe_destroy(&frame);
	return valid;
}