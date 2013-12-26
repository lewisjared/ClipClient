/**
 \file	src\MessageShout.cpp

 \brief	Implements the MessageShout class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

MessageShout::MessageShout()
	:Message(MSG_SHOUT)
{
	m_content = zmsg_new();
}

MessageShout::~MessageShout()
{
	zmsg_destroy(&m_content);
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

	int flags = zmsg_size (m_content)? ZFRAME_MORE: 0;

	int result = sendBytes(socket, bs, flags);

	zmsg_send(&m_content, socket);

	return result;
}

zmsg_t* MessageShout::getContent()
{
	return m_content;
}

/**
 \fn	void MessageShout::setContent(zmsg_t* content)

 \brief	Appends a frame to the message
		Takes ownership of the passed message

 \param 	content	If non-null, the conten of this message.
 */
void MessageShout::addContent(zframe_t* content)
{
	zmsg_append(m_content, &content);
}

void MessageShout::setContent(zmsg_t* content)
{
	zmsg_destroy(&m_content);
	m_content = zmsg_dup(content);
}

std::string MessageShout::getGroup() const
{
	return m_group;
}

void MessageShout::setGroup(const std::string& group)
{
	m_group = group;
}