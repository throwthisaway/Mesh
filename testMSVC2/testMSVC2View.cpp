
// testMSVC2View.cpp : implementation of the CtestMSVC2View class
//

#include "stdafx.h"
#include "testMSVC2.h"

#include "testMSVC2Doc.h"
#include "CntrItem.h"
#include "testMSVC2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestMSVC2View

IMPLEMENT_DYNCREATE(CtestMSVC2View, CRichEditView)

BEGIN_MESSAGE_MAP(CtestMSVC2View, CRichEditView)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CtestMSVC2View construction/destruction

CtestMSVC2View::CtestMSVC2View()
{
	// TODO: add construction code here

}

CtestMSVC2View::~CtestMSVC2View()
{
}

BOOL CtestMSVC2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CRichEditView::PreCreateWindow(cs);
}

void CtestMSVC2View::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	::DeleteObject(m_fnt.Detach());

	LOGFONT lf;
	
	int nHeight = -MulDiv(10, GetDeviceCaps(this->GetDC()->GetSafeHdc(), LOGPIXELSY), 72);

	m_fnt.CreateFontA(nHeight, 0, 0, 0, FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEVICE_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		FF_MODERN | FIXED_PITCH,
		_T("Consolas"));

	this->SetFont(&m_fnt);

	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));
}

void CtestMSVC2View::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}


void CtestMSVC2View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CtestMSVC2View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CtestMSVC2View diagnostics

#ifdef _DEBUG
void CtestMSVC2View::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CtestMSVC2View::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CtestMSVC2Doc* CtestMSVC2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestMSVC2Doc)));
	return (CtestMSVC2Doc*)m_pDocument;
}
#endif //_DEBUG


// CtestMSVC2View message handlers
