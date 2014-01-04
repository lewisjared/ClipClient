#include "StdAfx.h"
#include "TaskBarIcon.h"

DEFINE_LOGGER(CTaskBarIcon);

CTaskBarIcon::CTaskBarIcon(wxWindow* parent)
	:wxTaskBarIcon(), m_parent(parent)
{
	Bind(wxEVT_TASKBAR_LEFT_DCLICK, &CTaskBarIcon::OnDoubleClick, this);

	SetIcon(wxICON(CLIPCLIENT));
}

CTaskBarIcon::~CTaskBarIcon()
{
	RemoveIcon();
}

void CTaskBarIcon::OnDoubleClick(wxTaskBarIconEvent& )
{
	if (m_parent->IsShown())
	{
		LOG() << "TaskBar double clicked, Hiding main window";
		m_parent->Show(false);
	} else {
		LOG() << "TaskBar double clicked, Showing main window";
		m_parent->Show(true);
	}	
}
