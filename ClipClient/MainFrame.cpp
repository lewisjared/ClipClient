#include "StdAfx.h"
#include "MainFrame.h"


wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT,  CMainFrame::OnExit)
wxEND_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
}


void CMainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}