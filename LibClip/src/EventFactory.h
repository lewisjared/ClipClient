#pragma once

#include "Event.h"

using zyre::Event;
class ByteStream;
class KeyValuePair;

class EventFactory
{
public:
	static Event* generateJoin(boost::uuids::uuid from, std::string group);
	static Event* generateLeave(boost::uuids::uuid from, std::string group);
	static Event* generateEnter(boost::uuids::uuid from, KeyValuePair headers);
	static Event* generateExit(boost::uuids::uuid from);
	static Event* generateShout(boost::uuids::uuid from, ByteStream content);
	static Event* generateWhisper(boost::uuids::uuid from, ByteStream content);
};

