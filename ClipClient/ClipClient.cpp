// ClipClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClipClient.h"
#include "ClipboardItem.h"
#include "Plugin/PluginManager.h"
#include "Settings.h"
#include "Node.h"
#include "Logger.h"


void ss::init_settings( ) {
	def_cfg().add_storage(TTEXT("user"), new file_storage("user.ini"));

	// defaults
	setting(TTEXT("max_size")) = 10;
}


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	wxMessageBox
		(
		"There appears to be no system tray support in your current environment. This sample may not behave as expected.",
		"Warning",
		wxOK | wxICON_EXCLAMATION
		);


	return true;
}