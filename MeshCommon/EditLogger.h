#pragma once
#include <windows.h>
#include "Logger.h"

namespace Log
{

class CEditLogger : public CLogger
{
	LPTSTR buffer;
	HWND m_hWnd;
public:
	CEditLogger(HWND parentHWnd);
	virtual void Write(LPCTSTR);
	virtual void Close();
	void Flush(void) const;
	virtual ~CEditLogger(void);
};

}