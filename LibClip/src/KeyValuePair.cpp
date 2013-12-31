#include "KeyValuePair.h"
#include "Logger.h"

#include <cassert>

void KeyValuePair::addValue(const std::string &key, const std::string &value)
{
	//Overwrite new value
	m_data[key] = value;
}

void KeyValuePair::addValue(const std::string &keyValue)
{
	size_t pos = keyValue.find_first_of('=');
	std::string key = keyValue.substr(0, pos);
	std::string value = keyValue.substr(pos+1);

	addValue(key, value);
}

std::string KeyValuePair::getValue(const std::string &key) const
{
	auto it = m_data.find(key);
	if (it == m_data.end())
		return "";

	return it->second;
}

std::string KeyValuePair::getValueLine(const std::string &key) const
{
	return key + "=" + getValue(key);
}

ByteStream KeyValuePair::generateBS() const
{
	ByteStream bs;
	bs.putString("KEYVALUEPAIR");
	bs.putUINT16(m_data.size());
	for (auto it = m_data.begin(); it != m_data.end(); ++it)
	{
		std::string line = it->first + "=" + it->second;
		bs.putString(line);
	}
	bs.putString("END");
	return bs;
}

void KeyValuePair::log() const
{
	for (auto it = m_data.begin(); it != m_data.end(); ++it)
		LOG() << "\t" << it->first + "=" + it->second << std::endl;
	LOG() << "\t" << "end" << std::endl;
}

void KeyValuePair::parseBS(ByteStream& bs)
{
	std::string header = bs.getString();
	assert(header == "KEYVALUEPAIR");
	uint16_t size = bs.getUINT16();

	for (int i = 0; i < size; i++)
	{
		std::string line = bs.getString();
		addValue(line);
	}
	std::string end = bs.getString();
	assert(end == "END");
}