#ifndef LOGPOLICY_H
#define LOGPOLICY_H

#include <string>

namespace logging
{
	
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
	virtual void write(const std::string& msg) = 0;
	virtual bool isOpen() = 0;
};
}

#endif