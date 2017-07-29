#include "StdAfx.h"
#include "GLWindowHandler.h"

static bool keys[256], active=true;
static ATOM MyRegisterClass(HINSTANCE hInstance, LPCSTR lpszWindowClass, LPCSTR lpszMenuName = NULL, HICON hIcon = NULL, HICON hIconSm = NULL);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LPCTSTR szWindowClass = _T("OpenGL");
// for testing purposes...
static GLvoid DrawGLScene(void);
Scene::CGLScene* CGLWindowHandler::m_pScene;

CGLWindowHandler::CGLWindowHandler(void) : m_fullscreen(FALSE), m_hWnd(NULL), m_hRC(NULL), m_eThreaded(E_NOTHREAD)
{
}

CGLWindowHandler::~CGLWindowHandler(void)
{
}

void CGLWindowHandler::Cleanup(void)
{
	m_fullscreen = FALSE;
	m_hWnd = NULL;
	m_hRC = NULL;
	memset(keys, 0, sizeof(keys));
	active = true;
}
int CGLWindowHandler::Create(TCHAR* title, int width, int height, int bits, BOOL threaded, BOOL fullscreen, HINSTANCE hInst,HWND hWnd)
{
	m_fullscreen = fullscreen;
	DWORD dwStyle,dwExStyle;
	GLuint PixelFormat;
	::EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&m_DMorig);
	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight=height;
		dmScreenSettings.dmPelsWidth=width;
		dmScreenSettings.dmBitsPerPel=bits;
		dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (::ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// TODO:: Log...
			::MessageBox(NULL,_T("Can't switch to fullscreen...\n	Changing to windowed mode"),_T("ERROR"),MB_OK|MB_ICONERROR);
			fullscreen=FALSE;
		}
	}
	if (!hWnd)
	{
		m_eThreaded = E_THREAD_RC_AND_WNDPROC;
		MyRegisterClass(::AfxGetInstanceHandle(), szWindowClass/*, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MESHVIEW2005))*//*,LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));*/);
		if (fullscreen)
		{
			dwStyle=WS_POPUP;
			dwExStyle=WS_EX_APPWINDOW;
			::ShowCursor(TRUE);
		}
		else
		{
			dwStyle=WS_OVERLAPPEDWINDOW;
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}
		RECT WindowRect;
		WindowRect.bottom=(long)height;
		WindowRect.right=(long)width;
		WindowRect.top=(long)0;
		WindowRect.left=(long)0;
		::AdjustWindowRectEx(&WindowRect,dwStyle,FALSE,dwExStyle);
		if (!(m_hWnd=hWnd=CreateWindowEx(dwExStyle,szWindowClass,title,WS_CLIPSIBLINGS|WS_CLIPCHILDREN|dwStyle,0,0,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,NULL,NULL,hInst,NULL)))
		{
			Kill();
			// TODO:: Log
			::MessageBox(NULL,_T("Can't create window..."),_T("ERROR"),MB_OK|MB_ICONERROR);
			return NULL;
		}
	}
	else
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_CLIPSIBLINGS|WS_CLIPCHILDREN|::GetWindowLong(hWnd, GWL_STYLE));
	}
	m_hwndForThread = hWnd;
#ifdef _WIN64
	::SetWindowLong(hWnd, GWLP_USERDATA, (LONG)this);
#else if
    ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
#endif
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW|
			PFD_SUPPORT_OPENGL|
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			bits,
			0,0,0,0,0,0,
			0,0,
			0,
			0,0,0,0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	HDC hDC = ::GetDC(hWnd);
	if(!(PixelFormat=::ChoosePixelFormat(hDC,&pfd)))
	{
		Kill();
		// TODO:: Log
		::MessageBox(NULL,_T("Cannot find a suitable pixelformat..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return -1;
	}
	if(!::SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		Kill();
		// TODO:: Log
		::MessageBox(NULL,_T("Cannot change the pixelformat..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return -1;
	}
	if (!(m_hRC=::wglCreateContext(hDC)))
	{
		Kill();
		// TODO:: Log
		::MessageBox(NULL,_T("Cannot create the GL Rendering Context..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return -1;
	}
	m_width = (GLint)width;
	m_height = (GLint)height;
	m_hDC = hDC;
	if (!threaded)
	{
		if(!::wglMakeCurrent(hDC,m_hRC))
		{
			Kill();
			// TODO:: Log
			::MessageBox(NULL,_T("Cannot activate the GL Rendering Context..."),_T("ERROR"),MB_OK|MB_ICONERROR);
			return -1;
		}

		Resize((GLint)width,(GLint)height);
	}
	if (m_hWnd)
		::ShowWindow(m_hWnd, TRUE);
	return 0;
}

void CGLWindowHandler::Kill(HWND hWnd, HINSTANCE hInst)
{
	if (m_fullscreen)
	{
		if (!::ChangeDisplaySettings(NULL,CDS_TEST))
		{
			::ChangeDisplaySettings(NULL,CDS_RESET);
			::ChangeDisplaySettings(&m_DMorig,CDS_RESET);
		}
		else
			::ChangeDisplaySettings(NULL,CDS_RESET);
		::ShowCursor(TRUE);
	}
	if (m_hRC) 
	{
		if (!::wglMakeCurrent(NULL,NULL))
		{
			Log::CLog::Write("CGLWindowHandler::Kill: Release of Device Context and Rendering Context failed...\n");
			::MessageBox(NULL,_T("Release of Device Context and Rendering Context failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		}
		if (!::wglDeleteContext(m_hRC))
		{
			Log::CLog::Write("CGLWindowHandler::Kill: Release Rendering Context failed...\n");
			::MessageBox(NULL,_T("Release Rendering Context failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		}
		m_hRC=NULL;

	}
	if (m_hWnd)
	{
		HDC hDC = ::GetDC(m_hWnd);
		if (hDC && !::ReleaseDC(m_hWnd,hDC))
		{
			Log::CLog::Write("CGLWindowHandler::Kill: Release Device Contex failed...\n");
			::MessageBox(NULL,_T("Release Device Contex failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		}

		if (m_hWnd && !::DestroyWindow(m_hWnd))
		{
			Log::CLog::Write("CGLWindowHandler::Kill: Could not release hWnd...\n");
			::MessageBox(NULL,_T("Could not release hWnd..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		}
		m_hWnd = NULL;
		if (!hInst && !UnregisterClass(szWindowClass,hInst))
		{
			Log::CLog::Write("CGLWindowHandler::Kill: Could not unregister class...\n");
			::MessageBox(NULL,_T("Could not unregister class..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		}
		hInst = NULL;
	}
}

//const float LightPosition[3] = {3.0f, 3.0f, -1.0f};
//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat mat_shininess[] = { 50.0 };

void CGLWindowHandler::Init(void)
{
	// TODO:: clarify...
	//glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	if (m_pScene)
		if (!m_pScene->Init(m_hwndForThread, ::AfxGetInstanceHandle()))
		{
			//m_glWindow.Kill(hWnd, hInst);
			//return -1;
			Log::CLog::Write("CGLWindowHandler::Init: Cannot initialize scene...\n");
		}
}

void CGLWindowHandler::Resize(GLint width, GLint height)
{
	if (m_pScene)
		m_pScene->GetGetCurrentViewPort().SetupViewport(width, height);
	//glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}
int CGLWindowHandler::AttachRenderingContext(void)
{
	if (!::wglMakeCurrent(m_hDC, m_hRC))
	{
			Kill();
			Log::CLog::Write("CGLWindowHandler::AttachRenderingContext: Cannot attach the GL Rendering Context...\n");
			::MessageBox(NULL,_T("Cannot attach the GL Rendering Context..."),_T("ERROR"),MB_OK|MB_ICONERROR);
			return -1;
	}
	return 0;
}
void CGLWindowHandler::DetachRenderingContext(void)
{
	if (!::wglMakeCurrent(0, 0))
	{
		Log::CLog::Write("CGLWindowHandler::DetachRenderingContext: Release of Device Context and Rendering Context failed...");
		::MessageBox(NULL,_T("Release of Device Context and Rendering Context failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
	}
}
WPARAM CGLWindowHandler::MessageLoop(void)
 {
	bool done=false;
	MSG msg;
	
	while (!done)
	{
		if (PeekMessage(&msg,m_hWnd,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
				done=TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (!active)
			WaitMessage();
		//if (keys[VK_F1])
		//{
		//	keys[VK_F1]=FALSE;
		//	KillGLWindow(FALSE);
		//	m_fullscreen=!m_fullscreen;
		//	if (Create(title.GetBuffer(), CAppConstants::DefWindowWidth, CAppConstants::DefWindowHeight, CAppConstants::DefBpp, m_fullscreen, hInst, hWnd, NULL))
		//	done = TRUE;
		//	Init();
		//}
		if (keys[VK_ESCAPE])
			done=TRUE;
		DrawGLScene();
		SwapBuffers(::GetDC(m_hWnd));
	}
	Kill();
	return msg.wParam;
 }

int CGLWindowHandler::CreateThread(Scene::CGLScene *scene, HANDLE hSignalObj)
{
	UINT threadID;
	m_hSignalObj = hSignalObj;
	m_pScene = scene;
	InitializeCriticalSection(&m_criticalSection);
	if (!(m_threadHandle = (HANDLE) ::_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))ThreadLoop,
		this, 0, &threadID)))
	{
		Log::CLog::Write("CGLWindowHandler::CreateThread\tThread creation failed...");
		return -1;
	}
	return 0;
}
void CGLWindowHandler::RunThread(void)
{
	if (m_eThreaded == E_NOTHREAD)
		m_eThreaded = E_THREAD_RC_NOTHREAD_WNDPROC;
	if (AttachRenderingContext())
	{
		::CloseHandle(m_threadHandle);
		return;
	}
	Init();
	Resize(m_width, m_height);
	MSG msg;
	m_bHasMsgToProcess = FALSE;
	m_bDone = FALSE;
	while (!m_bDone)
	{
		
		if (m_bHasMsgToProcess)
		{
			::EnterCriticalSection(&m_criticalSection);
			msg = m_stMsg;
			::LeaveCriticalSection(&m_criticalSection);
			m_dwResult = WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			m_bHasMsgToProcess = FALSE;
			::SetEvent(m_hSignalObj);
		}
		m_pScene->Draw(m_hDC);
		::Sleep(1);
	}
	DetachRenderingContext();
	::SetEvent(m_hSignalObj);
	::CloseHandle(m_threadHandle);
	::DeleteCriticalSection(&m_criticalSection);
}
void CGLWindowHandler::ThreadLoop(void* param)
{
	((CGLWindowHandler*)param)->RunThread();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
#ifdef _WIN64
    CGLWindowHandler* wndHandler = (CGLWindowHandler*)::GetWindowLong(hWnd, GWLP_USERDATA);
#else if
	CGLWindowHandler* wndHandler = (CGLWindowHandler*)::GetWindowLong(hWnd, GWL_USERDATA);
#endif
	if (wndHandler)
		wndHandler->m_bHandled = TRUE;
	else
		return 0;
	if (! (wndHandler->m_dwResult = wndHandler->m_pScene->WndProc(hWnd, message, wParam, lParam)))
		return wndHandler->m_dwResult;
	switch (message)
	{
	case WM_ACTIVATE:
		if (!HIWORD(wParam))
			active=true;
		else 
			active=false;
		return 0;
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		default:
			wndHandler->m_bHandled = FALSE;
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CLOSE:
  		PostQuitMessage(0);
		wndHandler->m_bHandled = FALSE;
		return 0;
	case WM_KEYDOWN:
		keys[wParam]=TRUE;
		return 0;
	case WM_KEYUP:
		keys[wParam]=FALSE;
		return 0;
	case WM_SIZE:
		wndHandler->Resize(LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_DESTROY:
		//PostQuitMessage(0);
		wndHandler->m_bDone = true;
		wndHandler->m_bHandled = FALSE;
		break;
	default:
		if ( wndHandler->DefWndProcShouldBeCalled())
			return DefWindowProc(hWnd, message, wParam, lParam);
		wndHandler->m_bHandled = FALSE;
		break;
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance, LPCSTR lpszWindowClass, LPCSTR lpszMenuName, HICON hIcon, HICON hIconSm )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= lpszMenuName;
	wcex.lpszClassName	= lpszWindowClass;
	wcex.hIconSm		= hIconSm;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// for testing purposes...
GLfloat		rotY = 0.0f, rotX = 0.0f;
GLvoid DrawGLScene(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();						// Reset The View
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);						// Drawing Using Triangles
		glVertex3f( 3.0f, 3.0f, 3.0f);				// Top
		glVertex3f(2.9f,2.9f, 3.0f);				// Bottom Left
		glVertex3f( 3.1f,2.9f, 3.0f);				// Bottom Right
	glEnd();
	glTranslatef(0.0f,0.0f,-10.0f);					// Move Left 1.5 Units And Into The Screen 6.0
	
	glBegin(GL_TRIANGLES);						// Drawing Using Triangles
		glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
		glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
		glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
	glEnd();	
		glTranslatef(3.0f,0.0f,0.0f);	
			glBegin(GL_QUADS);						// Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f);				// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);				// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
	glEnd();
	glRotatef(rotY,0.0f,1.0f,0.0f);	
	glRotatef(rotX,1.0f,0.0f,0.0f);	
	//scene.Draw();
	rotY += 0.02f;
	rotX += 0.015f;
}
