#ifdef WIN32
/**
 * \file	log\LogPolicy.h
 *
 * \brief	Declares the abstract base class for logging policies
 */

#ifndef LOGPOLICY_H
#define LOGPOLICY_H

#include <string>
#include <ostream>

/**
 \class	LogPolicy

 \brief	An abstract base class to represent a logging policy.

 Inherited classes must override abstract functions to provide functionality
 */

class LogPolicy
{
public:
	virtual void openOStream(const std::string& name) = 0;
	virtual void closeOStream() = 0;
	virtual void writeLine(const std::string& msg) = 0;
	virtual bool isOpen() = 0;
	virtual std::ostream& getStream() = 0;
};


#endif
#endif // WIN32