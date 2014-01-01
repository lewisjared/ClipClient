// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Settings.h"
#include "Logger.h"
#include "ClipClient.h"
#include "MainFrame.h"

#include "VLD.h"

using namespace ss;


void ss::init_settings( ) {
	def_cfg().add_storage(TTEXT("user"), new file_storage("user.ini"));

	// defaults
	setting(TTEXT("max_size")) = 10;
	setting(TTEXT("log_file")) = "clipclient.log";
}


IMPLEMENT_APP(CClipClientApp)

CClipClientApp::CClipClientApp()
{
	initLogging();
}

CClipClientApp::~CClipClientApp()
{

}

bool CClipClientApp::OnInit()
{
	LOG_INFO() << "Initialising Application" << std::endl;

	if ( !wxApp::OnInit() )
		return false;

	m_mainFrame = new CMainFrame("Test",wxDefaultPosition, wxDefaultSize);
	m_mainFrame->Show();

	return true;
}

int CClipClientApp::OnExit()
{
	LOG() << "Exiting application" << std::endl;

#ifdef DEBUG
	FreeConsole();
#endif

	return 1;
}

void CClipClientApp::initLogging()
{
#ifndef NDEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout); //redirect cout to the console
	Logger::getInstance().setSeverity(DEBUG_SEV);
#else
	Logger::getInstance().enableFile(setting<std::string>(TTEXT("log_file")));
#endif
}