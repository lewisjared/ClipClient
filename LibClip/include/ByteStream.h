#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include "czmq.h"
#include "Logger.h"
#include <boost/uuid/uuid.hpp>

class EByteStream : public std::runtime_error {
public:
	EByteStream(const std::string& message) 
		: std::runtime_error(message) { };
};

#define BS_DEFAULT_ALLOCATE 10

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
	ByteStream(zframe_t* frame, bool takeOwnership = true);
	ByteStream(const ByteStream& a);
	ByteStream& operator= (const ByteStream& other);

	~ByteStream(void);

	void append(const ByteStream& bs);

	/**
	 \fn	zframe_t* ByteStream::getFrame();
	
	 \brief	Gets the data as a zframe_t. The calling function takes ownership of this frame and must destroy it.
	
	 \return	A zframe_t containing data
	 */
	zframe_t* getFrame();

	/**
	 \fn	void ByteStream::setFrame(zframe_t* frame);
	
	 \brief	Copies the data from a zframe

			This function takes ownership of the frame.
	
	 \param	frame	The frame.
	 */
	void setFrame(zframe_t* frame,bool takeOwnership=true);

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
	DECLARE_LOGGER();
};

std::ostream& operator<< (std::ostream& os, const ByteStream &bs);