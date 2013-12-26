#include "MessageFactory.h"
#include "ByteStream.h"
#include "Logger.h"

MessageHello* MessageFactory::generateHello()
{
	MessageHello* msg = new MessageHello();

	return msg;
}

MessageShout* MessageFactory::generateShout()
{
	MessageShout* msg = new MessageShout();

	return msg;
}


MessageWhisper* MessageFactory::generateWhisper()
{
	MessageWhisper* msg = new MessageWhisper();

	return msg;
}

MessageJoin* MessageFactory::generateJoin()
{
	MessageJoin* msg = new MessageJoin();
	return msg;
}

MessageLeave* MessageFactory::generateLeave()
{
	MessageLeave* msg = new MessageLeave();
	return msg;
}

MessagePing* MessageFactory::generatePing()
{
	return new MessagePing();
}

MessagePingOK* MessageFactory::generatePingOk()
{
	return new MessagePingOK();
}

MessageHeader* MessageFactory::generateHeader(const std::string& key, const std::string& value)
{
	return new MessageHeader(key, value);
}

Message* MessageFactory::parse( void* socket )
{
	Message* msg;
	ByteStream* frameStream;
	zframe_t *frame = NULL;
	zframe_t *address = NULL;
	zmsg_t *content = NULL;
	uint8_t numHeaders;
	uint8_t numGroups;

	//  Read valid message frame from socket; we loop over any
	//  garbage data we might receive from badly-connected peers
	while (true) {
		//  If we're reading from a ROUTER socket, get address
		if (zsocket_type (socket) == ZMQ_ROUTER) {
			address = zframe_recv (socket);
			if (!address)
				throw EEmptyPacket();         //  Interrupted
			if (!zsocket_rcvmore (socket))
				throw EMalformedPacket();
		}
		//  Read and parse command in frame
		frame = zframe_recv (socket);
		if (!frame)
			throw EEmptyPacket();

		//  Get and check protocol signature
		frameStream = new ByteStream(frame);

		uint16_t signature;
		signature = frameStream->getUINT16();
		if (signature == (0xAAA0 | 1))
			break;                  //  Valid signature

		//  Protocol assertion, drop message
		while (zsocket_rcvmore (socket)) {
			zframe_destroy (&frame);
			frame = zframe_recv (socket);
		}
	}
	//  Get message id and parse per message type
	msg_t type = (msg_t)frameStream->getByte();

	uint16_t sequence = frameStream->getUINT16();

	if (type == MSG_HELLO)
	{
		msg = new MessageHello();
		msg->setSequence(sequence);
		dynamic_cast<MessageHello*>(msg)->setIP(frameStream->getString());
		dynamic_cast<MessageHello*>(msg)->setMailbox(frameStream->getUINT16());

		numGroups = frameStream->getByte();
		while (numGroups--) {
			std::string group = frameStream->getString();
			dynamic_cast<MessageHello*>(msg)->addGroup(group);
		}
		dynamic_cast<MessageHello*>(msg)->setStatus(frameStream->getByte());
		numHeaders = frameStream->getByte();
		while (numHeaders--) {
			std::string header = frameStream->getString();
			dynamic_cast<MessageHello*>(msg)->addHeader(header);
		}
	} else if (type == MSG_WHISPER)
	{
		msg = new MessageWhisper();
		msg->setSequence(sequence);
		//  Get zero or more remaining frames,
		//  leave current frame untouched
		content = dynamic_cast<MessageWhisper*> (msg)->getContent();
		while (zsocket_rcvmore (socket))
			zmsg_add (content, zframe_recv (socket));
	} else if (type == MSG_SHOUT)
	{
		msg = new MessageShout();
		msg->setSequence(sequence);
		dynamic_cast<MessageShout*> (msg)->setGroup(frameStream->getString());
		//  Get zero or more remaining frames,
		//  leave current frame untouched
		content = dynamic_cast<MessageShout*> (msg)->getContent();
		while (zsocket_rcvmore (socket))
			zmsg_add (content, zframe_recv (socket));
	} else if (type == MSG_JOIN)
	{
		msg = new MessageJoin();
		msg->setSequence(sequence);
		dynamic_cast<MessageJoin*> (msg)->setGroup(frameStream->getString());
		dynamic_cast<MessageJoin*> (msg)->setStatus(frameStream->getByte());
	} else if (type == MSG_LEAVE)
	{
		msg = new MessageLeave();
		msg->setSequence(sequence);
		dynamic_cast<MessageLeave*> (msg)->setGroup(frameStream->getString());
		dynamic_cast<MessageLeave*> (msg)->setStatus(frameStream->getByte());
	} else if (type == MSG_PING)
	{
		msg = new MessagePing();
		msg->setSequence(sequence);
	} else if (type == MSG_PING_OK)
	{
		msg = new MessagePingOK();
		msg->setSequence(sequence);
	} else if (type == MSG_HEADER)
	{
		std::string key = frameStream->getString();
		std::string value = frameStream->getString();
		msg = new MessageHeader(key, value);
		msg->setSequence(sequence);
	} else {
		LOG_WARN() << "Invalid message type received "<< type;
	}
	if (address)
		msg->setAddress(address);

	//  Successful return
	delete frameStream;
	return msg;
}
