#include "Beacon.h"
#include "ByteStream.h"
#include "Logger.h"


Beacon::Beacon(zctx_t* context, int portNumber)
	:m_context(context), m_portNumber(portNumber)
{
	m_beacon = zbeacon_new(m_context, 9999);
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

void Beacon::publish(const std::string& packet)
{
	zbeacon_publish(m_beacon, (byte*)packet.c_str(), packet.size());
}

void Beacon::setNoEcho()
{
	zbeacon_noecho(m_beacon);
}

void Beacon::silence()
{
	zbeacon_silence(m_beacon);
}

void Beacon::subscribe(const ByteStream& packet)
{
	zbeacon_subscribe(m_beacon,(byte*) packet.data(), packet.size());
}

void Beacon::subscribe(const std::string& packet)
{
	zbeacon_subscribe(m_beacon, (byte*)packet.c_str(), packet.size());
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
		memcpy(&beacon.packet, zframe_data(frame), zframe_size(frame));
		assert(beacon.packet.version == BEACON_VERSION);
	} else 
		valid = false;

	zstr_free (&ipaddress);
	zframe_destroy(&frame);
	return valid;
}