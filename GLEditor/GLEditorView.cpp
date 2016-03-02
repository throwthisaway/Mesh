// GLEditorView.cpp : implementation of the CGLEditorView class
//

#include "stdafx.h"
#include "GLEditor.h"

#include "GLEditorDoc.h"
#include "GLEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLEditorView

IMPLEMENT_DYNCREATE(CGLEditorView, CView)

BEGIN_MESSAGE_MAP(CGLEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGLEditorView construction/destruction

CGLEditorView::CGLEditorView()
{
	// TODO: add construction code here

}

CGLEditorView::~CGLEditorView()
{
}

BOOL CGLEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLEditorView drawing

void CGLEditorView::OnDraw(CDC* /*pDC*/)
{
	CGLEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CGLEditorView printing

BOOL CGLEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLEditorView diagnostics

#ifdef _DEBUG
void CGLEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGLEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLEditorDoc* CGLEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLEditorDoc)));
	return (CGLEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLEditorView message handlers
