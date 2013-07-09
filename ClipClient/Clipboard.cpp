#include "stdafx.h"
#include "Clipboard.h"


Clipboard::Clipboard(void)
{
}

void Clipboard::checkClipboard()
{
	// Check read the linked list of formats and create a new Clipboard item for each
	UINT format = EnumClipboardFormats(0);

	while (format)
	{
		m_data[format] = ClipboardItem(format);
		format = EnumClipboardFormats(format);
	}
}

void Clipboard::empty()
{
	EmptyClipboard();
}

DWORD Clipboard::getSequenceNum()
{
	return GetClipboardSequenceNumber();
}