// GLFrame.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "GLFrame.h"


// CGLFrame

//IMPLEMENT_DYNAMIC(CGLFrame, CMDIChildWndEx)
IMPLEMENT_DYNCREATE(CGLFrame, CMDIChildWndEx)

CGLFrame::CGLFrame()
{
}

CGLFrame::~CGLFrame()
{
}


BEGIN_MESSAGE_MAP(CGLFrame, CMDIChildWndEx)
END_MESSAGE_MAP()



// CGLFrame message handlers



BOOL CGLFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	EnableDocking(CBRS_ALIGN_ANY);
	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}
