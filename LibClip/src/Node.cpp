#include "Node.h"
#include "NodeThread.h"
#include "Event.h"
#include "Zyre.h"

#include "boost/uuid/uuid_io.hpp"


DEFINE_LOGGER(CNode);

CNode::CNode(wxEvtHandler* handler)
	:wxEvtHandler()
{
	m_context = zctx_new();
	m_node = new NodeThread(m_context);
	m_node->setEventHandler(this);
	m_pipe = m_node->run();

	Bind(ZYRE_EVENT, &CNode::onZyreEvent, this);
}


CNode::~CNode(void)
{
	delete m_node;
	zctx_destroy(&m_context);
}

void CNode::addHeader(const std::string &key, const std::string &value)
{
	zstr_sendm(m_pipe, "SET");
	zstr_sendm(m_pipe, key.c_str());
	zstr_send(m_pipe, value.c_str());
}

void* CNode::getSocket()
{
	return m_pipe;
}

void CNode::join(const std::string &group)
{
	zstr_sendm(m_pipe, "JOIN");
	zstr_send(m_pipe, group.c_str());
}

void CNode::leave(const std::string &group)
{
	zstr_sendm(m_pipe, "LEAVE");
	zstr_send(m_pipe, group.c_str());
}

void CNode::start()
{
	zstr_send(m_pipe, "START");
	char* resp = zstr_recv(m_pipe);
	assert(std::string(resp) == "OK");
	zstr_free(&resp);
}

void CNode::whisper(boost::uuids::uuid target, const ByteStream& bs)
{
	zmsg_t* msg = zmsg_new();
	zmsg_pushmem(msg, &target, 16);
	zmsg_pushstr(msg,"WHISPER");
	
	zframe_t* frame = zframe_new(bs.data(), bs.size());
	zmsg_append(msg, &frame);
	zmsg_send(&msg, m_pipe);
}

void CNode::whisper( boost::uuids::uuid target, const std::string &text )
{
	ByteStream bs;
	bs.putString(text);
	whisper(target, bs);
}


void CNode::shout(const std::string &group, const ByteStream& bs)
{
	zmsg_t* msg = zmsg_new();
	zmsg_pushstr(msg,group.c_str());
	zmsg_pushstr(msg,"SHOUT");
	
	zframe_t* frame = zframe_new(bs.data(), bs.size());
	zmsg_append(msg, &frame);
	zmsg_send(&msg, m_pipe);
}

void CNode::shout(const std::string &group, const std::string &text)
{
	ByteStream bs;
	bs.putString(text);
	shout(group,bs);
}


UserList CNode::getUserList()
{
	wxMutexLocker locker(m_mutex);
	return m_userList; //Puts a copy on the stack before the local lock is destroyed
}

void CNode::onZyreEvent(wxThreadEvent& event)
{
	CEvent evt = event.GetPayload<CEvent>();

	wxMutexLocker locker(m_mutex);
	if (evt.getType() == EVT_ENTER)
	{
		LOG() << "Adding user " << evt.getFrom() << " to userlist";
		CUser newUser(evt.getFrom(), evt.getHeaders());
		m_userList.addUser(newUser);
	} else if (evt.getType() == EVT_EXIT)
	{
		LOG() << "Removing user " << evt.getFrom() << " from userlist";
		CUser user = m_userList.getUserByUUID(evt.getFrom());
		m_userList.removeUser(user);
	}

	//Forwards event on to the main application handler
	m_handler->ProcessEvent(event);
}