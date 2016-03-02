// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
#include <GL/glaux.h>
#else
#include <GL/glx.h>
#endif
#include "VectorMath.h"


namespace Scene
{

typedef float _FLOAT;
#define MOUSE_BUTTONS 3
#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
class CCamera 
{
	const _FLOAT magnx, magny, magnh, magnp;
	_FLOAT m_fDivX, m_fDivY, m_fDivH, m_fDivP;
	GLint m_nWidth, m_nHeight;
	GLint m_nX, m_nY, m_nPrevX, m_nPrevY;
	CVector<_FLOAT> t, r; // Store the current translation and rotation
	bool mouse_state[MOUSE_BUTTONS];
public:
	CCamera();
	virtual ~CCamera();
	inline GLint GetWidth() { return m_nWidth; };
	inline GLint GetHeight() { return m_nHeight; };
	inline bool GetMouseState(int index) { return mouse_state[index]; };
	inline void SetMouseState(int index, bool state ) {mouse_state[index] = state; };
	void SetMousePos(int x, int y);
	inline CVector<_FLOAT>& GetPos() { return t; };
	inline CVector<_FLOAT>& GetRot() { return r; };
	GLvoid SetupViewport(GLint width, GLint height);
	void HandleMouseData(void *);
	inline GLvoid AdjustTranslation()	{ glTranslatef(t.a, t.b, t.c); };
	inline GLvoid AdjustRotation()
	{
		glRotatef(r.a,1.0f,0.0f,0.0f);	
		glRotatef(r.b,0.0f,1.0f,0.0f);	
		glRotatef(r.c,0.0f,0.0f,1.0f);	
	};
	inline void SetMouseStartPos(int x, int y) { m_nX = x; m_nY = y; };
	inline void Translate(int x, int y, int z) { t.a += (_FLOAT)x / m_fDivX; t.b += (_FLOAT)y / m_fDivY; t.c += (_FLOAT)z / m_fDivY; };
	inline void Rotate(int h, int p, int b) { r.a += (_FLOAT)h / m_fDivH; r.b += (_FLOAT) p / m_fDivP; r.c += (_FLOAT)b / m_fDivP; };
};

}