#include "Message.h"

Message::Message(msg_t type)
	:m_id(type)
{
	m_address = NULL;
}

Message::~Message()
{
	if (m_address)
		zframe_destroy(&m_address);
}

msg_t Message::getID() const
{
	return m_id;
}

void Message::setAddress(zframe_t* address)
{
	if (m_address)
		zframe_destroy(&m_address);
	m_address = address;
}

void Message::setSequence(uint16_t sequence)
{
	m_sequence = sequence;
}


