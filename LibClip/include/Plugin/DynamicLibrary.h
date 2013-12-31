#pragma once

#include <string>

class DynamicLibrary
{
public:
	static DynamicLibrary* load(const std::wstring& path);
	~DynamicLibrary(void);
	void* getSymbol(const std::string& name);

private:
	DynamicLibrary();
	DynamicLibrary(void* handle);
	DynamicLibrary(const DynamicLibrary &);

	static void LogErrorString();
	 
	void* m_handle;
};

