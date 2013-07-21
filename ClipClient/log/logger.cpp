#include <cassert>
#include <ctime>
#include <sstream>

using namespace logging;


template<typename log_policy>
Logger<log_policy>::Logger()
{
	m_lineNum = 0;
	m_policy = new log_policy;
	
	m_mutex = CreateMutex(NULL,false,NULL);
}

template<typename log_policy>
Logger<log_policy>::~Logger()
{
	delete m_policy;
}

template<typename log_policy>
Logger<log_policy>& Logger<log_policy>::getInstance()
{
	static Logger<log_policy> logger;

	return logger;
}

template<typename log_policy>
std::string Logger<log_policy>::getTime()
{
	std::string timeStr;
	time_t currentTime;
	time(&currentTime);

	timeStr = ctime(&currentTime);

	//Drop the newline char
	return timeStr.substr(0, timeStr.length()-1);
}

template<typename log_policy>
std::string Logger<log_policy>::getHeaderText()
{
	std::stringstream text;
	text.width(5);
	text << m_lineNum++ << " < " << getTime() << " > ";

	return text.str();
}

template<typename log_policy>
void Logger<log_policy>::setName(const std::string& name)
{
	m_name = name;
	m_policy->openOStream(name);
}

template<typename log_policy>
void Logger<log_policy>::print(const std::string& msg)
{
	printImpl(msg,DEBUG);
}

template<typename log_policy>
void Logger<log_policy>::printErr(const std::string& msg)
{
	printImpl(msg,ERROR_SEV);
}

template<typename log_policy>
void Logger<log_policy>::printWarn(const std::string& msg)
{
	printImpl(msg,WARNING);
}

template<typename log_policy>
void Logger<log_policy>::printImpl(const std::string& msg, severity_type severity)
{
	assert(m_policy->isOpen());

	//TODO check severity level
	
	WaitForSingleObject( m_mutex, INFINITE );
	std::string line = getHeaderText();
	switch(severity)
	{
	case DEBUG:
		line.append("<DEBUG> : ");
		break;
	case WARNING:
		line.append("<WARNING> : ");
		break;
	case ERROR_SEV:
		line.append("<ERROR> : ");
		break;
	}
	line.append(msg);
	m_policy->write(line);
	ReleaseMutex( m_mutex );
}