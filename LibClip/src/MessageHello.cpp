/**
 \file	src\MessageHello.cpp

 \brief	Implements the MessageHello class.
 */

#include "ByteStream.h"
#include "Message.h"
#include <cassert>

DEFINE_LOGGER(MessageHello);

MessageHello::MessageHello()
	:Message(MSG_HELLO)
{
}

/**
 \fn	int MessageHello::send(void* socket)
 \brief	Send this message via a socket

 \param [in,out]	socket	If non-null, the socket.

 \return	0 if successful.
 */

int MessageHello::send(void* socket)
{
	assert(socket);

	ByteStream bs = ByteStream();

	// Write the header
	bs.putUINT16(0xAAA0 | 1);
	bs.putByte(m_id);

	bs.putUINT16(m_sequence);

	//IP string
	bs.putString(m_ip);
	//Mailbox port
	bs.putUINT16(m_mailboxPort);
	//Groups that the peer is connected to
	bs.putByte(m_groups.size());
	for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
		bs.putString(*it);
	//Status
	bs.putByte(m_status);

	bs.append(m_headers.generateBS());

	return sendBytes(socket, bs, 0);
}

KeyValuePair MessageHello::getHeaders() const
{
	return m_headers;
}

void MessageHello::setHeaders(KeyValuePair headers)
{
	m_headers = headers;
}

void MessageHello::setGroups(TStringVector groups)
{
	m_groups = groups;
}

TStringVector MessageHello::getGroups() const
{
	return m_groups;
}

void MessageHello::addHeader(const std::string& key, const std::string& value)
{
	//Overwrites the key with new value 
	m_headers.addValue(key,value);
}

void MessageHello::addHeader(const std::string& keyValue)
{
	//Parse the header string
	m_headers.addValue(keyValue);
}

void MessageHello::addGroup(const std::string& group)
{
	m_groups.push_back(group);
}

std::string MessageHello::getIP() const
{
	return m_ip;
}

void MessageHello::setIP(std::string ip)
{
	m_ip = ip;
}

uint16_t MessageHello::getMailbox() const
{
	return m_mailboxPort;
}

void MessageHello::setMailbox(uint16_t mailboxPort)
{
	m_mailboxPort = mailboxPort;
}

void MessageHello::setStatus(uint8_t status)
{
	m_status = status;
}

void MessageHello::log()
{
	LOG() << "MessageHello dump";
	LOG() << m_ip << ":" << m_mailboxPort;
	m_headers.log();
	
	std::string line = "Groups: ";
	for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
		line.append(*it + " ");
	
	LOG() << line;
}