#include "EventFactory.h"
#include "KeyValuePair.h"

using namespace zyre;

Event* EventFactory::generateJoin(boost::uuids::uuid from, std::string group)
{
	return new Event(EVT_INVALID);
}

Event* EventFactory::generateLeave(boost::uuids::uuid from, std::string group)
{
	return new Event(EVT_INVALID);
}

Event* EventFactory::generateEnter(boost::uuids::uuid from, KeyValuePair headers)
{
	Event* event = new Event(EVT_ENTER);
	event->setFrom(from);
	event->setContent(headers.generateBS());

	return event;
}

Event* EventFactory::generateExit(boost::uuids::uuid from)
{
	Event* event = new Event(EVT_EXIT);
	event->setFrom(from);

	return event;
}

Event* EventFactory::generateShout(boost::uuids::uuid from, ByteStream content)
{
	Event* event = new Event(EVT_SHOUT);
	event->setFrom(from);
	event->setContent(content);

	return event;
}

Event* EventFactory::generateWhisper(boost::uuids::uuid from, ByteStream content)
{
	Event* event = new Event(EVT_WHISPER);
	event->setFrom(from);
	event->setContent(content);

	return event;
}