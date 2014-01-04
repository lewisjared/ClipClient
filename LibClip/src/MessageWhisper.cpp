/**
 \file	src\MessageWhisper.cpp

 \brief	Implements the MessageWhisper class.
 */
 
#include "ByteStream.h"
#include "Message.h"
#include <cassert>

DEFINE_LOGGER(MessageWhisper);

MessageWhisper::MessageWhisper()
	:Message(MSG_WHISPER)
{
}

/**
 \fn	int MessageWhisper::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageWhisper::send(void* socket)
{
	assert(socket);

	size_t frameSize = HEADER_SIZE;
	frameSize += 2; //sequence

	ByteStream bs = ByteStream(frameSize);

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	int flags = m_content.size() ? ZFRAME_MORE: 0;

	int result = sendBytes(socket, bs, flags);

	sendBytes(socket, m_content, 0);

	return result;
}

ByteStream MessageWhisper::getContent()
{
	return m_content;
}

void MessageWhisper::setContent(ByteStream content)
{
	m_content = content;
}

void MessageWhisper::setContent(zmsg_t* content)
{
	m_content = ByteStream(zmsg_pop(content));
}

void MessageWhisper::log()
{
	LOG() << m_content;
}