#pragma once

#include "LightSourceParameters.h"
#include "GLScene.h"

// CLightsToolBar
class CLightsToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CLightsWnd
class CLightsWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CLightsWnd)

	int _lightCount;
	CLightsToolBar _toolBar;
public:
	CLightsWnd();
	virtual ~CLightsWnd();
	void AddLight(LightSource& light);
	void InitLightsList(Scene::CGLScene * pGLScene);
	void Clear();
protected:
	CMFCPropertyGridCtrl m_wndLightsList;
	static void OnUpdateAmbient(DWORD_PTR dwData, const COLORREF color);
	static void OnUpdateDiffuse(DWORD_PTR dwData, const COLORREF color);
	static void OnUpdateSpecular(DWORD_PTR dwData, const COLORREF color);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAdd();
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
protected:
	void AdjustLayout(void);

};


