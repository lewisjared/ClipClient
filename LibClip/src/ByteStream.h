#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include "czmq.h"

class EByteStream : public std::runtime_error {
public:
	EByteStream(const std::string& message) 
		: std::runtime_error(message) { };
};

class ByteStream
{
public:
	ByteStream(size_t bufferSize);
	ByteStream(zframe_t* frame);
	~ByteStream(void);

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
	

	void* data() const;
	size_t size() const;
	size_t usedSize() const;
private:
	bool m_allocated;
	byte* m_data;
	byte* m_start;
	byte* m_end;
};

