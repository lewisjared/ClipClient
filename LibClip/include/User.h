#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>

#include "KeyValuePair.h"

namespace uuids = boost::uuids;

class CUser
{
public:
	CUser();
	CUser(uuids::uuid uuid, KeyValuePair kvp);
	virtual ~CUser(void);

	bool isValid() const;
	std::string getName() const;
	KeyValuePair getHeaders() const;
	uuids::uuid getUUID() const;
protected:
	uuids::uuid m_uuid;
	KeyValuePair m_headers;
};

typedef std::map< uuids::uuid, CUser> UserMap;

class UserList
{
public:
	UserList();

	/**
	 \fn	void UserList::addUser(const CUser& user);
	
	 \brief	Adds a user to the list.
			Overwrites an existing user with the same UUID
	 \param	user	The user.
	 */
	void addUser(const CUser& user);
	void removeUser(const CUser& user);

	CUser getUserByName(const std::string& name) const;
	CUser getUserByUUID(uuids::uuid uuid) const;

private:
	UserMap m_map;
};
