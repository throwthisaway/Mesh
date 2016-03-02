// GLEditorView.h : interface of the CGLEditorView class
//


#pragma once


class CGLEditorView : public CView
{
protected: // create from serialization only
	CGLEditorView();
	DECLARE_DYNCREATE(CGLEditorView)

// Attributes
public:
	CGLEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGLEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GLEditorView.cpp
inline CGLEditorDoc* CGLEditorView::GetDocument() const
   { return reinterpret_cast<CGLEditorDoc*>(m_pDocument); }
#endif

