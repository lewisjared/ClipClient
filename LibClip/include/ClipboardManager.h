#pragma once

#include <map>
#include "ClipboardItem.h"

typedef std::map<ClipFormat_t,ClipboardItem> ClipboardMap;

class CClipboardManager
{
public:
	CClipboardManager(void);
	
	void checkClipboard();
	void empty();
	DWORD getSequenceNum();
	ClipboardMap getData() {return m_data;};

private:
	ClipboardMap m_data;
};

