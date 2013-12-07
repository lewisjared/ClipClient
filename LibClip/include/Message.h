
#ifndef MESSAGE_H
#define MESSAGE_H

#include <map>
#include <string>


enum msg_t
{
	MSG_HELLO = 1,
	MSG_WHISPER,
	MSG_SHOUT,
	MSG_JOIN,
	MSG_LEAVE,
	MSG_PING,
	MSG_PING_OK
};

typedef std::map<std::string, std::string> KeyValuePair;

class Message
{
public:
	Message(msg_t id);
	~Message();

	KeyValuePair getHeaders();
	std::ostream& dump() const;

	int send(void* socket);
private:
	msg_t m_id;
	KeyValuePair m_headers;
};

#endif