//#ifdef WIN32
#include "StdAfx.h"
//#endif
#include "WinEditor.h"

namespace Scene
{
#define SURFACE_EDITOR_CLASS _T("SURFACEDITOR")
#define SURFACE_EDITOR_WINDOW_TITLE _T("Surface editor")
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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
CWinEditor::CWinEditor() : m_hCursor(NULL), m_hWnd(NULL)
{
}

BOOL CWinEditor::CreateAndShow(int x, int y, int width, int height, HWND hWndParent, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	WNDCLASSEX wcex = {sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL, // TODO:: icon
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		SURFACE_EDITOR_CLASS,
		NULL, // TODO:: icon small
	};
	::RegisterClassEx(&wcex);
	m_hWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, SURFACE_EDITOR_CLASS, SURFACE_EDITOR_WINDOW_TITLE, WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW, x, y , width, height, hWndParent, NULL, hInstance, NULL);
	if (!m_hWnd)
		return FALSE;
	::ShowWindow(m_hWnd, TRUE);
	return TRUE;
}

void CWinEditor::Destroy()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
		::UnregisterClass(SURFACE_EDITOR_CLASS, m_hInstance);
	}
	if (m_hCursor)
	{
		::DeleteObject(m_hCursor);
		m_hCursor = NULL;
	}

}

CWinEditor::~CWinEditor(void)
{
	Destroy();
}

}
