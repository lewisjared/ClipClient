#pragma once

#include <vector>
#include <sstream>


typedef unsigned char byte;

class ByteStream
{
public:
	ByteStream(void);
	~ByteStream(void);

	byte* data();
	size_t size();
private:
	std::stringstream m_ss;
};

