/**
 \file	ClipboardItem.h

 \brief	Declares the clipboard item class.
 */

#pragma once

#include "Data.h"
typedef UINT ClipFormat_t;

/**
 \class	ClipboardItem

 \brief	Clipboard item.

 Defines an item on the clipboard

 \date	7/2/2013
 */

class ClipboardItem
{
public:
	ClipboardItem();
	ClipboardItem(ClipFormat_t format);
	~ClipboardItem(void);

	/**
	 \fn	static ClipboardItem* ClipboardItem::create();
	
	 \brief	Creates a new clipboard object from the current state of the clipboard
	
	 \date	7/2/2013
	
	 \return	null if it fails, else a ClipboardItem
	 */

	static ClipboardItem* create();

	/**
	 \fn	void ClipboardItem::destroy();
	
	 \brief	Destroys this object.
	 This also empties the clipboard and cleans up any allocated data
	
	 \date	7/2/2013
	 */

	void destroy();

	Data getData();

	// Access the Format
	const ClipFormat_t& getFormat(void) const	{ return(m_format);		};

	// Access the Uid
	DWORD getUid(void) const	{ return(m_uid);	};

	
private:
	ClipFormat_t m_format;
	DWORD m_uid;
	Data m_data;

	void readClipboard();
	void readUID();
};

