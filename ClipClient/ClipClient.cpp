// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Settings.h"
#include "Logger.h"
#include "ClipClient.h"
#include "MainFrame.h"

#include "VLD.h"


void ss::init_settings( ) {
	def_cfg().add_storage(TTEXT("user"), new file_storage("user.ini"));

	// defaults
	setting(TTEXT("max_size")) = 10;
}


IMPLEMENT_APP(CClipClientApp)

CClipClientApp::CClipClientApp()
{
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

	return 1;
}