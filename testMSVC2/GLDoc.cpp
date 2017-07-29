// GLDoc.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "GLDoc.h"
#include "SceneBrowser.h"
#include "MainFrm.h"
#include "Torus.h"

// CGLDoc

IMPLEMENT_DYNCREATE(CGLDoc, CDocument)

CGLDoc::CGLDoc() : m_pScene(NULL)
{
}

BOOL CGLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (m_pScene)
		DEL(m_pScene);
	m_pScene = new Scene::CGLScene();
	// TODO:: workaround: no viewport information available...
	m_pScene->vp.SetupViewport(640, 480);
	m_pScene->Init(NULL, ::AfxGetInstanceHandle());
	return TRUE;
}

CGLDoc::~CGLDoc()
{
	DEL(m_pScene);
}


BEGIN_MESSAGE_MAP(CGLDoc, CDocument)
END_MESSAGE_MAP()


// CGLDoc diagnostics

#ifdef _DEBUG
void CGLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CGLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CGLDoc serialization

void CGLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		CString fName = ar.GetFile()->GetFilePath();
		if (m_pScene)
			DEL(m_pScene);
		m_pScene = new Scene::CGLScene();
		// TODO:: workaround: no viewport information available...
		m_pScene->vp.SetupViewport(640, 480);
		m_pScene->Init(NULL, ::AfxGetInstanceHandle());
		//const char * fnames[] = /*{"C:\\Content\\Objects\\disc.mesh", "C:\\Content\\Objects\\ball.mesh", "C:\\Content\\Objects\\ball.mesh"};*/{fName, ".\\Objects\\64THUNDE_triple.mesh"};
        const char * fnames[] = { "F:\\install\\3D\\Objects\\checkerboard.mesh"/*"F:\\install\\3D\\Robot\\robot_only_fixed_centered1.mesh",*//* "f:\\install\\3D\\Objects\\64THUNDE_triple.mesh",
			"f:\\install\\3D\\Objects\\layer_test.mesh"*/
			/*"f:\\install\\3D\\Objects\\checkerboard.mesh"*/
			/*, ".\\Objects\\box.mesh", ".\\Objects\\64THUNDE_triple.mesh"*/};
		const Vector<float> pos[] = {/*Vector<float>(0.f, -45.f, 0.f),*/ Vector<float>(0.f, 0.f, 0.f), Vector<float>(-4.f, 0.f, -4.f),
			Vector<float>(4.f, 4.f, 4.f) };
		m_pScene->LoadMeshes(sizeof(fnames)/sizeof(fnames[0]), fnames, pos);
		/*Scene::CTorus * torus = new Scene::CTorus();
		torus->Setup(0.5f, 1.5f, 10, 20);
		m_pScene->Add(Vector<float>(0.f, 0.f, 0.f), torus);*/
		// Signal pSceneBrowser of file opening
		// TODO:: replace with message routing
		CWinApp* pWinApp = ::AfxGetApp();
		CMainFrame* pMainFrame = (CMainFrame*)pWinApp->m_pMainWnd;
		CSceneBrowser * pSceneBrowser = (CSceneBrowser*)pMainFrame->GetPane(ID_SCENE_BROWSERWND);	
		if (pSceneBrowser)
			pSceneBrowser->OnSceneOpen(m_pScene);
		//
	}
}
#endif


// CGLDoc commands
