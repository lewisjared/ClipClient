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
	static Message* generateHello();
	static Message* generateWhisper();
	static Message* generateJoin();
	static Message* generateLeave();
	static Message* generatePing();
	static Message* generatePingOk();

	// Parses a Message from a socket
	static Message* parse(void* socket);
};

