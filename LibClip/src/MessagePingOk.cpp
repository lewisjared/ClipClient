/**
 \file	src\MessagePingOK.cpp

 \brief	Implements the MessagePingOK class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

MessagePingOK::MessagePingOK()
	:Message(MSG_PING_OK)
{

}

/**
 \fn	int MessagePing::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessagePingOK::send(void* socket)
{
	assert(socket);

	size_t frameSize = HEADER_SIZE + 2;

	ByteStream bs = ByteStream(frameSize);

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	return sendBytes(socket, bs, 0);
}

