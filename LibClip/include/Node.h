#pragma once


#include <string>

#include "czmq.h"
#include "boost/uuid/uuid.hpp"

#include "ByteStream.h"

class NodeThread;


class Node
{
public:
	Node(zctx_t* context);
	~Node(void);

	void join(const std::string &group);
	void leave(const std::string &group);
	void addHeader(const std::string &key, const std::string &value);
	void start();
	void whisper(boost::uuids::uuid target, const ByteStream& bs);
	void shout(const ByteStream& bs);
private:
	NodeThread* m_node;
	void* m_pipe;
};

