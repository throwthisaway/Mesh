// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Viewport.h"
#include "AppConstants.h"
namespace Scene
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef _VS6
// Raw input data handling...
UINT CCamera::dwSize = 40;
BYTE CCamera::lpb[40];
// ...Raw input data handling
#endif

CCamera::CCamera():t(0.0,0.0,-40.0), r(0.0,0.0,0.0), magnx(15.0), magny(15.0), magnh(360.0), magnp(180.0)
{
	ZeroMemory(mouse_state, sizeof(mouse_state));
}

CCamera::~CCamera()
{

}

BOOL CCamera::SetupRawInputDevice(BOOL handleInputMsg, HWND hWnd)
{
#ifndef _VS6
	if (handleInputMsg)
	{
		m_hWnd = hWnd;
		RAWINPUTDEVICE Rid[1];
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
		Rid[0].dwFlags = RIDEV_INPUTSINK;   
		Rid[0].hwndTarget = m_hWnd;
		return RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
	}	
#endif
	return TRUE;
}
#ifndef _VS6
void CCamera::HandleRawData(LPARAM lParam)
{
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, 
		lpb, &dwSize, sizeof(RAWINPUTHEADER));
    
	RAWINPUT* raw = (RAWINPUT*)lpb;
    
	if (raw->header.dwType == RIM_TYPEMOUSE) 
	{
		HandleMouseData(&raw->data.mouse);
	} 
}

void CCamera::HandleMouseData(PRAWMOUSE mouse)
{
	if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
		mouse_state[LEFT_MOUSE_BUTTON] = true;
	else if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
		mouse_state[LEFT_MOUSE_BUTTON] = false;
	if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN )
		mouse_state[RIGHT_MOUSE_BUTTON] = true;
	else if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP )
		mouse_state[RIGHT_MOUSE_BUTTON] = false;
	//if (mouse->usButtonFlags & (RI_MOUSE_LEFT_BUTTON_DOWN & RI_MOUSE_RIGHT_BUTTON_DOWN))
	//{
	//	Translate(mouse->lLastX, 0, mouse->lLastY);		
	//}	
	//else if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
	//{	
	//	Rotate(mouse->lLastX, mouse->lLastY, 0);
	//}
	//else 
	if (mouse_state[LEFT_MOUSE_BUTTON])
	{
		Rotate( mouse->lLastY, mouse->lLastX, 0);
	}
	if (mouse_state[RIGHT_MOUSE_BUTTON])
	{
		Translate(mouse->lLastX, 0, mouse->lLastY);		
	}

}
#endif
void CCamera::SetMousePos(int x, int y)
{
	m_nPrevX = m_nX; m_nPrevY = m_nY; m_nX = x; m_nY = y;
	int difX = m_nX-m_nPrevX, difY = m_nY-m_nPrevY;
	if (mouse_state[LEFT_MOUSE_BUTTON])
	{
		Rotate( difY, difX, 0);
	}
	if (mouse_state[RIGHT_MOUSE_BUTTON])
	{
		Translate(difX*magnx, 0, difY*magny);		
	}
}
GLvoid CCamera::SetupViewport(GLint width, GLint height)
{
	if (height==0)
		height=1;
	glViewport(0,0,m_nWidth = width, m_nHeight = height);
	m_fDivX = (float)m_nWidth / magnx; m_fDivY = (float)m_nHeight / magny;
	m_fDivH = (float)m_nWidth / magnh; m_fDivP = (float)m_nHeight / magnp;
	SetProjection();
}
GLvoid CCamera::SetProjection(void)
{
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(CAppConstants::fovy,(GLfloat)m_nWidth/(GLfloat)m_nHeight,CAppConstants::znear,CAppConstants::zfar);//bbox*2.0f
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
}
