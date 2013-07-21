#ifndef FILELOGPOLICY_H
#define FILELOGPOLICY_H

#include "logPolicy.h"

#include <memory>
#include <string>
#include <fstream>


namespace logging
{

/**
 \class	FileLogPolicy

 \brief	File policy for logging to a file.

 Flushes logs as soon as they are written
 */

	class FileLogPolicy : public LogPolicy
	{
		std::unique_ptr<std::ofstream> m_stream;
	public:
		FileLogPolicy();
		~FileLogPolicy();

		virtual void openOStream( const std::string& name );
		virtual void closeOStream();
		virtual void write( const std::string& msg );
		virtual bool isOpen();
	};
}

#endif