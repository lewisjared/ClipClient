#pragma once

#include <vector>


typedef unsigned char byte;

class ByteStream
{
public:
	ByteStream(void);
	ByteStream
	~ByteStream(void);

	byte* data();
	size_t size();
private:
	stringstream m_ss;
};

