/**
 \file	src\MessageHeader.cpp

 \brief	Implements the MessageHeader class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

DEFINE_LOGGER(MessageHeader);


MessageHeader::MessageHeader(const std::string& key, const std::string& value)
	:Message(MSG_HEADER), m_key(key), m_value(value)
{

}

/**
 \fn	int MessagePing::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageHeader::send(void* socket)
{
	assert(socket);

	size_t frameSize = HEADER_SIZE;
	frameSize += 2; //Sequence
	frameSize++;
	frameSize += m_key.size();
	frameSize++;
	frameSize += m_value.size();

	ByteStream bs = ByteStream(frameSize);

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	bs.putString(m_key);
	bs.putString(m_value);

	return sendBytes(socket, bs, 0);
}

void MessageHeader::log()
{
	LOG() << m_key << "=" << m_value;
}