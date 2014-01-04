/**
 \file	src\MessageShout.cpp

 \brief	Implements the MessageShout class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

DEFINE_LOGGER(MessageShout);

MessageShout::MessageShout()
	:Message(MSG_SHOUT)
{
}

/**
 \fn	int MessageShout::send(void* socket)
 \brief	Send this message via a socket

 \param	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageShout::send(void* socket)
{
	assert(socket);

	size_t frameSize = HEADER_SIZE;
	frameSize += 2; //sequence
	frameSize++; //String size
	frameSize+= m_group.size(); //group

	ByteStream bs = ByteStream(frameSize);

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	//Group String
	bs.putString(m_group);

	int flags = m_content.size() ? ZFRAME_MORE: 0;

	int result = sendBytes(socket, bs, flags);

	sendBytes(socket, m_content, 0);

	return result;
}

ByteStream MessageShout::getContent()
{
	return m_content;
}

void MessageShout::setContent(zmsg_t* content)
{
	m_content = ByteStream(zmsg_pop(content));
}

void MessageShout::setContent(ByteStream bs)
{
	m_content = bs;
}

std::string MessageShout::getGroup() const
{
	return m_group;
}

void MessageShout::setGroup(const std::string& group)
{
	m_group = group;
}

void MessageShout::log()
{
	LOG() << m_group << " : " << m_content;
}