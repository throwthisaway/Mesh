
// testMSVC2.h : main header file for the testMSVC2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "GLWindowHandler.h"
#include "GLDoc.h"
#include "VTest1.h"

// CtestMSVC2App:
// See testMSVC2.cpp for the implementation of this class
//

class CtestMSVC2App : public CWinAppEx
{
	CDocTemplate* m_pGLTemplate, *m_pVTest1Template;
	CGLWindowHandler m_scene;

public:
	CGLDoc * m_pSceneDoc;
	CtestMSVC2App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewGl32773();
	afx_msg void OnBuildBuild();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	void GenerateSpheres(void);
	afx_msg void OnUpdateBuildBuild(CCmdUI *pCmdUI);
};

extern CtestMSVC2App theApp;
