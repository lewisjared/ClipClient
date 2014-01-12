#pragma once

#include <string>

#include "czmq.h"
#include "boost/uuid/uuid.hpp"

#include "ByteStream.h"
#include "Logger.h"
#include "User.h"

#include "wx/event.h"

class NodeThread;
class CEvent;
class wxEvtHandler;
class wxMutex;


class CNode : public wxEvtHandler
{
public:
	CNode(wxEvtHandler* handler = NULL);
	~CNode(void);

	void* getSocket();

	void join(const std::string &group);
	void leave(const std::string &group);
	void addHeader(const std::string &key, const std::string &value);
	void start();
	void whisper(boost::uuids::uuid target, const ByteStream& bs);
	void whisper(boost::uuids::uuid target, const std::string &text);
	void shout(const std::string &group, const ByteStream& bs);
	void shout(const std::string &group, const std::string &text);
	UserList getUserList();
private:
	void onZyreEvent(wxThreadEvent& event);
	wxEvtHandler* m_handler;
	wxMutex m_mutex;
	NodeThread* m_node;
	UserList m_userList;
	void* m_pipe;
	zctx_t* m_context;
	DECLARE_LOGGER();
};