/**
 \file	src\MessageJoin.cpp

 \brief	Implements the MessageJoin class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

MessageJoin::MessageJoin()
	:Message(MSG_JOIN)
{

}

/**
 \fn	int MessageJoin::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageJoin::send(void* socket)
{
	assert(socket);

	size_t frameSize = HEADER_SIZE;
	frameSize += 2; //sequence
	frameSize++; //size
	frameSize += m_group.size();
	frameSize++; //status

	ByteStream bs = ByteStream(frameSize);

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	//Group String
	bs.putString(m_group);

	//Status
	bs.putByte(m_status);

	assert(bs.size() == frameSize);

	return sendBytes(socket, bs, 0);
}

std::string MessageJoin::getGroup() const
{
	return m_group;
}

void MessageJoin::setGroup(const std::string& group)
{
	m_group = group;
}

uint8_t MessageJoin::getStatus() const
{
	return m_status;
}

void MessageJoin::setStatus(uint8_t status)
{
	m_status = status;
}

