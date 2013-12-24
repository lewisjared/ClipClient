#pragma once


#include <string>

#include "czmq.h"


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
private:
	NodeThread* m_node;
	void* m_pipe;
};

