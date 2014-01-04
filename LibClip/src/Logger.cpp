#include "Logger.h"

#include <fstream>

#include <boost/shared_ptr.hpp>
#include "boost/utility/empty_deleter.hpp"
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include "boost/log/expressions.hpp"
#include <boost/log/expressions/formatters/stream.hpp>
#include "boost/log/expressions/formatters/date_time.hpp"
#include <boost/log/support/date_time.hpp>
#include "boost/log/utility/setup/common_attributes.hpp"

std::ostream& operator<< (std::ostream& strm, SeverityLevel level)
{
	static const char* strings[] =
	{
		"debug",
		"info",
		"warning",
		"error",
		"critical"
	};

	if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[level];
	else
		strm << static_cast< int >(level);

	return strm;
}


logging::formatting_ostream& operator<<
	(
	logging::formatting_ostream& strm,
	logging::to_log_manip< SeverityLevel, tag::severity > const& manip
	)
{
	static const char* strings[] =
	{
		"DBUG",
		"INFO",
		"WARN",
		"ERRR",
		"CRIT"
	};

	SeverityLevel level = manip.get();
	if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[level];
	else
		strm << static_cast< int >(level);

	return strm;
}

void initLogging(const std::string &filename)
{
	boost::shared_ptr< logging::core > core = logging::core::get();

	//Create a sink back end that logs to cout and file
	boost::shared_ptr< logging::sinks::text_ostream_backend > backend =
		boost::make_shared< logging::sinks::text_ostream_backend >();
	backend->add_stream( boost::shared_ptr< std::ostream >(&std::cout, boost::empty_deleter()));

	if(!filename.empty())
		backend->add_stream( boost::shared_ptr< std::ostream >(new std::ofstream(filename)));

	//backend->auto_flush(true);

	typedef logging::sinks::synchronous_sink< logging::sinks::text_ostream_backend > sink_t;
	boost::shared_ptr< sink_t > sink(new sink_t(backend));

	sink->set_formatter	(
		logging::expressions::stream << logging::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%b %d %H:%M:%S.%f")
		<< " <" << severity << "> "
		<< "[" << logging::expressions::attr< std::string >("Channel") << "] : "
		<< logging::expressions::message
		);
	core->add_sink(sink);
	logging::add_common_attributes();
}