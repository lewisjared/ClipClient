#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include "czmq.h"

#include <boost/uuid/uuid.hpp>

class EByteStream : public std::runtime_error {
public:
	EByteStream(const std::string& message) 
		: std::runtime_error(message) { };
};

#define BS_DEFAULT_ALLOCATE 50

/**
 \class	ByteStream

 \brief	Class that contains serialised bytes
		
 */
class ByteStream
{
public:

	/**
	 \fn	ByteStream::ByteStream();
	
	 \brief	Default constructor.
	 */
	ByteStream();
	ByteStream(size_t bufferSize);
	ByteStream(zframe_t* frame);
	ByteStream(const ByteStream& a);

	~ByteStream(void);

	/**
	 \fn	void ByteStream::setFrame(zframe_t* frame);
	
	 \brief	Copies the data from a zframe

			This function takes ownership of the frame.
	
	 \param	frame	The frame.
	 */
	void setFrame(zframe_t* frame);

	void putBlock(void* data, size_t size);
	void getBlock(void* buffer, size_t size);
	void putByte(uint8_t data);
	uint8_t getByte();
	void putUINT16(uint16_t data);
	uint16_t getUINT16();
	void putUINT32(uint32_t data);
	uint32_t getUINT32();
	void putUINT64(uint64_t data);
	uint64_t getUINT64();
	void putString(const std::string& data);
	std::string getString();
	boost::uuids::uuid getUUID();
	void putUUID(boost::uuids::uuid uuid);
	

	void* data() const;
	size_t size() const;
	size_t reservedSize() const;
private:
	void checkSize(size_t toAdd);
	byte* m_writePtr;
	byte* m_readPtr;
	byte* m_start;
	byte* m_end;
};

