#pragma once

#include <string>

#include "StdAfx.h"

/**
 \fn	void GetErrorString(std::wstring &error)

 \brief	Gets the error string for the last system error

 \param [in,out]	error	The errorString.
 */

void GetErrorString(std::wstring &error)
{
#ifdef WIN32
	DWORD errorCode = GetLastError();
	WCHAR errorMessage[100];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM , 
		0, errorCode,0,errorMessage, MAX_PATH,0);
	error = errorMessage;
#endif // WIN32
}
