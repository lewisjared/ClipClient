#ifdef WIN32
/**
 * \file LogConsole.h
 *
 * \brief	Declares the a policy to log to cout

 */
#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include "LogPolicy.h"

#include <memory>
#include <string>
#include <fstream>


/**
 \class	FileLogPolicy

 \brief	File policy for logging to a file.

 Flushes logs as soon as they are written
 */

class ConsoleLogPolicy : public LogPolicy
{
public:
	ConsoleLogPolicy();

	virtual void openOStream( const std::string& name );
	virtual void closeOStream();
	virtual void writeLine( const std::string& msg );
	virtual bool isOpen();
	virtual std::ostream& getStream();
};


#endif
#endif // WIN32