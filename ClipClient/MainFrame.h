#pragma once

class CMainFrame :	public wxFrame
{
public:
	CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	
private:
	void OnExit(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();
};
