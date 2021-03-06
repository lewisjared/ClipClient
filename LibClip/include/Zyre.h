/**
 \file	Zyre.h

 \brief	Defines some common constants for the Zyre C++ interface
 */

#ifndef ZYRE_H
#define ZYRE_H

#include <wx/event.h>

#define REAP_INTERVAL 1000
#define EXPIRED_TIME 2000
#define EVAISIVE_TIME 1000

#include <vector>
#include <string>

typedef std::vector<std::string> TStringVector;

wxDECLARE_EVENT(ZYRE_EVENT, wxThreadEvent);

#endif