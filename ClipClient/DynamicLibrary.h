#pragma once

#include <string>

class DynamicLibrary
{
public:
	static DynamicLibrary* load(const std::wstring& path, std::wstring &error);
	~DynamicLibrary(void);
	void* getSymbol(const std::string& name);

private:
	DynamicLibrary();
	DynamicLibrary(void* handle);
	DynamicLibrary(const DynamicLibrary &);
	 
	void* m_handle;
};

