#include "Message.h"
#include "Logger.h"

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

/**
 \fn	int Message::sendBytes( ByteStream bs )

 \brief	Generates and sends a frame over a socket.

 \param	bs	The ByteStream containing data to be sent.

 \return	0 if successful.
 */

int Message::sendBytes( void* socket, ByteStream bs, int flags )
{
	//  If we're sending to a ROUTER, we send the address first
	if (zsocket_type (socket) == ZMQ_ROUTER) {
		assert (m_address);
		if (zframe_send (&m_address, socket, ZFRAME_MORE)) {
			return -1;
		}
	}
	//  Now send the data frame
	zframe_t* frame = zframe_new(bs.data(), bs.size());
	if (zframe_send (&frame, socket, flags)) {
		zframe_destroy (&frame);
		LOG_WARN() << "Frame could not be sent" << std::endl;
		return -1;
	}

	return 0;
}


