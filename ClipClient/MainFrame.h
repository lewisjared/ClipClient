#pragma once

class CTaskBarIcon;
class CNode;

class CMainFrame :	public wxFrame
{
public:
	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~CMainFrame();
	
private:
	void OnExit(wxCommandEvent& event);

	CTaskBarIcon* m_taskbar;
	CNode* m_node;

	DECLARE_EVENT_TABLE();
};
