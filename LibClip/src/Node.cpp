#include "Node.h"
#include "NodeThread.h"

Node::Node(zctx_t* context)
{
	m_node = new NodeThread(context);
	m_pipe = m_node->run();
}


Node::~Node(void)
{
	delete m_node;
}

void Node::addHeader(const std::string &key, const std::string &value)
{
	zstr_sendm(m_pipe, "SET");
	zstr_sendm(m_pipe, key.c_str());
	zstr_send(m_pipe, value.c_str());
}

void Node::join(const std::string &group)
{
	zstr_sendm(m_pipe, "JOIN");
	zstr_send(m_pipe, group.c_str());
}

void Node::leave(const std::string &group)
{
	zstr_sendm(m_pipe, "LEAVE");
	zstr_send(m_pipe, group.c_str());
}

void Node::start()
{
	zstr_send(m_pipe, "START");
	char* resp = zstr_recv(m_pipe);
	assert(std::string(resp) == "OK");
	zstr_free(&resp);
}

void Node::whisper(boost::uuids::uuid target, const ByteStream& bs)
{

}

void Node::shout(const ByteStream& bs)
{
	zstr_sendm(m_pipe, "SHOUT");
	zmsg_t* msg = zmsg_new();

	zframe_t* frame = zframe_new(bs.data(), bs.size());
	zmsg_append(msg, &frame);
	zmsg_send(&msg, m_pipe);
	zmsg_destroy(&msg);
}