#include "MessageFactory.h"
#include "ByteStream.h"
#include "Logger.h"

Message* MessageFactory::generateHello()
{
	MessageHello* msg = new MessageHello();

	return (Message*) msg;
}

Message* MessageFactory::generateWhisper()
{
	MessageWhisper* msg = new MessageWhisper();

	return (Message*) msg;
}

Message* MessageFactory::generateJoin()
{
	MessageJoin* msg = new MessageJoin();
	return (Message*) msg;
}

Message* MessageFactory::generateLeave()
{
	MessageLeave* msg = new MessageLeave();
	return (Message*) msg;
}

Message* MessageFactory::generatePing()
{
	return (Message*) new MessagePing();
}

Message* MessageFactory::generatePingOk()
{
	return (Message*) new MessagePingOK();
}

Message* MessageFactory::parse( void* socket )
{
	Message* msg;
	ByteStream frameStream;
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
		frameStream.setFrame(frame);

		uint16_t signature;
		signature = frameStream.getUINT16();
		if (signature == (0xAAA0 | 1))
			break;                  //  Valid signature

		//  Protocol assertion, drop message
		while (zsocket_rcvmore (socket)) {
			zframe_destroy (&frame);
			frame = zframe_recv (socket);
		}
		zframe_destroy (&frame);
	}
	//  Get message id and parse per message type
	msg_t type = (msg_t)frameStream.getByte();

	switch (type) {
	case MSG_HELLO:
		msg = new MessageHello();
		msg->setSequence(frameStream.getUINT16());
		dynamic_cast<MessageHello*>(msg)->setIP(frameStream.getString());
		dynamic_cast<MessageHello*>(msg)->setMailbox(frameStream.getUINT16());

		numGroups = frameStream.getByte();
		while (numGroups--) {
			std::string group = frameStream.getString();
			dynamic_cast<MessageHello*>(msg)->addGroup(group);
		}
		static_cast<MessageHello*>(msg)->setStatus(frameStream.getByte());
		numHeaders = frameStream.getByte();
		while (numHeaders--) {
			std::string header = frameStream.getString();
			dynamic_cast<MessageHello*>(msg)->addHeader(header);
		}
		break;

	case MSG_WHISPER:
		msg = new MessageWhisper();
		msg->setSequence(frameStream.getUINT16());
		//  Get zero or more remaining frames,
		//  leave current frame untouched
		content = dynamic_cast<MessageWhisper*> (msg)->getContent();
		while (zsocket_rcvmore (socket))
			zmsg_add (content, zframe_recv (socket));
		break;

	case MSG_SHOUT:
		msg = new MessageShout();
		msg->setSequence(frameStream.getUINT16());
		dynamic_cast<MessageShout*> (msg)->setGroup(frameStream.getString());
		//  Get zero or more remaining frames,
		//  leave current frame untouched
		content = dynamic_cast<MessageShout*> (msg)->getContent();
		while (zsocket_rcvmore (socket))
			zmsg_add (content, zframe_recv (socket));
		break;

	case MSG_JOIN:
		msg = new MessageJoin();
		msg->setSequence(frameStream.getUINT16());
		dynamic_cast<MessageJoin*> (msg)->setGroup(frameStream.getString());
		dynamic_cast<MessageJoin*> (msg)->setStatus(frameStream.getByte());
		break;

	case MSG_LEAVE:
		msg = new MessageLeave();
		msg->setSequence(frameStream.getUINT16());
		dynamic_cast<MessageLeave*> (msg)->setGroup(frameStream.getString());
		dynamic_cast<MessageLeave*> (msg)->setStatus(frameStream.getByte());
		break;

	case MSG_PING:
		msg = new MessagePing();
		msg->setSequence(frameStream.getUINT16());
		break;

	case MSG_PING_OK:
		msg = new MessagePingOK();
		msg->setSequence(frameStream.getUINT16());
		break;

	default:
		LOG_WARN() << "Invalid message type received "<< type;
	}
	if (address)
		msg->setAddress(address);

	//  Successful return
	zframe_destroy (&frame);
	return msg;
}
