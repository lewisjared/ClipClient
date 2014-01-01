#include "StdAfx.h"
#include "MainFrame.h"
#include "TaskBarIcon.h"
#include "Node.h"


wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT,  CMainFrame::OnExit)
wxEND_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	m_taskbar = new CTaskBarIcon(this);
	m_node = new CNode();
	SetIcon(wxICON(CLIPCLIENT));
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