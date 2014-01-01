#include "EventFactory.h"
#include "KeyValuePair.h"

CEvent* EventFactory::generateJoin(boost::uuids::uuid from, std::string group)
{
	return new CEvent(EVT_INVALID);
}

CEvent* EventFactory::generateLeave(boost::uuids::uuid from, std::string group)
{
	return new CEvent(EVT_INVALID);
}

CEvent* EventFactory::generateEnter(boost::uuids::uuid from, KeyValuePair headers)
{
	CEvent* event = new CEvent(EVT_ENTER);
	event->setFrom(from);
	event->setContent(headers.generateBS());

	return event;
}

CEvent* EventFactory::generateExit(boost::uuids::uuid from)
{
	CEvent* event = new CEvent(EVT_EXIT);
	event->setFrom(from);

	return event;
}

CEvent* EventFactory::generateShout(boost::uuids::uuid from, ByteStream content)
{
	CEvent* event = new CEvent(EVT_SHOUT);
	event->setFrom(from);
	event->setContent(content);

	return event;
}

CEvent* EventFactory::generateWhisper(boost::uuids::uuid from, ByteStream content)
{
	CEvent* event = new CEvent(EVT_WHISPER);
	event->setFrom(from);
	event->setContent(content);

	return event;
}