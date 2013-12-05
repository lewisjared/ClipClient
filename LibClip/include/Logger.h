/**
 * \file	log/Logger.h
 *
 * \brief	Declares the Logger class
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <ostream>

#include "LogPolicy.h"
/**
	\enum	severity_type
	
	\brief	Enumeration for the various levels of warnings
	*/

enum Severity
{
	DEBUG_LOG,
	INFO,
	WARNING,
	ERROR_SEV,
};

class Logger
{
public:
	~Logger();

	static Logger& getInstance();
	void enableFile(const std::string& name);
	void enableConsole();
	void setSeverity(Severity severity);

	std::ostream&  print();
	std::ostream&  printInfo();
	std::ostream&  printErr();
	std::ostream&  printWarn();
private:
	Logger();
	std::string getTime();
	std::string getHeaderText();
	void printHeader(Severity severity);

	std::ostream* m_nullStream;
	Severity m_severity;
	std::string m_name;
	unsigned int m_lineNum;
	LogPolicy* m_file;
};


/**
	Ease of use macros
 */
#define LOG Logger::getInstance().print
#define LOG_INFO Logger::getInstance().printInfo
#define LOG_ERR Logger::getInstance().printErr
#define LOG_WARN Logger::getInstance().printWarn

#endif