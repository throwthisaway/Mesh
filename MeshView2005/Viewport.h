// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "w32types.h"
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>
#include "VectorMath.h"
#include "CrtDbg.h"


namespace Scene
{

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

//#define VS6

#define MOUSE_BUTTONS 3
#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
class CCamera  
{
#ifndef VS6
	// Raw input data handling...
	static UINT dwSize;
	static BYTE lpb[40];
	// ...Raw input data handling
#endif
	const float magnx, magny, magnh, magnp;
	float m_fDivX, m_fDivY, m_fDivH, m_fDivP;
	GLint m_nWidth, m_nHeight;
	GLint m_nX, m_nY, m_nPrevX, m_nPrevY;
	HWND m_hWnd;
	Vector<float> t, r; // Store the current translation and rotation
	bool mouse_state[MOUSE_BUTTONS];
public:
	CCamera();
	virtual ~CCamera();
	BOOL SetupRawInputDevice(BOOL handleInputMsg, HWND hWnd);
	inline GLint GetWidth(GLvoid) { return m_nWidth; };
	inline GLint GetHeight(GLvoid) { return m_nHeight; };
	inline bool GetMouseState(int index) { return mouse_state[index]; };
	inline void SetMouseState(int index, bool state ) {mouse_state[index] = state; };
	void SetMousePos(int x, int y);
	inline Vector<float>& GetPos(GLvoid) { return t; };
	inline Vector<float>& GetRot(GLvoid) { return r; };
	GLvoid SetupViewport(GLint width, GLint height);
	inline GLvoid AdjustTranslation(GLvoid)	{ glTranslatef(t.x, t.y, t.z); };
	inline GLvoid AdjustRotation(GLvoid)
	{
		//_RPTF3(_CRT_WARN, "AdjustRotation( ) %.4f %.4f %.4f", r.x, r.y, r.z );
		glRotatef(r.x,1.0f,0.0f,0.0f);	
		glRotatef(r.y,0.0f,1.0f,0.0f);	
		glRotatef(r.z,0.0f,0.0f,1.0f);	
	};
    GLvoid SetProjection(void);
#ifndef _VS6
	void HandleRawData(LPARAM lParam);
	void HandleMouseData(PRAWMOUSE mouse);
#endif
	inline void Translate(int x, int y, int z) { t.x += (float)x / m_fDivX; t.y += (float)y / m_fDivY; t.z += (float)z / m_fDivY; };
	inline void Rotate(int h, int p, int b) { r.x += (float)h / m_fDivH; r.y += (float) p / m_fDivP; r.z += (float)b / m_fDivP; 
	/*_RPTF3(_CRT_WARN, "Rotate( ) %.4f %.4f %.4f", r.x, r.y, r.z );*/ };

};

}