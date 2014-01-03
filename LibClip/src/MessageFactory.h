#pragma once

#include <stdexcept>
#include <string>

#include "Message.h"

class EMalformedPacket : public std::runtime_error {
public:
	EMalformedPacket() 
		: std::runtime_error("Malformed Packet") { };
};

class EEmptyPacket : public std::runtime_error {
public:
	EEmptyPacket() 
		: std::runtime_error("Empty Packet") { };
};

class MessageFactory
{
public:
	static MessageHello* generateHello();
	static MessageShout* generateShout();
	static MessageWhisper* generateWhisper();
	static MessageJoin* generateJoin();
	static MessageLeave* generateLeave();
	static MessagePing* generatePing();
	static MessagePingOK* generatePingOk();
	static MessageHeader* generateHeader(const std::string &key, const std::string & value);

	// Parses a Message from a socket
	static Message* parse(void* socket);
private:
	DECLARE_LOGGER();
};

