#include "ByteStream.h"


ByteStream::ByteStream(void)
{
}


ByteStream::~ByteStream(void)
{
}

byte* ByteStream::data()
{
	return m_ss.str().c_str();
}

size_t ByteStream::size()
{
	return m_ss.str().size();
}
