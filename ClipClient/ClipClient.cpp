// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClipClient.h"
#include "MainFrame.h"

#include "VLD.h"

#include "Logger.h"

using namespace ss;


void ss::init_settings( ) {
	def_cfg().add_storage(TTEXT("user"), new file_storage("user.ini"));

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

int CClipClientApp::OnExit()
{
	LOG() << "Exiting application";



	return 1;
}