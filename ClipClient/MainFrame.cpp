#include "StdAfx.h"
#include "MainFrame.h"
#include "TaskBarIcon.h"
#include "Zyre.h"
#include "Node.h"
#include "Event.h"
#include "ClipboardManager.h"


wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT,  CMainFrame::OnExit)
wxEND_EVENT_TABLE()

Logger CMainFrame::m_logger = Logger(boost::log::keywords::channel = "CMainFrame");
//DEFINE_LOGGER(CMainFrame);

CMainFrame::CMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	
	m_taskbar = new CTaskBarIcon(this);
	m_node = new CNode(this);
	m_clipboard = new CClipboardManager();

	SetIcon(wxICON(CLIPCLIENT));

	Bind(ZYRE_EVENT, &CMainFrame::OnZyreEvent, this);
	Bind(wxEVT_HOTKEY, &CMainFrame::OnHotkey, this);

	//Register a hotkey
	if (!RegisterHotKey(1000, wxMOD_SHIFT | wxMOD_CONTROL, 0x43))
	{
		LOG_WARN() << "Could not register hotkey";
	}

}

CMainFrame::~CMainFrame()
{
	delete m_taskbar;
	delete m_node;
	delete m_clipboard;
}

void CMainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void CMainFrame::OnZyreEvent(wxThreadEvent& event)
{

	CEvent evt = event.GetPayload<CEvent>();

	if (evt.getType() == EVT_SHOUT)
	{
		std::string text = evt.getContent().getString();
		text += "Copied";
		m_clipboard->setClipboard(text);
	}

}

void CMainFrame::OnHotkey(wxKeyEvent& event)
{
	LOG() << "Captured hotkey";

	//m_clipboard->simulateCopy();
	std::string text;

	wxTextDataObject* obj = m_clipboard->getText();

	if (obj)
	{
		LOG() << "Text in clipboard: " << obj->GetText();
		text = obj->GetText();
	}

	m_node->shout("ALL", text);
	delete obj;
}

/**
 \fn	void CMainFrame::RegisterHotKey(int hotkeyId, int modifiers, int keycode)

 \brief	Registers the hot key.
		 Adapted from MSW implementation of RegisterHotKey to not repeat hotkeyevents.

 \param	hotkeyId 	Identifier for the hotkey.
 \param	modifiers	The modifiers.
 \param	keycode  	The keycode.
 */
bool CMainFrame::RegisterHotKey(int hotkeyId, int modifiers, int keycode)
{
	UINT win_modifiers=0;
	if ( modifiers & wxMOD_ALT )
		win_modifiers |= MOD_ALT;
	if ( modifiers & wxMOD_SHIFT )
		win_modifiers |= MOD_SHIFT;
	if ( modifiers & wxMOD_CONTROL )
		win_modifiers |= MOD_CONTROL;
	if ( modifiers & wxMOD_WIN )
		win_modifiers |= MOD_WIN;
#ifdef MOD_NOREPEAT
	win_modifiers |= MOD_NOREPEAT;
#endif

	if ( !::RegisterHotKey(GetHwnd(), hotkeyId, win_modifiers, keycode) )
	{
		wxLogLastError(wxT("RegisterHotKey"));

		return false;
	}

	return true;
}