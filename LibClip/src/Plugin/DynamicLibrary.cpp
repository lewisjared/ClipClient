#include "Plugin/DynamicLibrary.h"
#include "Logger.h"

#ifdef WIN32
#include <windows.h>
#endif

DynamicLibrary::DynamicLibrary(void)
	: m_handle(NULL)
{
}

DynamicLibrary::DynamicLibrary(void* handle)
	: m_handle(handle)
{

}

DynamicLibrary::DynamicLibrary(const DynamicLibrary &obj)
{
	m_handle = obj.m_handle;
}


DynamicLibrary::~DynamicLibrary(void)
{
#ifdef WIN32
	FreeLibrary((HMODULE)m_handle);
#else if LINUX
	dlclose(m_handle);
#endif
}

DynamicLibrary* DynamicLibrary::load(const std::wstring& path)
{
	DynamicLibrary* library = new DynamicLibrary;
#ifdef WIN32
	library->m_handle = LoadLibrary(path.c_str());
#endif // WINDOWS
	if (library->m_handle == NULL)
	{
		LogErrorString();
		delete library;
		library = NULL;
	}

	return library;
}

void* DynamicLibrary::getSymbol(const std::string& name)
{
#ifdef WIN32
	return GetProcAddress((HMODULE)m_handle, name.c_str());
#endif // WIN32
}

void DynamicLibrary::LogErrorString()
{
#ifdef WIN32
	DWORD errorCode = GetLastError();
	WCHAR errorMessage[100];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM , 
		0, errorCode,0,errorMessage, MAX_PATH,0);
	LOG_WARN() << "Error loading library: " << errorMessage << std::endl;
#endif // WIN32
}