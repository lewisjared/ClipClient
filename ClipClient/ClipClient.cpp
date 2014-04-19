// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClipClient.h"
#include "MainFrame.h"

#include "VLD.h"

#include "Logger.h"

#include "Settings.h"
#include "wx/cmdline.h"


void ss::init_settings( ) {
	//def_cfg().add_storage(TTEXT("user"), new file_storage("user.ini"));

	// defaults
	setting(TTEXT("max_size")) = 10;
	setting(TTEXT("log_file")) = "clipclient.log";
}


IMPLEMENT_APP(CClipClientApp)
DEFINE_LOGGER(CClipClientApp);

CClipClientApp::CClipClientApp()
{
#ifndef NDEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout); //redirect cout to the console
#endif

	initLogging("sample.log");
}

CClipClientApp::~CClipClientApp()
{
#ifndef NDEBUG
	FreeConsole();
#endif
}

bool CClipClientApp::OnInit()
{
	LOG_INFO() << "Initialising Application";

	if ( !wxApp::OnInit() )
		return false;

	m_mainFrame = new CMainFrame("Test",wxDefaultPosition, wxDefaultSize);
	m_mainFrame->Show();

	return true;
}


void CClipClientApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	wxApp::OnInitCmdLine(parser);

	//Add desired command line options
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{
			wxCMD_LINE_OPTION,
			"c",
			"configfile",
			"Location of the configuration file",
			wxCMD_LINE_VAL_STRING,
			wxCMD_LINE_PARAM_OPTIONAL
		},
		wxCMD_LINE_DESC_END
	};
	parser.SetDesc(cmdLineDesc);
}

bool CClipClientApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxApp::OnCmdLineParsed(parser);
	wxString tempStr;
	
	//Set the config file location
	// Default location is config.ini
	tempStr = "config.ini";
	parser.Found("configfile", &tempStr);
	ss::def_cfg().add_storage(TTEXT(""), new ss::file_storage(tempStr.ToStdString()));

	//Extract config params into settings
	ss::setting(TTEXT("test")) = "Hello";

	return true;
}

int CClipClientApp::OnExit()
{
	LOG() << "Exiting application";

	return 1;
}