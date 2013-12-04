
#ifndef ZRE_HEADER_H
#define ZRE_HEADER_H

#include <map>
#include <string>

enum msg_typ
{
	ZRE_MSG_HELLO,
	ZRE_MSG_WHISPER,
	ZRE_MSG_SHOUT,
	ZRE_MSG_JOIN,
	ZRE_MSG_PING,
	ZRE_MSG_PING_OK
};

typedef std::map<std::string, std::string> KeyValuePair;

class ZRE_Msg
{
public:
	ZRE_Msg(msg_typ id);
	~ZRE_Msg();

	static ZRE_Msg* parse(void* input);

	KeyValuePair getHeaders();
	std::ostream& dump() const;

	int send(void* socket)
private:
	msg_typ m_id;
	KeyValuePair m_headers;
};

#endif