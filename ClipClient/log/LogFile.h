#ifdef WIN32
/**
 * \file	log/LogPolicy.h
 *
 * \brief	Declares the a policy to log to a file
 */
#ifndef FILELOGPOLICY_H
#define FILELOGPOLICY_H

#include "log/LogPolicy.h"

#include <memory>
#include <string>
#include <fstream>


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
	virtual void writeLine( const std::string& msg );
	virtual bool isOpen();
	virtual std::ostream& getStream();
};


#endif
#endif // WIN32