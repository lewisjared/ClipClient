#include "windows.h"
#include "Clipboard.h"


CClipboardManager::CClipboardManager(void)
{
}

void CClipboardManager::checkClipboard()
{
	// Check read the linked list of formats and create a new Clipboard item for each
	UINT format = EnumClipboardFormats(0);

	while (format)
	{
		m_data[format] = ClipboardItem(format);
		format = EnumClipboardFormats(format);
	}
}

void CClipboardManager::empty()
{
	EmptyClipboard();
}

DWORD CClipboardManager::getSequenceNum()
{
	return GetClipboardSequenceNumber();
}