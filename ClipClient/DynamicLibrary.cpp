#include "StdAfx.h"
#include "DynamicLibrary.h"
#include "utils.h"


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

DynamicLibrary* DynamicLibrary::load(const std::wstring& path, std::wstring &error)
{
	DynamicLibrary* library = new DynamicLibrary;
#ifdef WIN32
	library->m_handle = LoadLibrary(path.c_str());
#endif // WINDOWS
	if (library->m_handle == NULL)
	{
		GetErrorString(error);
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