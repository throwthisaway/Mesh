// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include "StdAfx.h"
#endif
#include "Viewport_glut.h"

namespace Scene
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera():t(0.0,0.0,-10.0), r(0.0,0.0,0.0), magnx(15.0), magny(15.0), magnh(180.0), magnp(180.0)
{

}

CCamera::~CCamera()
{

}

void CCamera::HandleMouseData(void *)
{
	//TODO::
	/*if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
		mouse_state[LEFT_MOUSE_BUTTON] = true;
	else if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
		mouse_state[LEFT_MOUSE_BUTTON] = false;
	if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN )
		mouse_state[RIGHT_MOUSE_BUTTON] = true;
	else if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP )
		mouse_state[RIGHT_MOUSE_BUTTON] = false;
	if (mouse_state[LEFT_MOUSE_BUTTON])
	{
		Rotate( mouse->lLastY, mouse->lLastX, 0);
	}
	if (mouse_state[RIGHT_MOUSE_BUTTON])
	{
		Translate(mouse->lLastX, 0, mouse->lLastY);		
	}*/

}
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
		Translate(difX, 0, difY);		
	}	
}
GLvoid CCamera::SetupViewport(GLint width, GLint height)
{
	if (height==0)
		height=1;
	
	m_fDivX = (_FLOAT)m_nWidth / magnx; m_fDivY = (_FLOAT)m_nHeight / magny;
	m_fDivH = (_FLOAT)m_nWidth / magnh; m_fDivP = (_FLOAT)m_nHeight / magnp;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,m_nWidth = width, m_nHeight = height);
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);//bbox*2.0f
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

}
