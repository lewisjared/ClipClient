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

	std::string getName() const;
	KeyValuePair getHeaders() const;
	uuids::uuid getUUID() const;
protected:
	uuids::uuid m_uuid;
	KeyValuePair m_headers;
};

