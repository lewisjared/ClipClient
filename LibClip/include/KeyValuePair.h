#pragma once

#include <string>
#include <map>

#include "ByteStream.h"
#include "Logger.h"

class KeyValuePair
{
public:	
	KeyValuePair();

	void addValue(const std::string &key, const std::string &value);
	void addValue(const std::string &keyValue);
	std::string getValue(const std::string &key) const;
	std::string getValueLine(const std::string &key) const;
	void log() const;
	ByteStream generateBS() const;
	void parseBS(ByteStream& bs);
	//static KeyValuePair parseBS(ByteStream& bs);
private:
	std::map<std::string, std::string> m_data;
	DECLARE_LOGGER();
};

