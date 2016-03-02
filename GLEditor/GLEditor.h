// GLEditor.h : main header file for the GLEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGLEditorApp:
// See GLEditor.cpp for the implementation of this class
//

class CGLEditorApp : public CWinApp
{
public:
	CGLEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGLEditorApp theApp;