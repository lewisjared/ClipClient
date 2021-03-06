#pragma once

#include "Logger.h"

class CMainFrame;

// Define a new application
class CClipClientApp : public wxApp
{
public:
	CClipClientApp();
	virtual ~CClipClientApp();

	virtual int OnExit();
	virtual bool OnInit();
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

private:
	CMainFrame* m_mainFrame;

	DECLARE_LOGGER();
};

DECLARE_APP(CClipClientApp)