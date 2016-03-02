#pragma once
#ifdef WIN32
#include <windows.h>
#endif
#include "Logger.h"
namespace Scene
{

class CWinEditor
{	
	HWND m_hWnd;
	HCURSOR m_hCursor;
	HINSTANCE m_hInstance;
public:
	BOOL CreateAndShow(int x, int y, int width, int height, HWND hWndParent, HINSTANCE hInstance);
	void Destroy();
	CWinEditor(void);
	virtual ~CWinEditor(void);
};

}