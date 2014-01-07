#include "windows.h"
#include "wx/dataobj.h"
#include "wx/clipbrd.h"
#include "ClipboardManager.h"

DEFINE_LOGGER(CClipboardManager);


CClipboardManager::CClipboardManager(void)
{
}

wxTextDataObject* CClipboardManager::getText()
{
	wxTextDataObject* data = NULL;

	if (wxClipboard::Get()->Open())
	{
		if (wxClipboard::Get()->IsSupported(wxDF_TEXT))
		{
			data = new wxTextDataObject;
			wxClipboard::Get()->GetData(*data);
		}
		wxClipboard::Get()->Close();
	}
	return data;
}

void CClipboardManager::setClipboard(const std::string &text)
{
	if (wxClipboard::Get()->Open())
	{
		wxTextDataObject* data = new wxTextDataObject(text);
		wxClipboard::Get()->SetData(data);
		wxClipboard::Get()->Close();
	}
}

void CClipboardManager::simulateCopy() const
{
	//simulateKeyPress(VK_LCONTROL);
	simulateKeyPress('C');
	simulateKeyRelease('C');
	//simulateKeyRelease(VK_LCONTROL);
}

void CClipboardManager::simulateCut() const
{
	simulateKeyPress(VK_LCONTROL);
	simulateKeyPress(0x58);
	simulateKeyRelease(0x58);
	simulateKeyRelease(VK_LCONTROL);
}

void CClipboardManager::simulatePaste() const
{
	simulateKeyPress(VK_LCONTROL);
	simulateKeyPress(0x56);
	simulateKeyRelease(0x56);
	simulateKeyRelease(VK_LCONTROL);
}

void CClipboardManager::simulateKeyPress(uint16_t vk) const
{
	INPUT key = {0};
	key.type = INPUT_KEYBOARD;
	key.ki.wVk = vk; 
	SendInput(1, &key, sizeof(INPUT));
}

void CClipboardManager::simulateKeyRelease(uint16_t vk) const
{
	INPUT key;
	key.type = INPUT_KEYBOARD;
	key.ki.wScan = 0; // hardware scan code for key
	key.ki.time = 0;
	key.ki.dwExtraInfo = 0;
	key.ki.wVk = vk; 
	key.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &key, sizeof(INPUT));
}