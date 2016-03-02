
// testMSVC2View.h : interface of the CtestMSVC2View class
//


#pragma once

class CtestMSVC2CntrItem;

class CtestMSVC2View : public CRichEditView
{
	CFont m_fnt;
protected: // create from serialization only
	CtestMSVC2View();
	DECLARE_DYNCREATE(CtestMSVC2View)

// Attributes
public:
	CtestMSVC2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CtestMSVC2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in testMSVC2View.cpp
inline CtestMSVC2Doc* CtestMSVC2View::GetDocument() const
   { return reinterpret_cast<CtestMSVC2Doc*>(m_pDocument); }
#endif

