#pragma once

#include "ByteStream.h"
#include "KeyValuePair.h"

#include "boost/uuid/uuid.hpp"
#include <string>
#include "czmq.h"

enum EventType
{
	EVT_INVALID,
	EVT_WHISPER,
	EVT_SHOUT,
	EVT_ENTER,
	EVT_EXIT,
};

class CEvent
{
public:
	CEvent();
	CEvent(EventType type);
	~CEvent();

	EventType getType() const;
	std::string getTypeStr() const;
	bool isValid() const;

	void setContent(const ByteStream& bs);
	ByteStream getContent() const;
	KeyValuePair getHeaders();

	boost::uuids::uuid getFrom() const;
	void setFrom(boost::uuids::uuid uuid);

	void dump(const std::string &filename);
private:
	EventType m_type;
	boost::uuids::uuid m_from;
	ByteStream m_content;
};