#ifdef WIN32
#include "LogFile.h"

#include <assert.h>


FileLogPolicy::FileLogPolicy()
	:m_stream(new std::ofstream)
{
}

FileLogPolicy::~FileLogPolicy()
{
	closeOStream();
}

void FileLogPolicy::openOStream( const std::string& name )
{
	m_stream->open(name.c_str(), std::ios::out | std::ios::trunc);

	assert(m_stream->is_open());
}

void FileLogPolicy::closeOStream()
{
	if (m_stream)
		m_stream->close();
}

bool FileLogPolicy::isOpen()
{
	if (m_stream)
		return m_stream->is_open();
	return false;
}

void FileLogPolicy::writeLine( const std::string& msg )
{
	(*m_stream) << msg << std::endl;
	m_stream->flush();
}

std::ostream& FileLogPolicy::getStream()
{
	return (*m_stream);
}

#endif // WIN32
