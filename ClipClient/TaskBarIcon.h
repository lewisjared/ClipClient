#pragma once

#include <wx/taskbar.h>

class CTaskBarIcon :
	public wxTaskBarIcon
{
public:
	CTaskBarIcon(wxWindow* parent);

private:
	void OnDoubleClick(wxTaskBarIconEvent& event);

	wxWindow* m_parent;
};

