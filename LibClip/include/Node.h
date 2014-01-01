#pragma once


#include <string>

#include "czmq.h"
#include "boost/uuid/uuid.hpp"

#include "ByteStream.h"

class NodeThread;


namespace zyre
{

	class Event;

class Node
{
public:
	Node();
	~Node(void);

	void* getSocket();

	Event* recv();

	void join(const std::string &group);
	void leave(const std::string &group);
	void addHeader(const std::string &key, const std::string &value);
	void start();
	void whisper(boost::uuids::uuid target, const ByteStream& bs);
	void whisper(boost::uuids::uuid target, const std::string &text);
	void shout(const std::string &group, const ByteStream& bs);
	void shout(const std::string &group, const std::string &text);
private:
	NodeThread* m_node;
	void* m_pipe;
	zctx_t* m_context;
};

}