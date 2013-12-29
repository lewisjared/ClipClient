#pragma once

#include "ByteStream.h"

#include "boost/uuid/uuid.hpp"
#include <string>
#include "czmq.h"

namespace zyre
{

enum EventType
{
	EVT_INVALID,
	EVT_WHISPER,
	EVT_SHOUT,
	EVT_ENTER,
	EVT_EXIT,
};

class Event
{
public:
	Event(EventType type);
	~Event();

	EventType getType() const;
	bool isValid() const;

	void setContent(const ByteStream& bs);
	ByteStream getContent() const;

	boost::uuids::uuid getFrom() const;
	void setFrom(boost::uuids::uuid uuid);

	static Event* parse(zmsg_t* msg);

private:
	EventType m_type;
	boost::uuids::uuid m_from;
	ByteStream m_content;
};

}