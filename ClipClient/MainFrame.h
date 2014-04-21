#pragma once

#include "Logger.h"

class CTaskBarIcon;
class CNode;
class CClipboardManager;

class CMainFrame :	public wxFrame
{
public:
	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~CMainFrame();
	
private:
	void OnExit(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);
	void OnZyreEvent(wxThreadEvent& event);
	void OnHotkey(wxKeyEvent& event);
	bool RegisterHotKey(int hotkeyId, int modifiers, int keycode);

	CTaskBarIcon* m_taskbar;
	CNode* m_node;
	CClipboardManager* m_clipboard;
	wxTextCtrl    *m_text;

	DECLARE_EVENT_TABLE();
	DECLARE_LOGGER();
};
