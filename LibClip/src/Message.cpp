#include "Message.h"
#include "Logger.h"

Message::Message(msg_t type)
	:m_id(type)
{
}

Message::~Message()
{
}

msg_t Message::getID() const
{
	return m_id;
}

void Message::setAddress(zframe_t* address)
{
	ByteStream bs(address);
	m_address = bs.getUUID();
}

boost::uuids::uuid Message::getAddress() const
{
	return m_address;
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
	int result = 0;
	//  If we're sending to a ROUTER, we send the address first
	if (zsocket_type (socket) == ZMQ_ROUTER) {
		ByteStream addressByteStream(16);
		addressByteStream.putUUID(m_address);
		zframe_t* frame = zframe_new(addressByteStream.data(), addressByteStream.size());

		if (zframe_send (&frame, socket, ZFRAME_MORE)) 
		{
			LOG_WARN() << "Address frame could not be sent" << std::endl;
			result = -2;
		}
		zframe_destroy (&frame);
	}

	if (result == 0)
	{
		//  Now send the data frame
		zframe_t* frame = zframe_new(bs.data(), bs.size());
		if (zframe_send (&frame, socket, flags)) 
		{
			result = -1;
			LOG_WARN() << "Frame could not be sent" << std::endl;
		}
		zframe_destroy (&frame);
	}

	return result;
}


