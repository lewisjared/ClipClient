#pragma once

#include <cstdint>

#include "Logger.h"

class wxTextDataObject;

/**
 \class	CClipboardManager

 \brief	Manager for the clipboard
 */
class CClipboardManager
{
public:
	CClipboardManager(void);

	/**
	 \fn	wxTextDataObject* CClipboardManager::getText();
	
	 \brief	Gets the text currently in the clipboard
	
	 \return	null if it fails, else the text.
	 */
	wxTextDataObject* getText();

	/**
	 \fn	void CClipboardManager::setClipboard(const std::string &text);
	
	 \brief	Sets the clipboard data to contain text
	
	 \param	text	The text.
	 */
	void setClipboard(const std::string &text);

	/**
	 \fn	void CClipboardManager::simulateCopy() const;
	
	 \brief	Simulate a Ctrl+C key event that is forwarded to the operating system
	 */
	void simulateCopy() const;

	/**
	 \fn	void CClipboardManager::simulateCut() const;
	
	 \brief	Simulate a Ctrl+X key event that is forwarded to the operating system
	 */
	void simulateCut() const;

	/**
	 \fn	void CClipboardManager::simulatePaste() const;
	
	 \brief	Simulate a Ctrl+V key event that is forwarded to the operating system
	 */
	void simulatePaste() const;
private:
	void simulateKeyPress(uint16_t vk) const;
	void simulateKeyRelease(uint16_t vk) const;
	DECLARE_LOGGER();
};

