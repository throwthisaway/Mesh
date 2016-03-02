#pragma once
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLScene.h"

class CGLWindowHandler
{
	// For threading...
	enum {E_NOTHREAD, E_THREAD_RC_NOTHREAD_WNDPROC, E_THREAD_RC_AND_WNDPROC} m_eThreaded;
	HANDLE m_hSignalObj;
	// Handle related, refactor!!!
	HANDLE m_threadHandle;
	HWND m_hwndForThread;
	// ...For threading
	DEVMODE m_DMorig;
	HGLRC m_hRC;
	BOOL m_fullscreen;
	HWND m_hWnd;
	HDC m_hDC;
	GLint m_width, m_height;
public:
	static Scene::CGLScene * m_pScene;
	// For threading...
	MSG m_stMsg;
	BOOL m_bHasMsgToProcess;
	BOOL m_bHandled;
	LRESULT m_dwResult;
	CRITICAL_SECTION m_criticalSection;
	inline BOOL DefWndProcShouldBeCalled(void) { return !(m_eThreaded == E_THREAD_RC_NOTHREAD_WNDPROC); };
	// ...For threading
	BOOL m_bDone;
	CGLWindowHandler(void);
	~CGLWindowHandler(void);
	int Create(TCHAR* title, int width, int height, int bits, BOOL threaded = FALSE, BOOL fullscreen = FALSE, HINSTANCE hInst = NULL, HWND hWnd = NULL);
	void Kill(HWND hWnd = NULL, HINSTANCE hInst = NULL);
	void Cleanup(void);
	void Init(void);
	void Resize(GLint width, GLint height);
	int AttachRenderingContext(void);
	void DetachRenderingContext(void);
	int CreateThread(Scene::CGLScene *scene, HANDLE hSignalObj);
	static void ThreadLoop(void* param);
	void RunThread(void);
	WPARAM MessageLoop(void);
};
