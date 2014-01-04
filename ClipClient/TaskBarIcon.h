#pragma once

#include "Logger.h"
#include <wx/taskbar.h>

class CTaskBarIcon :
	public wxTaskBarIcon
{
public:
	CTaskBarIcon(wxWindow* parent);
	~CTaskBarIcon();

private:
	void OnDoubleClick(wxTaskBarIconEvent& event);

	wxWindow* m_parent;

	DECLARE_LOGGER();
};

