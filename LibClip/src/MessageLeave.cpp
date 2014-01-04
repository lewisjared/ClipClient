/**
 \file	src\MessageLeave.cpp

 \brief	Implements the MessageLeave class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

DEFINE_LOGGER(MessageLeave);

MessageLeave::MessageLeave()
	:Message(MSG_LEAVE)
{

}

/**
 \fn	int MessageLeave::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageLeave::send(void* socket)
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

	return sendBytes(socket, bs, 0);
}

std::string MessageLeave::getGroup() const
{
	return m_group;
}

void MessageLeave::setGroup(const std::string& group)
{
	m_group = group;
}

uint8_t MessageLeave::getStatus() const
{
	return m_status;
}

void MessageLeave::setStatus(uint8_t status)
{
	m_status = status;
}

void MessageLeave::log()
{
	LOG() << m_group;
}
