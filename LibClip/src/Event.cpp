#pragma  once

#include "Event.h"
#include "Logger.h"

#include <fstream>
#include <boost/uuid/uuid_io.hpp>

CEvent::CEvent()
	:m_type(EVT_INVALID)
{

}

CEvent::CEvent(EventType type)
	: m_type(type)
{
}

CEvent::~CEvent()
{

}


ByteStream CEvent::getContent() const
{
	assert(m_type == EVT_SHOUT || m_type == EVT_WHISPER || m_type == EVT_ENTER);
	return m_content;
}

KeyValuePair CEvent::getHeaders()
{
	assert(m_type == EVT_ENTER);

	//Copy bs 
	ByteStream bs(m_content);
	
	KeyValuePair kvp;
	kvp.parseBS(bs);
	return kvp;
}

EventType CEvent::getType() const
{
	return m_type;
}

std::string CEvent::getTypeStr() const
{
	switch(m_type)
	{
	case EVT_ENTER:		return "ENTER";		break;
	case EVT_EXIT:		return "EXIT";		break;
	case EVT_INVALID:	return "INVALID";	break;
	case EVT_SHOUT:		return "SHOUT";		break;
	case EVT_WHISPER:	return "WHISPER";	break;
	}
	return "INVALID";
}

boost::uuids::uuid CEvent::getFrom() const
{
	return m_from;
}

bool CEvent::isValid() const
{
	return (m_type != EVT_INVALID);
}

void CEvent::setFrom(boost::uuids::uuid uuid)
{
	m_from = uuid;
}

void CEvent::setContent(const ByteStream& bs)
{
	m_content = bs;
}

void CEvent::dump(const std::string &filename)
{
	std::ofstream output;
	output.open(filename, std::ios::out);

	output << getTypeStr() << " from "  << m_from << std::endl;
	output.write((char*)m_content.data(), m_content.size());
}