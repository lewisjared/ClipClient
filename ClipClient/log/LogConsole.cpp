#ifdef WIN32
#include "LogConsole.h"

#include <iostream>


ConsoleLogPolicy::ConsoleLogPolicy()
{
}

void ConsoleLogPolicy::openOStream( const std::string& name )
{
	//Do Nothing
}

void ConsoleLogPolicy::closeOStream()
{
	//Do Nothing
}

bool ConsoleLogPolicy::isOpen()
{
	return true;
}

void ConsoleLogPolicy::writeLine( const std::string& msg )
{
	std::cout << msg << std::endl;
}

std::ostream& ConsoleLogPolicy::getStream()
{
	return std::cout;
}

#endif // WIN32
