#ifdef WIN32
#include <cassert>
#include <ctime>
#include <sstream>

#include "Logger.h"
#include "LogFile.h"
#include "LogConsole.h"

Logger::Logger()
{
	m_nullStream = new std::ostream(NULL);
	m_lineNum = 0;
	m_severity = WARNING;
	m_file = NULL;

	enableConsole();
}

Logger::~Logger()
{
	delete m_file;
}


Logger& Logger::getInstance()
{
	static Logger logger;

	return logger;
}

void Logger::enableConsole()
{
	if (m_file)
		delete m_file;
	m_file = new ConsoleLogPolicy;
}

void Logger::enableFile(const std::string& name)
{
	if (m_file)
		delete m_file;
	m_name = name;
	m_file = new FileLogPolicy;
	m_file->openOStream(name);
}

std::string Logger::getTime()
{
	std::string timeStr;
	time_t currentTime;
	time(&currentTime);

	timeStr = ctime(&currentTime);

	//Drop the newline char
	return timeStr.substr(0, timeStr.length()-1);
}

std::string Logger::getHeaderText()
{
	std::stringstream text;
	text.width(5);
	text << m_lineNum++ << " < " << getTime() << " > ";

	return text.str();
}

std::ostream& Logger::print()
{
	if (m_severity <= DEBUG_LOG)
	{
		printHeader(DEBUG_LOG);
		return m_file->getStream();
	} else {
		return *m_nullStream;
	}
}

std::ostream& Logger::printInfo()
{
	//Info is always printed
	printHeader(DEBUG_LOG);
	return m_file->getStream();
}

std::ostream& Logger::printErr()
{
	if (m_severity <= ERROR_SEV)
	{
		printHeader(ERROR_SEV);
		return m_file->getStream();
	} else {
		return *m_nullStream;
	}
}

std::ostream& Logger::printWarn()
{
	if (m_severity <= WARNING)
	{
		printHeader(WARNING);
		return m_file->getStream();
	} else {
		return *m_nullStream;
	}
}


void Logger::printHeader(Severity severity)
{
	assert(m_file->isOpen());

	std::string line = getHeaderText();
	switch(severity)
	{
	case DEBUG_LOG:
		line.append("<DEBUG> : ");
		break;
	case INFO:
		line.append("<INFO> : ");
		break;
	case WARNING:
		line.append("<WARNING> : ");
		break;
	case ERROR_SEV:
		line.append("<ERROR> : ");
		break;
	}
	m_file->getStream() << line;
}

void Logger::setSeverity(Severity severity)
{
	m_severity = severity;
}
#endif // WIN32