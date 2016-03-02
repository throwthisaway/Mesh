#pragma once
#include "GLWindowHandler.h"
#include "GLScene.h"
// CGLView view
class CGLView : public CView
{
	DECLARE_DYNCREATE(CGLView)
	
	// For threading...
	HANDLE m_hWaitObj;
	// ...For threading
	CGLWindowHandler m_glWindow;
	BOOL m_bCreated;
protected:
	CGLView();           // protected constructor used by dynamic creation
	virtual ~CGLView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL DestroyWindow();
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


