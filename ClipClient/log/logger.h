#ifndef LOGGER_H
#define LOGGER_H

#include "fileLogPolicy.h"


namespace logging
{
	/**
	 \enum	severity_type
	
	 \brief	Enumeration for the various levels of warnings
	 */

	enum severity_type
	{
		DEBUG = 1,
		WARNING,
		ERROR_SEV
	};

	template<typename log_policy>
	class Logger
	{
	public:
		~Logger();

		static Logger& getInstance();
		void setName(const std::string& name);

		void print(const std::string& msg);
		void printErr(const std::string& msg);
		void printWarn(const std::string& msg);
	private:
		Logger();
		std::string getTime();
		std::string getHeaderText();
		void printImpl(const std::string& msg, severity_type severity);

		std::string m_name;
		unsigned int m_lineNum;
		log_policy* m_policy;
		HANDLE m_mutex;
	};
}

#include "logger.cpp"
/**
 \brief	Declare a static instance of the logger
 */
 
typedef logging::Logger<logging::FileLogPolicy > MyLogger;

#define LOG MyLogger::getInstance().print
#define LOG_ERR MyLogger::getInstance().printErr
#define LOG_WARN MyLogger::getInstance().printWarn

#endif