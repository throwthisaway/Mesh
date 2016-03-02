// Hud.h: interface for the CHud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUD_H__909E004D_37A2_458E_B0BF_94B4D73168C7__INCLUDED_)
#define AFX_HUD_H__909E004D_37A2_458E_B0BF_94B4D73168C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef WIN32
#include <windows.h>
#endif
#include "GLExtensions.h"
#include "GLBitmapFont.h"

namespace Scene
{

#define LINELEN 128
#define HUDENTRIES 6
typedef struct st_HudEntry
{
	float x, y;
	long len;
	const char * text;
	char line[LINELEN];

}HudEntry;

class CHud : CGLExtensions
{
	GLhandleARB m_Program;
	static HudEntry hudEntries[HUDENTRIES];	
	BOOL m_bShow;
	CGLBitmapFont font;
	long m_nWidth, m_nHeight;	
public:
	void Init(long width, long height, GLhandleARB program);
	inline void Show(bool show) { m_bShow = show; };
	void Set(long index,...);
	void Cleanup(void);
	void Draw(void);
	void PrintXY(int x, int y, const char * str, ...);
	CHud();
	virtual ~CHud();

};

}
#endif // !defined(AFX_HUD_H__909E004D_37A2_458E_B0BF_94B4D73168C7__INCLUDED_)
