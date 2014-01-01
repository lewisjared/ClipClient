#include "StdAfx.h"
#include "MainFrame.h"
#include "TaskBarIcon.h"
#include "Zyre.h"
#include "Node.h"
#include "Event.h"


wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT,  CMainFrame::OnExit)
wxEND_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_taskbar = new CTaskBarIcon(this);
	m_node = new CNode(this);
	SetIcon(wxICON(CLIPCLIENT));

	Bind(ZYRE_EVENT, &CMainFrame::OnZyreEvent, this);
	Bind(wxEVT_HOTKEY, &CMainFrame::OnHotkey, this);

	//Register a hotkey
	if (!RegisterHotKey(1000, wxMOD_SHIFT | wxMOD_CONTROL, 0x43))
	{
		LOG_WARN() << "Could not register hotkey" << std::endl;
	}

}

CMainFrame::~CMainFrame()
{
	delete m_taskbar;
	delete m_node;
}

void CMainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void CMainFrame::OnZyreEvent(wxThreadEvent& event)
{

	CEvent evt = event.GetPayload<CEvent>();

}

void CMainFrame::OnHotkey(wxKeyEvent& event)
{
	LOG() << "Captured hotkey" << std::endl;

}