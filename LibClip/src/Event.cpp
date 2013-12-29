#include "Event.h"
#include "Logger.h"


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
	assert(m_type == EVT_SHOUT || m_type == EVT_WHISPER);
	return m_content;
}

EventType Event::getType() const
{
	return m_type;
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
	} else {
		LOG_ERR() << "Invalid event command, " << command << ", dropping message" << std::endl;
	}


	Event* event = new Event(type);

	if (event)
	{
		ByteStream uuid (zmsg_pop(msg));
		event->setFrom(uuid.getUUID());

		if (type == EVT_WHISPER || type == EVT_SHOUT)
		{
			ByteStream content(zmsg_pop(msg));
			event->setContent(content);
		}
	}

	zmsg_destroy(&msg);

	return event;
}