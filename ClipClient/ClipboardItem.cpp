#include "stdafx.h"
#include <string>
#include "ClipboardItem.h"
#include "ClipClient.h"
#include "DynamicLibrary.h"
#include "PluginManager.h"

#include "ss/setting.h"

using namespace ss;


ClipboardItem::ClipboardItem()
	:m_format(-1), m_data()
{

}

ClipboardItem::ClipboardItem(ClipFormat_t format)
: m_format(format), m_data()
{
	OpenClipboard(NULL);
	readClipboard();
	readUID();
	CloseClipboard();
}



ClipboardItem::~ClipboardItem(void)
{
}

void ClipboardItem::destroy()
{
	if (!OpenClipboard(NULL))
		return;

	EmptyClipboard();

	CloseClipboard();
}

Data ClipboardItem::getData()
{
	return m_data;
}

void ClipboardItem::readClipboard()
{
	//Clipboard is already open
	//Allocate the correct size
	HGLOBAL   hglb;
	hglb = GetClipboardData(m_format);
	std::wstring str;

	if (hglb != NULL) 
    {
		void* rawData = GlobalLock(hglb);
		size_t allocatedSize = GlobalSize(hglb);

		if (allocatedSize < setting<int>(TTEXT("max_size")))
			m_data = Data(rawData, allocatedSize);

		GlobalUnlock(hglb);
	}
}

void ClipboardItem::readUID()
{
	m_uid = GetClipboardSequenceNumber();	
}
