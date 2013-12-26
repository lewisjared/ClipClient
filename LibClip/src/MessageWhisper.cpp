/**
 \file	src\MessageWhisper.cpp

 \brief	Implements the MessageWhisper class.
 */
 
#include "ByteStream.h"
#include "Message.h"
#include <cassert>


MessageWhisper::MessageWhisper()
	:Message(MSG_WHISPER)
{
	m_content = zmsg_new();
}

MessageWhisper::~MessageWhisper()
{
	zmsg_destroy(&m_content);
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

	int flags = zmsg_size (m_content)? ZFRAME_MORE: 0;

	int result = sendBytes(socket, bs, flags);

	zmsg_send(&m_content, socket);

	return result;
}

zmsg_t* MessageWhisper::getContent()
{
	return m_content;
}

void MessageWhisper::setContent(zmsg_t* content)
{
	//Destroy the old message
	zmsg_destroy(&m_content);
	//Duplicate the message that is passed
	m_content = zmsg_dup(content);
}