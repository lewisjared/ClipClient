
#ifndef MESSAGE_H
#define MESSAGE_H

#include <map>
#include <vector>
#include <string>
#include "czmq.h"
#include "ByteStream.h"

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

#define HEADER_SIZE 3


class Message
{
public:
	Message(msg_t id);
	~Message();
	msg_t getID() const;

	std::ostream& dump() const;

	virtual int send(void* socket) = 0;

	void setSequence(uint16_t sequence);
	void setAddress(zframe_t* address);
	zframe_t* getAddress() const;
protected:
	int sendBytes(void* socket, ByteStream bs, int flags);
	msg_t m_id;
	uint16_t m_sequence;
	zframe_t* m_address;
};

typedef std::map<std::string, std::string> KeyValuePair;
typedef std::vector<std::string> TStringVector;

class MessageHello : public Message
{
public:
	MessageHello();

	KeyValuePair getHeaders();
	void addHeader(const std::string& keyValue);
	void addHeader(const std::string& key, const std::string& value);
	void addGroup(const std::string& group);
	void setIP(std::string ip);
	void setMailbox(uint16_t mailboxPort);
	void setStatus(uint8_t status);

	int send(void* socket);
private:
	KeyValuePair m_headers;
	TStringVector m_groups;
	uint16_t m_mailboxPort;
	uint8_t m_status;
	std::string m_ip;
};

class MessageWhisper : public Message
{
public:
	MessageWhisper();
	zmsg_t* getContent();

	int send(void* socket);
private:
	zmsg_t* m_content;
};

class MessageShout : public Message
{
public:
	MessageShout();
	zmsg_t* getContent();
	std::string getGroup() const;
	void setGroup(const std::string& group);

	int send(void* socket);
private:
	std::string m_group;
	zmsg_t* m_content;
};

class MessageJoin : public Message
{
public:
	MessageJoin();

	std::string getGroup() const;
	void setGroup(const std::string& group);
	uint8_t getStatus() const;
	void setStatus(uint8_t status);

	int send(void* socket);
private:
	std::string m_group;
	uint8_t m_status;

};

class MessageLeave : public Message
{
public:
	MessageLeave();

	std::string getGroup() const;
	void setGroup(const std::string& group);
	uint8_t getStatus() const;
	void setStatus(uint8_t status);

	int send(void* socket);
private:
	std::string m_group;
	uint8_t m_status;
};

class MessagePing :public Message
{
public:
	MessagePing();
	int send(void* socket);
};

class MessagePingOK : public Message
{
public:
	MessagePingOK();
	int send(void* socket);
};

#endif