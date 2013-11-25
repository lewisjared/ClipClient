/**
 * \file	log/Logger.h
 *
 * \brief	Declares the Logger class
 */
#ifndef LOGGER_H
#define LOGGER_H

#ifdef WIN32
#include "log/LogPolicy.h"
#include <ostream>

/**
	\enum	severity_type
	
	\brief	Enumeration for the various levels of warnings
	*/

enum severity_type
{
	DEBUG = 1,
	INFO,
	WARNING,
	ERROR_SEV
};

class Logger
{
public:
	~Logger();

	static Logger& getInstance();
	void enableFile(const std::string& name);
	void enableConsole();
	void setSeverity(severity_type severity);

	std::ostream&  print();
	std::ostream&  printInfo();
	std::ostream&  printErr();
	std::ostream&  printWarn();
private:
	Logger();
	std::string getTime();
	std::string getHeaderText();
	void printHeader(severity_type severity);

	std::ostream* m_nullStream;
	severity_type m_severity;
	std::string m_name;
	unsigned int m_lineNum;
	LogPolicy* m_file;
};

//#include "BS_Logger.inl"


/**
	Ease of use macros
 */
#define LOG Logger::getInstance().print
#define LOG_INFO Logger::getInstance().printInfo
#define LOG_ERR Logger::getInstance().printErr
#define LOG_WARN Logger::getInstance().printWarn
#endif // WIN32

#endif