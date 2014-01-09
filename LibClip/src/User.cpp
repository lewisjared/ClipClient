#include "User.h"

#include "boost/uuid/nil_generator.hpp"


CUser::CUser()
	:m_uuid(uuids::nil_uuid())
{
}

CUser::CUser(uuids::uuid uuid, KeyValuePair kvp)
	:m_uuid(uuid), m_headers(kvp)
{

}


CUser::~CUser(void)
{
}

std::string CUser::getName() const
{
	return m_headers.getValue("name");
}

KeyValuePair CUser::getHeaders() const
{
	return m_headers;
}

uuids::uuid CUser::getUUID() const
{
	return m_uuid;
}