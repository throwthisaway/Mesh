// GLView.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "GLView.h"
#include "GLDoc.h"
#include "AppConstants.h"
#include "SceneBrowser.h"
#include "MainFrm.h"

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )	

// CGLView


IMPLEMENT_DYNCREATE(CGLView, CView)

CGLView::CGLView() : m_bCreated(FALSE)
{

}

CGLView::~CGLView()
{

}

BEGIN_MESSAGE_MAP(CGLView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGLView drawing
float y = 0.;
void CGLView::OnDraw(CDC* pDC)
{
	/*CGLDoc* pDoc = (CGLDoc*)GetDocument();
	if (pDoc->m_pScene)
		pDoc->*/
	CGLDoc* pDoc = (CGLDoc*)GetDocument();
	if (pDoc->m_pScene && pDoc->m_pScene->_loaded)
	{
		m_glWindow.AttachRenderingContext();
		pDoc->m_pScene->Draw(pDC->GetSafeHdc());
	}
	//CDocument* pDoc = GetDocument();
	//m_scene.Draw(pDC->GetSafeHdc());

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	//glLoadIdentity();						// Reset The View
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glBegin(GL_TRIANGLES);						// Drawing Using Triangles
	//	glVertex3f( 3.0f, 3.0f, 3.0f);				// Top
	//	glVertex3f(2.9f,2.9f, 3.0f);				// Bottom Left
	//	glVertex3f( 3.1f,2.9f, 3.0f);				// Bottom Right
	//glEnd();
	//glTranslatef(0.0f,0.0f,-10.0f);					// Move Left 1.5 Units And Into The Screen 6.0
	//
	//glBegin(GL_TRIANGLES);						// Drawing Using Triangles
	//	glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
	//	glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
	//	glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
	//glEnd();	
	//	glTranslatef(3.0f,0.0f,0.0f);	
	//		glBegin(GL_QUADS);						// Draw A Quad
	//	glVertex3f(-1.0f, 1.0f, 0.0f);				// Top Left
	//	glVertex3f( 1.0f, 1.0f, 0.0f);				// Top Right
	//	glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
	//	glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
	//glEnd();
	//SwapBuffers(pDC->GetSafeHdc());
}


// CGLView diagnostics

#ifdef _DEBUG
void CGLView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGLView message handlers

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDC *pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == NULL)
	{
		return -1;
	}
	HINSTANCE hInst = ::AfxGetInstanceHandle();
	//::SetWindowText(hWnd, lpCreateStruct->lpszName);
	CString title;
	ASSERT(title.LoadString(IDS_GL_WINDOW_TITLE));
	if (m_glWindow.Create(title.GetBuffer(), CAppConstants::DefWindowWidth, CAppConstants::DefWindowHeight, CAppConstants::DefBpp, FALSE, FALSE, hInst, hWnd))
	{
		return -1;
	}
	m_glWindow.Init();

	//m_hWaitObj = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	//ASSERT(m_hWaitObj);
	//m_bCreated = TRUE;
	//return m_glWindow.CreateThread(m_pScene, m_hWaitObj);
	return 0;
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	//m_glWindow.Resize(cx, cy);
	CGLDoc* pDoc = (CGLDoc*)GetDocument();
	if (pDoc->m_pScene)
		pDoc->m_pScene->vp.SetupViewport(cx, cy);
}

BOOL CGLView::DestroyWindow()
{
	//m_scene.Cleanup();
	CGLDoc* pDoc = (CGLDoc*)GetDocument();
	if (pDoc->m_pScene)
	{
		DEL(pDoc->m_pScene);
	}
	if (m_bCreated)
	{
		
		::WaitForSingleObject(m_hWaitObj, CAppConstants::WaitForHandleReleaseObjects);
		m_glWindow.Kill(GetSafeHwnd());
	}
	::CloseHandle(m_hWaitObj);
	return CView::DestroyWindow();
}

BOOL CGLView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_bCreated)
	{
		::EnterCriticalSection(&m_glWindow.m_criticalSection);
		m_glWindow.m_stMsg.hwnd = GetSafeHwnd();
		m_glWindow.m_stMsg.message = message;
		m_glWindow.m_stMsg.wParam = wParam;
		m_glWindow.m_stMsg.lParam = lParam;
		m_glWindow.m_bHasMsgToProcess = TRUE;
		::LeaveCriticalSection(&m_glWindow.m_criticalSection);
		::WaitForSingleObject(m_hWaitObj, CAppConstants::DefWaitForHandle);
		if (m_glWindow.m_bHandled)
		{
			*pResult = m_glWindow.m_dwResult;
			return TRUE;
		}
	}
	CGLDoc* pDoc = (CGLDoc*)GetDocument();
	if (pDoc && pDoc->m_pScene)
	{
		if (!(*pResult = pDoc->m_pScene->WndProc(GetSafeHwnd(), message, wParam, lParam)))
		{
			this->Invalidate(0);
			return TRUE;
		}
	}
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}
