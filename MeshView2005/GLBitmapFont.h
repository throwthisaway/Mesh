#pragma once
#ifdef WIN32
#include <windows.h>
#endif
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
//#include <GL/glaux.h>
#else
#include <GL/glx.h>
#endif
#include "Img\\Bmp.h"
#include "GLExtensions.h"

namespace Scene
{

class CGLBitmapFont : CGLExtensions
{
	GLuint m_nFont;
	GLuint m_nFontList;
	void Clear(void);
	void Build();
public:
	BOOL m_bLoaded;
	inline GLint GetList() { return m_nFontList; };
	inline BOOL IsLoaded(void) { return m_bLoaded; };
	BOOL Load(const char * fname);
	CGLBitmapFont(void);
	GLvoid SetDrawState();
	GLvoid RestoreDrawState();
	virtual ~CGLBitmapFont(void);
};

}