
// testMSVC2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "testMSVC2.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "testMSVC2Doc.h"
#include "testMSVC2View.h"
/////////////////////////////////////
#include "GLFrame.h"
#include "GLView.h"
#include "GLDoc.h"
#include "GLVTest1Doc.h"
#include "Mesh.h"

/////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestMSVC2App

BEGIN_MESSAGE_MAP(CtestMSVC2App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CtestMSVC2App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_VIEW_GL32773, &CtestMSVC2App::OnViewGl32773)
	ON_COMMAND(ID_BUILD_BUILD, &CtestMSVC2App::OnBuildBuild)
	ON_UPDATE_COMMAND_UI(ID_BUILD_BUILD, &CtestMSVC2App::OnUpdateBuildBuild)
END_MESSAGE_MAP()


// CtestMSVC2App construction

CtestMSVC2App::CtestMSVC2App() : m_pSceneDoc(NULL)
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CtestMSVC2App object

CtestMSVC2App theApp;


// CtestMSVC2App initialization

BOOL CtestMSVC2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_VSHADER_TYPE,
		RUNTIME_CLASS(CtestMSVC2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CtestMSVC2View));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_testMSVC2TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
	/////////////////////////////////////////////////////////	
	pDocTemplate = new CMultiDocTemplate(IDR_FSHADER_TYPE,
		RUNTIME_CLASS(CtestMSVC2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CtestMSVC2View));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_testMSVC2TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
	
	m_pGLTemplate = new CMultiDocTemplate(IDR_GLVIEWTYPE,
		RUNTIME_CLASS(CGLDoc),
		RUNTIME_CLASS(CGLFrame), // custom MDI child frame
		RUNTIME_CLASS(CGLView));
	if (!m_pGLTemplate)
		return FALSE;
	m_pGLTemplate->SetContainerInfo(IDR_testMSVC2TYPE_CNTR_IP);
	AddDocTemplate(m_pGLTemplate);

	m_pVTest1Template = new CMultiDocTemplate(IDR_PDTESTTYPE,
		RUNTIME_CLASS(CGLVTest1Doc),
		RUNTIME_CLASS(CGLFrame), // custom MDI child frame
		RUNTIME_CLASS(CGLView));
	if (!m_pVTest1Template)
		return FALSE;
	m_pVTest1Template->SetContainerInfo(IDR_testMSVC2TYPE_CNTR_IP);
	AddDocTemplate(m_pVTest1Template);
	/////////////////////////////////////////////////////////
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	/////////////////////////////////////////////////////////
	// Don't open any document window:
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	//POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	//CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
	// TODO:: later it should respond to *.scene files (update IDR_testMSVC2TYPE_CNTR_IP accordingly!!!)
	OpenDocumentFile(".\\Objects\\multilayer.mesh");
	//CWinAppEx::OnFileNew();
	//CDocTemplate* pTemplate = m_pGLTemplate;
	//ASSERT(pTemplate != NULL);
	//ASSERT_KINDOF(CDocTemplate, pTemplate);
	//pTemplate->OpenDocumentFile(lpszFileName);

	//POSITION docPos = pTemplate->GetFirstDocPosition();
	//CDocument * pDoc = pTemplate->GetNextDoc(docPos);
	//ASSERT_KINDOF(CGLDoc, pDoc);
	//CGLDoc * pGLDoc = (CGLDoc*)pDoc;
	//pGLDoc->m_pScene = new Scene::CGLScene();
	//pGLDoc->m_pScene->Init(NULL, ::AfxGetInstanceHandle());
	//const char * fnames[] = {};
	//pGLDoc->m_pScene->LoadMeshes(sizeof(fnames)/sizeof(fnames[0]), fnames);
	//CSceneBrowser * pSceneBrowser = (CSceneBrowser*)pMainFrame->GetPane(ID_SCENE_BROWSERWND);	
	//if (pSceneBrowser)
	//	pSceneBrowser->OnSceneOpen(pGLDoc->m_pScene);
	/////////////////////////////////////////////////////////
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CtestMSVC2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CtestMSVC2App customization load/save methods

void CtestMSVC2App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
    bNameValid = strName.LoadString(IDS_SCENE_BROWSER);
	ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_SURFACE);
}

void CtestMSVC2App::LoadCustomState()
{
}

void CtestMSVC2App::SaveCustomState()
{
}

void CtestMSVC2App::OnViewGl32773()
{
	////ASSERT_KINDOF(CDocTemplate, m_pGLTemplate);
	////m_pGLTemplate->OpenDocumentFile(NULL);
	m_scene.Create("OpenGL", 640, 480, 24, FALSE, FALSE, ::AfxGetInstanceHandle());
	m_scene.Init();
	m_scene.MessageLoop();
}

void CtestMSVC2App::OnBuildBuild()
{
	m_pSceneDoc->m_pScene->ProgramReload();
    for (Scene::Meshes::iterator it = m_pSceneDoc->m_pScene->_meshes.begin();it!=m_pSceneDoc->m_pScene->_meshes.end();it++)
    	it->first->SetupGLSurfaces(m_pSceneDoc->m_pScene->m_Shaders);
}

CDocument* CtestMSVC2App::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CDocument* pDoc = CWinAppEx::OpenDocumentFile(lpszFileName);
//    ((CMainFrame*)m_pMainWnd)->GetMDITabs().SetActiveTabBoldFont(FALSE);
	if (pDoc->IsKindOf(RUNTIME_CLASS(CGLDoc)))
		m_pSceneDoc = (CGLDoc*)pDoc;
	return pDoc;
}

void CtestMSVC2App::OnUpdateBuildBuild(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pSceneDoc != NULL && m_pSceneDoc->m_pScene != NULL);
}
