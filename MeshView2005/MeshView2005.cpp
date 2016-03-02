// MeshView2005.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MeshView2005.h"
#include "GLScene.h"
#include "AppConstants.h"


#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )	
//#pragma comment( lib, "glaux.lib" )	

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
BOOL fullscreen = FALSE;
HWND hWnd;
HDC hDC;
HGLRC hRC;
int width, height;
DEVMODE DMorig;
BOOL done = FALSE;
BOOL active=TRUE;
bool keys[256];

static Scene::CGLScene scene;

GLfloat		rotY = 0.0f, rotX = 0.0f;	

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

#ifdef _VS6
#define _INT_PTR int
#else
#define _INT_PTR INT_PTR
#endif
_INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GLvoid KillGLWindow(BOOL bUnregisterClass = TRUE);
GLvoid DrawGLScene();
HWND CreateGLWindow(TCHAR * title,int width,int height,int bits,BOOL fullscreenflag,int nCmdShow);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MESHVIEW2005, szWindowClass, MAX_LOADSTRING);
	fullscreen = FALSE;
	MyRegisterClass(hInstance);
	// Perform application initialization:
	
	if (!InitInstance (hInstance, nCmdShow))
	{
		KillGLWindow();
		return -1;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MESHVIEW2005));

	// Main message loop:
/*	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/	
	if (!scene.Init(hWnd, hInstance))
	{
		KillGLWindow();
		return -1;
	}
	while (!done)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
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

		if (keys[VK_ESCAPE])
			done=TRUE;
		if (keys[VK_F1])
		{
			keys[VK_F1]=FALSE;
			KillGLWindow(FALSE);
			fullscreen=!fullscreen;
			if (!(hWnd = CreateGLWindow(::CAppConstants::app_name,1280,1024,32,fullscreen, nCmdShow)))
				done = TRUE;
			scene.Init(hWnd, hInstance);
		}
		else if(keys[VK_F7])
		{
			scene.ProgramReload();
		}
		scene.Draw(hDC);
//		DrawGLScene();		
	}
	KillGLWindow();
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MESHVIEW2005));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MESHVIEW2005);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
// KillGLWindow();
GLvoid KillGLWindow(BOOL bUnregisterClass)
{
	if (fullscreen)
	{
		if (!ChangeDisplaySettings(NULL,CDS_TEST))
		{
			ChangeDisplaySettings(NULL,CDS_RESET);
			ChangeDisplaySettings(&DMorig,CDS_RESET);
		}
		else
			ChangeDisplaySettings(NULL,CDS_RESET);
		ShowCursor(TRUE);
	}
	if (hRC) 
	{
		if (!wglMakeCurrent(NULL,NULL))		
			MessageBox(NULL,_T("Release of Device Context and Rendering Context failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		if (!wglDeleteContext(hRC))
			MessageBox(NULL,_T("Release Rendering Context failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		hRC=NULL;
	}
	if (hDC && !ReleaseDC(hWnd,hDC))
	{
		MessageBox(NULL,_T("Release Device Contex failed..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		hDC=NULL;
	}

	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL,_T("Could not release hWnd..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		hWnd=NULL;
	}
	if (bUnregisterClass && !UnregisterClass(szWindowClass,hInst))
	{
		MessageBox(NULL,_T("Could not unregister class..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		hInst=NULL;
	}
}
// ResizeGLScene(...);
GLvoid ResizeGLScene(GLint width,GLint height)
{
	scene.vp.SetupViewport(width, height);
	::width=width;
	::height=height;
}
// CreateGLWindow(...);
// return: HWND, if successful, NULL on failure 
HWND CreateGLWindow(TCHAR * title,int width,int height,int bits,BOOL fullscreenflag,int nCmdShow)
{
	HWND hWnd;
	DWORD dwStyle,dwExStyle;
	GLuint PixelFormat;
	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&DMorig);
	fullscreen = fullscreenflag;
	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight=height;
		dmScreenSettings.dmPelsWidth=width;
		dmScreenSettings.dmBitsPerPel=bits;
		dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL,_T("Can't switch to fullscreen...\n	Changing to windowed mode"),_T("ERROR"),MB_OK|MB_ICONERROR);
			fullscreen=FALSE;
		}
	}
	if (fullscreen)
	{
		dwStyle=WS_POPUP;
		dwExStyle=WS_EX_APPWINDOW;
		ShowCursor(TRUE);
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
	AdjustWindowRectEx(&WindowRect,dwStyle,FALSE,dwExStyle);
	if (!(hWnd=CreateWindowEx(dwExStyle,szWindowClass,title,WS_CLIPSIBLINGS|WS_CLIPCHILDREN|dwStyle,0,0,WindowRect.right-WindowRect.left,WindowRect.bottom-WindowRect.top,NULL,NULL,hInst,NULL)))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Can't create window..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	
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
	if (!(hDC=GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Cannot create the GL Device Context..."),_T("ERORR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Cannot find a suitable pixelformat..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Cannot change the pixelformat..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	if (!(hRC=wglCreateContext(hDC)))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Cannot create the GL Rendering Context..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	if(!wglMakeCurrent(hDC,hRC))
	{
		KillGLWindow();
		MessageBox(NULL,_T("Cannot activate the GL Rendering Context..."),_T("ERROR"),MB_OK|MB_ICONERROR);
		return NULL;
	}
	ShowWindow(hWnd,nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ResizeGLScene(width,height);	
	return hWnd;
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateGLWindow(::CAppConstants::app_name,800,600,32,fullscreen, nCmdShow);

   if (!hWnd)
   {
      return FALSE;
   }
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
	if (!scene.WndProc(hWnd, message, wParam, lParam))
		return 0;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ACTIVATE:
		if (!HIWORD(wParam))
			active=TRUE;
		else 
			active=FALSE;
		return 0;
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CLOSE:
  	PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		keys[wParam]=TRUE;
		return 0;
	case WM_KEYUP:
		keys[wParam]=FALSE;
		return 0;
	case WM_SIZE:
		ResizeGLScene(LOWORD(lParam),HIWORD(lParam));
		return 0;
	/*case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;*/
	/*case WM_DESTROY:
		PostQuitMessage(0);
		break;*/
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
_INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// DrawGLScene();
GLvoid DrawGLScene()
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
	glRotatef(rotY,0.0f,1.0f,0.0f);	
	glRotatef(rotX,1.0f,0.0f,0.0f);	
	//scene.Draw();
	rotY += 0.02f;
	rotX += 0.015f;
	
}