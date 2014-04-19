// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClipClient.h"
#include "MainFrame.h"

#include "VLD.h"

#include "Logger.h"

using namespace ss;

#include <cstring>
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
	bool validCMDLine = ParseCommandLine();

	if (validCMDLine)
	{

		m_mainFrame = new CMainFrame("Test",wxDefaultPosition, wxDefaultSize);
		m_mainFrame->Show();
		std::string filename = setting<std::string>(TTEXT("test"));

		return true;
	}

	return false;
}

int CClipClientApp::OnExit()
{
	LOG() << "Exiting application";

	return 1;
}

bool CClipClientApp::ParseCommandLine()
{
	bool isValidCMDLine = false;

	static const wxCmdLineEntryDesc cmdLineDesc[] = 
	{
		{ wxCMD_LINE_OPTION, "c", "config-file", "Location of the configuration file", wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_OPTIONAL},
		{ wxCMD_LINE_NONE}
	};

	wxCmdLineParser parser(cmdLineDesc, wxApp::argc, wxApp::argv);
	switch (parser.Parse())
	{
	case -1:
		LOG_WARN() << "Help was given via commandline";
		break;

	case 0:
		LOG() << "Commandline parsing complete";
		isValidCMDLine = true;
		break;

	default:
		LOG_ERR() << "Syntax error in the commandline";
		break;
	}

	//Extract the data from the parser and 
	if (isValidCMDLine)
	{
		wxString str = "config.ini";
		parser.Found("config-file",&str);
		ss::def_cfg().add_storage(TTEXT(""), new file_storage(str.ToStdString()));
		setting(TTEXT("test")) = "hello";
	}

	//Return if the command line was parsed correctly
	return isValidCMDLine;
}