#pragma once
#include "ViewTree.h"
#include "GLScene.h"

// CSceneBrowser

class CSceneBrowser : public CDockablePane
{
	DECLARE_DYNAMIC(CSceneBrowser)

	CImageList m_treeViewImages;
public:
	CSceneBrowser();
	virtual ~CSceneBrowser();
	void DestroyAllItemData();
protected:
	CViewTree m_wndTree;
	void AdjustLayout(void);
	void DestroyItemData(HTREEITEM hItem);
	void DestroyAllItemData(HTREEITEM hItem);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnSceneOpen(Scene::CGLScene * pGLScene);
	void OnChangeVisualStyle(void);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg void OnSurfaceOverrideprogram();
};


