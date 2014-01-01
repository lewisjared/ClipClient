#pragma once

#include "Event.h"

class ByteStream;
class KeyValuePair;

class EventFactory
{
public:
	static CEvent* generateJoin(boost::uuids::uuid from, std::string group);
	static CEvent* generateLeave(boost::uuids::uuid from, std::string group);
	static CEvent* generateEnter(boost::uuids::uuid from, KeyValuePair headers);
	static CEvent* generateExit(boost::uuids::uuid from);
	static CEvent* generateShout(boost::uuids::uuid from, ByteStream content);
	static CEvent* generateWhisper(boost::uuids::uuid from, ByteStream content);
};

