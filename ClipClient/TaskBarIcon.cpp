#include "StdAfx.h"
#include "TaskBarIcon.h"


CTaskBarIcon::CTaskBarIcon(wxWindow* parent)
	:wxTaskBarIcon(), m_parent(parent)
{
	Bind(wxEVT_TASKBAR_LEFT_DCLICK, &CTaskBarIcon::OnDoubleClick, this);

	SetIcon(wxICON(CLIPCLIENT));
}

void CTaskBarIcon::OnDoubleClick(wxTaskBarIconEvent& )
{
	m_parent->Show(!m_parent->IsShown());
}
