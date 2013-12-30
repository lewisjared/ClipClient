#include "ByteStream.h"

ByteStream::ByteStream()
{
	m_start = (byte*) malloc(BS_DEFAULT_ALLOCATE);
	m_end = m_start + BS_DEFAULT_ALLOCATE;
	m_readPtr = m_start;
	m_writePtr = m_start;
}
ByteStream::ByteStream(size_t bufferSize)
{
	m_start = (byte*) malloc(bufferSize);
	m_end = m_start + bufferSize;
	m_readPtr = m_start;
	m_writePtr = m_start;
}

/**
 \fn	ByteStream::ByteStream(const ByteStream& a);

 \brief	Copy constructor.
		Should probably use shared_ptr instead

 \param	a	The const ByteStream&amp; to process.
 */
ByteStream::ByteStream(const ByteStream& a)
{
	size_t reserved = a.reservedSize();
	m_start = (byte*) malloc(reserved);
	memcpy(m_start, a.m_start, reserved);
	m_end = m_start + reserved;
	m_readPtr = m_start + (a.m_readPtr-a.m_start);
	m_writePtr = m_start + (a.m_writePtr-a.m_start);
}

ByteStream::ByteStream(zframe_t* frame, bool takeOwnership)
	:m_start(NULL)
{
	setFrame(frame, takeOwnership);
}

/**
 \fn	ByteStream& ByteStream::operator= (const ByteStream& other)

 \brief	Assignment operator.

 \param	other	The bytestream to be copied.

 \return	A deep copy of this object.
 */
ByteStream& ByteStream::operator= (const ByteStream& other)
{
	if (m_start)
		free(m_start);
	//Perform a deep copy
	size_t reserved = other.reservedSize();
	m_start = (byte*) malloc(reserved);
	memcpy(m_start, other.m_start, reserved);
	m_end = m_start + reserved;
	m_readPtr = m_start + (other.m_readPtr-other.m_start);
	m_writePtr = m_start + (other.m_writePtr-other.m_start);

	return *this;
}

ByteStream::~ByteStream(void)
{
	free(m_start);
}

void ByteStream::checkSize(size_t toAdd)
{
	size_t newSize = size() + toAdd;
	byte* newStart = NULL;
	if (reservedSize() < toAdd)
	{
		newStart = (byte*)realloc((void*)m_start, newSize*2);
		m_readPtr = newStart + (m_readPtr-m_start);
		m_writePtr = newStart + (m_writePtr-m_start);
		m_end = newStart + newSize*2;
		m_start = newStart;
	}
}

void* ByteStream::data() const
{
	return m_start;
}

size_t ByteStream::size() const
{
	return m_writePtr - m_start;
}

size_t ByteStream::reservedSize() const
{
	return m_end - m_start;
}

zframe_t* ByteStream::getFrame()
{
	return zframe_new(m_start, size());
}

void ByteStream::setFrame( zframe_t* frame, bool takeOwnership) 
{
	if (m_start)
		free(m_start);
	m_start = (byte*)malloc(zframe_size(frame));
	memcpy(m_start, zframe_data(frame),zframe_size(frame));
	m_end = m_start + zframe_size(frame);
	m_readPtr = m_start;
	m_writePtr = m_end;

	//Destroy frame
	if (takeOwnership)
		zframe_destroy(&frame);
}

void ByteStream::putBlock( void* data, size_t size )
{
	checkSize(size);
	memcpy(m_writePtr, data, size);
	m_writePtr += size;
}

void ByteStream::getBlock( void* buffer, size_t size  )
{
	if (m_readPtr + size < m_end)
		throw EByteStream("Malformed Packet");
	memcpy(buffer,m_readPtr, size);
	m_readPtr += size;
}

void ByteStream::putByte( uint8_t data )
{
	checkSize(1);
	m_writePtr[0] = data;
	m_writePtr++;
}

uint8_t ByteStream::getByte()
{
	uint8_t result;
	if (m_readPtr + 1 > m_end)
		throw EByteStream("Malformed Packet");
	result = (uint8_t)m_readPtr [0];
	m_readPtr += 1;

	return result;
}

void ByteStream::putUINT16( uint16_t data )
{
	checkSize(2);
	m_writePtr [0] = (uint8_t) (((data) >> 8)  & 255); 
	m_writePtr [1] = (uint8_t) (((data))       & 255); 
	m_writePtr += 2; 
}

uint16_t ByteStream::getUINT16()
{
	uint16_t result;
	if (m_readPtr + 2 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint16_t) (m_readPtr [0]) << 8) \
		+ ((uint16_t) (m_readPtr [1]));
	m_readPtr += 2;

	return result;
}

void ByteStream::putUINT32( uint32_t data )
{
	checkSize(4);
	m_writePtr [0] = (uint8_t) (((data) >> 24) & 255); 
	m_writePtr [1] = (uint8_t) (((data) >> 16) & 255); 
	m_writePtr [2] = (uint8_t) (((data) >> 8)  & 255); 
	m_writePtr [3] = (uint8_t) (((data))       & 255); 
	m_writePtr += 4; 
}

uint32_t ByteStream::getUINT32()
{
	uint32_t result;
	if (m_readPtr + 4 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint32_t) (m_readPtr [0]) << 24) \
		+ ((uint32_t) (m_readPtr [1]) << 16) \
		+ ((uint32_t) (m_readPtr [2]) << 8) \
		+ ((uint32_t) (m_readPtr [3]));
	m_readPtr += 4;

	return result;
}

void ByteStream::putUINT64( uint64_t data )
{
	checkSize(8);
	m_writePtr [0] = (uint8_t) (((data) >> 56) & 255); 
	m_writePtr [1] = (uint8_t) (((data) >> 48) & 255); 
	m_writePtr [2] = (uint8_t) (((data) >> 40) & 255); 
	m_writePtr [3] = (uint8_t) (((data) >> 32) & 255); 
	m_writePtr [4] = (uint8_t) (((data) >> 24) & 255); 
	m_writePtr [5] = (uint8_t) (((data) >> 16) & 255); 
	m_writePtr [6] = (uint8_t) (((data) >> 8)  & 255); 
	m_writePtr [7] = (uint8_t) (((data))       & 255); 
	m_writePtr += 8; 
}

uint64_t ByteStream::getUINT64()
{
	uint64_t result;
	if (m_readPtr + 8 > m_end)
		throw EByteStream("Malformed Packet");
	(result) = ((uint64_t) (m_readPtr [0]) << 56) \
		+ ((uint64_t) (m_readPtr [1]) << 48) \
		+ ((uint64_t) (m_readPtr [2]) << 40) \
		+ ((uint64_t) (m_readPtr [3]) << 32) \
		+ ((uint64_t) (m_readPtr [4]) << 24) \
		+ ((uint64_t) (m_readPtr [5]) << 16) \
		+ ((uint64_t) (m_readPtr [6]) << 8) \
		+  (uint64_t) (m_readPtr [7]); 
	m_readPtr += 8;
	return result;
}

void ByteStream::putString( const std::string& data )
{
	uint8_t len = data.size();
	checkSize(len + 1);
	putByte(len);
	memcpy(m_writePtr, data.c_str(), len);
	m_writePtr += len;
}

std::string ByteStream::getString()
{
	//Byte representing length of the string
	uint8_t len = getByte();
	if (m_readPtr + len > m_end)
		throw EByteStream("Malformed Packet");
	std::string result = std::string((char*)m_readPtr, len);
	m_readPtr += len;

	return result;
}

void ByteStream::putUUID(boost::uuids::uuid uuid)
{
	checkSize(16);
	memcpy(m_writePtr, &uuid, 16);
	m_writePtr += 16;
}

boost::uuids::uuid ByteStream::getUUID()
{
	boost::uuids::uuid uuid;
	memcpy(&uuid, m_readPtr, 16);
	m_readPtr += 16;

	return uuid;
}