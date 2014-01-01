#include "Node.h"
#include "NodeThread.h"
#include "Event.h"

using namespace zyre;

Node::Node()
{
	m_context = zctx_new();
	m_node = new NodeThread(m_context);
	m_pipe = m_node->run();
}


Node::~Node(void)
{
	delete m_node;
	zctx_destroy(&m_context);
}

void Node::addHeader(const std::string &key, const std::string &value)
{
	zstr_sendm(m_pipe, "SET");
	zstr_sendm(m_pipe, key.c_str());
	zstr_send(m_pipe, value.c_str());
}

void* Node::getSocket()
{
	return m_pipe;
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
	zmsg_t* msg = zmsg_new();
	zmsg_pushmem(msg, &target, 16);
	zmsg_pushstr(msg,"WHISPER");
	
	zframe_t* frame = zframe_new(bs.data(), bs.size());
	zmsg_append(msg, &frame);
	zmsg_send(&msg, m_pipe);
}

void Node::whisper( boost::uuids::uuid target, const std::string &text )
{
	ByteStream bs;
	bs.putString(text);
	whisper(target, bs);
}


void Node::shout(const std::string &group, const ByteStream& bs)
{
	zmsg_t* msg = zmsg_new();
	zmsg_pushstr(msg,group.c_str());
	zmsg_pushstr(msg,"SHOUT");
	
	zframe_t* frame = zframe_new(bs.data(), bs.size());
	zmsg_append(msg, &frame);
	zmsg_send(&msg, m_pipe);
}

void Node::shout(const std::string &group, const std::string &text)
{
	ByteStream bs;
	bs.putString(text);
	shout(group,bs);
}

zyre::Event* Node::recv()
{
	//Read in a complete message
	// Blocks till message is ready
	zmsg_t* msg = zmsg_recv(m_pipe);

	//Parse the Event out of the message takes ownership of message
	Event* event = Event::parse(msg);


	//May as well null any invalid messages
	if (event)
		if (!event->isValid())
		{
			delete event;
			event = NULL;
		}

	return event;
}
