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

bool CUser::isValid() const
{
	return !m_uuid.is_nil();
}

/************************************************************************/
/* UserList                                                                     */
/************************************************************************/
UserList::UserList()
{

}

void UserList::addUser(const CUser& user)
{
	m_map[user.getUUID()] = user;
}

void UserList::removeUser(const CUser& user)
{
	auto it = m_map.find(user.getUUID());
	if (it != m_map.end())
		m_map.erase(it);
}

CUser UserList::getUserByName(const std::string& name) const
{
	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		if (it->second.getName() == name)
			return it->second;
	}
	return CUser();
}

CUser UserList::getUserByUUID(uuids::uuid uuid) const
{
	auto it = m_map.find(uuid);
	if (it != m_map.end())
		return it->second;
	return CUser();
}