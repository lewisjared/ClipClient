#include "Event.h"
#include "Logger.h"

#include <fstream>

#include <boost/uuid/uuid_io.hpp>


using namespace zyre;

Event::Event(EventType type)
	: m_type(type)
{
}

Event::~Event()
{

}


ByteStream Event::getContent() const
{
	assert(m_type == EVT_SHOUT || m_type == EVT_WHISPER || m_type == EVT_ENTER);
	return m_content;
}

KeyValuePair Event::getHeaders()
{
	assert(m_type == EVT_ENTER);

	//Copy bs 
	ByteStream bs(m_content);
	
	KeyValuePair kvp;
	kvp.parseBS(bs);
	return kvp;
}

EventType Event::getType() const
{
	return m_type;
}

std::string Event::getTypeStr() const
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

boost::uuids::uuid Event::getFrom() const
{
	return m_from;
}

bool Event::isValid() const
{
	return (m_type != EVT_INVALID);
}

void Event::setFrom(boost::uuids::uuid uuid)
{
	m_from = uuid;
}

void Event::setContent(const ByteStream& bs)
{
	m_content = bs;
}

void Event::send(void* socket)
{
	zmsg_t* msg = zmsg_new();
	zmsg_pushstr(msg, getTypeStr().c_str());
	zmsg_addmem(msg, &m_from, 16);

	//Now handle the conditional sending
	if (m_type == EVT_SHOUT || m_type == EVT_WHISPER || m_type == EVT_ENTER)
		zmsg_add(msg, m_content.getFrame());
	
	zmsg_send(&msg, socket);
}

void Event::dump(const std::string &filename)
{
	std::ofstream output;
	output.open(filename, std::ios::out);

	output << getTypeStr() << " from "  << m_from << std::endl;
	output.write((char*)m_content.data(), m_content.size());
}


Event* Event::parse(zmsg_t* msg)
{
	LOG() << "Parsing Event" << std::endl;
	//The first frame will contain the command
	std::string command = zmsg_popstr(msg);

	EventType type = EVT_INVALID;

	if (command == "WHISPER")
	{
		type = EVT_WHISPER;
	} else if (command == "SHOUT")
	{
		type = EVT_SHOUT;
	} else if (command == "ENTER")
	{
		type = EVT_ENTER;
	} else if (command == "EXIT")
	{
		type = EVT_EXIT;
	}

	Event* event = new Event(type);

	if (event->isValid())
	{
		ByteStream uuid (zmsg_pop(msg));
		event->setFrom(uuid.getUUID());

		if (type == EVT_WHISPER || type == EVT_SHOUT || type == EVT_ENTER)
		{
			zframe_t* frame = zmsg_pop(msg);
			if (frame)
			{
				ByteStream content(frame);
				event->setContent(content);
			}
		}
	} else {
		LOG_ERR() << "Invalid event command, " << command << ", dumping message" << std::endl;
		event->dump("invalidEvent.txt");
	}

	zmsg_destroy(&msg);

	return event;
}