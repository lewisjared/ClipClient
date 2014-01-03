#pragma once

#include <boost/log/common.hpp>
#include <boost/log/expressions/keyword.hpp>
#include "boost/log/sources/severity_feature.hpp"
#include "boost/log/sources/channel_feature.hpp"
#include "boost/log/sources/severity_channel_logger.hpp"

#include <cstring>


namespace logging=boost::log;


// We define our own severity levels
enum SeverityLevel
{
	DEBUG_SEV,
	INFO_SEV,
	WARN_SEV,
	ERROR_SEV,
	CRIT_SEV
};

// Define the attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", SeverityLevel)

// The operator is used for regular stream formatting
std::ostream& operator<< (std::ostream& strm, SeverityLevel level);

// The operator is used when putting the severity level to log
logging::formatting_ostream& operator<<
	(
	logging::formatting_ostream& strm,
	logging::to_log_manip< SeverityLevel, tag::severity > const& manip
	);

typedef logging::sources::severity_channel_logger_mt<SeverityLevel> Logger;

/**
 \def	DECLARE_LOGGER() Logger m_logger

 \brief	A macro that declares a logger. 
		This macro must be called locally prior to any logging taking place. For example in the class declaration
 */
#define DECLARE_LOGGER() static Logger m_logger

/**
 \def	DEFINE_LOGGER(name) m_logger = Logger(boost::log::keywords::channel = name)

 \brief	A macro that defines a logger in a named channel

 \param	name	The name.
 */
#define DEFINE_LOGGER(class) Logger class::m_logger = Logger(logging::keywords::channel = #class)

#define LOG() BOOST_LOG_SEV(m_logger, DEBUG_SEV)

#define LOG_INFO() BOOST_LOG_SEV(m_logger, INFO_SEV)

#define LOG_WARN() BOOST_LOG_SEV(m_logger, WARN_SEV)

#define LOG_ERR() BOOST_LOG_SEV(m_logger, ERROR_SEV)

#define LOG_CRIT() BOOST_LOG_SEV(m_logger, CRIT_SEV)

/**
 \fn	void initLogging();

 \brief	Initialises the logging system.

		In debug mode this spawns a console and writes logging to cout
		The logs are also written to a file specified by filename
 */
void initLogging(const std::string &filename = std::string());