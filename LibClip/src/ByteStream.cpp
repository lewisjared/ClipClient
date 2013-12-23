#include "ByteStream.h"


ByteStream::ByteStream()
{
	m_data = NULL;
	m_start = NULL;
	m_end = NULL;
	m_allocated = false;
}
ByteStream::ByteStream(size_t bufferSize)
{
	m_data = new byte[bufferSize];
	m_start = m_data;
	m_end = m_data + bufferSize;
	m_allocated = true;
}

/**
 \fn	ByteStream::ByteStream(const ByteStream& a);

 \brief	Copy constructor.
		Should probably use shared_ptr instead

 \param	a	The const ByteStream&amp; to process.
 */
ByteStream::ByteStream(const ByteStream& a)
{
	m_data = a.m_data;
	m_start = a.m_start;
	m_end = a.m_end;
	m_allocated = false;
}

ByteStream::ByteStream(zframe_t* frame)
{
	setFrame(frame);
}

ByteStream::~ByteStream(void)
{
	if (m_allocated)
	{
		delete m_start;
	}
}

void* ByteStream::data() const
{
	return m_start;
}

size_t ByteStream::size() const
{
	return m_end - m_start;
}

size_t ByteStream::usedSize() const
{
	return m_data - m_start;
}


void ByteStream::setFrame( zframe_t* frame ) 
{
	m_start = zframe_data(frame);
	m_end = m_data + zframe_size(frame);
	m_data = m_start;
	m_allocated = false;
}

void ByteStream::putBlock( void* data, size_t size )
{
	memcpy(m_data, data, size);
	m_data += size;
}

void ByteStream::getBlock( void* buffer, size_t size  )
{
	if (m_data + size < m_end)
		throw EByteStream("Malformed Packet");
	memcpy(buffer,m_data, size);
	m_data += size;
}

void ByteStream::putByte( uint8_t data )
{
	m_data[0] = data;
	m_data++;
}

uint8_t ByteStream::getByte()
{
	uint8_t result;
	if (m_data + 1 > m_end)
		throw EByteStream("Malformed Packet");
	result = (uint8_t)m_data [0];
	m_data += 1;

	return result;
}

void ByteStream::putUINT16( uint16_t data )
{
	m_data [0] = (uint8_t) (((data) >> 8)  & 255); 
	m_data [1] = (uint8_t) (((data))       & 255); 
	m_data += 2; 
}

uint16_t ByteStream::getUINT16()
{
	uint16_t result;
	if (m_data + 2 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint16_t) (m_data [0]) << 8) \
		+ ((uint16_t) (m_data [1]));
	m_data += 2;

	return result;
}

void ByteStream::putUINT32( uint32_t data )
{
	m_data [0] = (uint8_t) (((data) >> 24) & 255); 
	m_data [1] = (uint8_t) (((data) >> 16) & 255); 
	m_data [2] = (uint8_t) (((data) >> 8)  & 255); 
	m_data [3] = (uint8_t) (((data))       & 255); 
	m_data += 4; 
}

uint32_t ByteStream::getUINT32()
{
	uint32_t result;
	if (m_data + 4 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint32_t) (m_data [0]) << 24) \
		+ ((uint32_t) (m_data [1]) << 16) \
		+ ((uint32_t) (m_data [2]) << 8) \
		+ ((uint32_t) (m_data [3]));
	m_data += 4;

	return result;
}

void ByteStream::putUINT64( uint64_t data )
{
	m_data [0] = (uint8_t) (((data) >> 56) & 255); 
	m_data [1] = (uint8_t) (((data) >> 48) & 255); 
	m_data [2] = (uint8_t) (((data) >> 40) & 255); 
	m_data [3] = (uint8_t) (((data) >> 32) & 255); 
	m_data [4] = (uint8_t) (((data) >> 24) & 255); 
	m_data [5] = (uint8_t) (((data) >> 16) & 255); 
	m_data [6] = (uint8_t) (((data) >> 8)  & 255); 
	m_data [7] = (uint8_t) (((data))       & 255); 
	m_data += 8; 
}

uint64_t ByteStream::getUINT64()
{
	uint64_t result;
	if (m_data + 8 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint64_t) (m_data [0]) << 56) \
		+ ((uint64_t) (m_data [1]) << 48) \
		+ ((uint64_t) (m_data [2]) << 40) \
		+ ((uint64_t) (m_data [3]) << 32) \
		+ ((uint64_t) (m_data [4]) << 24) \
		+ ((uint64_t) (m_data [5]) << 16) \
		+ ((uint64_t) (m_data [6]) << 8) \
		+  (uint64_t) (m_data [7]); 
	m_data += 8;
	return result;
}

void ByteStream::putString( const std::string& data )
{
	uint8_t len = data.size();
	putByte(len);
	memcpy(m_data, data.c_str(), len);
	m_data += len;
}

std::string ByteStream::getString()
{
	//Byte representing length of the string
	uint8_t len = getByte();
	if (m_data + len > m_end)
		throw EByteStream("Malformed Packet");
	std::string result = std::string((char*)m_data, len);
	m_data += len;

	return result;
}

void ByteStream::putUUID(boost::uuids::uuid uuid)
{
	memcpy(m_data, &uuid, 16);
	m_data += 16;
}

boost::uuids::uuid ByteStream::getUUID()
{
	boost::uuids::uuid uuid;
	memcpy(&uuid, m_data, 16);
	m_data += 16;

	return uuid;
}