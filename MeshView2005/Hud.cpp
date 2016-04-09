#include "stdafx.h"
#include "File.h"
#include "Hud.h"
#include "AppConstants.h"

namespace Scene
{

	HudEntry CHud::hudEntries[] = {{10.0f, 0.0f, 0, "fps: %s", ""}, {10.0f, 10.0f, 0, "Pos: %g %g %g", ""}, {10.0f, 25.0f, 0, "Rot: %g %g %g", ""}, {10.0f, 40.0f, 0, "State: %d %d %d", ""}, {640.0f, 550.0f, 0, "%s","F1 : Fullscreen"}, {640.0f, 535.0f, 0, "%s", "F7 : Compile"}};

CHud::CHud() : m_bShow(TRUE), m_Program(0)
{	

}

CHud::~CHud()
{

}

void CHud::Init(long width, long height, GLhandleARB program)
{
	m_Program = program;
	m_nWidth = width; m_nHeight = height;
	try
	{
		font.Load(CAppConstants::font_path);
		m_bShow = font.m_bLoaded;
	}catch (IO::CFileNotFoundException)
	{
		Log::CLog::Write("CHud: Can't load font (%s)...\r\n", CAppConstants::font_path);
		m_bShow = FALSE;
	}
	hudEntries[0].y = (float)height - 50.0f/* Magic number: fontsize + menubar height*/;
	for (long i = 0; i < HUDENTRIES; i++)
		hudEntries[i].len = (long)strlen(hudEntries[i].line);
}

void CHud::Set(long index,...)
{
	va_list arglist;
	va_start(arglist,index);
	#ifdef WIN32
	hudEntries[index].len=_vsnprintf(hudEntries[index].line,LINELEN,hudEntries[index].text,arglist);
	#else
	hudEntries[index].len=vsnprintf(hudEntries[index].line,LINELEN,hudEntries[index].text,arglist);
	#endif
	va_end(arglist);
}

void CHud::PrintXY(int x, int y, const char * str, ...)
{
	static char buf[256];
	va_list arglist;
	va_start(arglist, str);
	vsprintf(buf, str, arglist);
	va_end(arglist);
	if (ShaderSupported)
		glUseProgramObjectARB(m_Program);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_nWidth,0,m_nHeight,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef((float)x, (float)y, 0.f);
	glListBase(font.GetList()-32);
	font.SetDrawState();
	glCallLists((GLsizei)strlen(buf),GL_UNSIGNED_BYTE,buf);
	font.RestoreDrawState();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void  CHud::Cleanup(void)
{

}

void CHud::Draw(void)
{
	if (!m_bShow)
		return;
	if (CGLExtensions::ShaderSupported)
	{
		glUseProgramObjectARB(m_Program);
	}
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_nWidth,0,m_nHeight,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glListBase(font.GetList()-32);	
	font.SetDrawState();

	for (long i = 0; i < HUDENTRIES; i++)
	{
		HudEntry *p = &hudEntries[i];
		glLoadIdentity();
		glTranslatef(p->x,p->y,0.0f);
		glCallLists(p->len,GL_UNSIGNED_BYTE,p->line);
	}
	font.RestoreDrawState();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}

}
