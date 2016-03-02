// GLVTest1Doc.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "GLVTest1Doc.h"


// CGLVTest1Doc

IMPLEMENT_DYNCREATE(CGLVTest1Doc, CDocument)

CGLVTest1Doc::CGLVTest1Doc() : 
	m_pScene(NULL)
{
}

BOOL CGLVTest1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (m_pScene)
		DEL(m_pScene);
	m_pScene = new Scene::CGLScene();
	// TODO:: workaround: no viewport information available...
	m_pScene->vp.SetupViewport(640, 480);
	m_pScene->Init(NULL, ::AfxGetInstanceHandle());
	std::vector<std::unique_ptr<Scene::IItem>> items = vtest.GenerateSpheres(m_pScene->m_Shaders);
	for (auto& it : items)
	{
		m_pScene->Add(std::move(it));
	}
	m_pScene->_loaded = TRUE;
	return TRUE;
}

CGLVTest1Doc::~CGLVTest1Doc()
{
}


BEGIN_MESSAGE_MAP(CGLVTest1Doc, CDocument)
END_MESSAGE_MAP()


// CGLVTest1Doc diagnostics

#ifdef _DEBUG
void CGLVTest1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CGLVTest1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CGLVTest1Doc serialization

void CGLVTest1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CGLVTest1Doc commands
