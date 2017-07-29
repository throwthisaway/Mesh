//#ifdef WIN32
#include "StdAfx.h"
//#endif
#include "GLBitmapFont.h"
#include "Img\\GLImageFactory.h"
#include "FileReader.h"
#include "Log.h"
#include "GLTypes.h"


namespace Scene
{

#define CHARSIZE 16
#define CHARSPACE CHARSIZE-6

CGLBitmapFont::CGLBitmapFont(void)
{

}

CGLBitmapFont::~CGLBitmapFont(void)
{
	Clear();
}

void CGLBitmapFont::Clear(void)
{
	if (m_bLoaded)
	{
		glDeleteLists(m_nFontList,256);
		glDeleteTextures(1, &m_nFont);
	}
	m_bLoaded = FALSE;
}

BOOL CGLBitmapFont::Load(const char * fname)
{
	Clear();
	Log::CLog::Write("Loading bitmap font...\r\n");
	Img::CImg * pImg = Img::CGLImageFactory::CreateImg(fname);
	if (pImg->Load(fname) != ID_BMP_OK)	
		return m_bLoaded = FALSE;
	m_bLoaded = TRUE;
	glGenTextures(1, &m_nFont);
	GLenum error = glGetError();
	glBindTexture(GL_TEXTURE_2D, m_nFont);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, pImg->GetImage().bpp>>3, pImg->GetImage().width, pImg->GetImage().height,0,GLPixelFormatConverter(pImg->GetImage().pf),GL_UNSIGNED_BYTE, pImg->GetImage().data.get());
	Build();
	delete pImg;
	return TRUE;
}

void CGLBitmapFont::Build()
{
	const int n=256;
	float x,y=1.0f;
	m_nFontList=glGenLists(256);
	for (int i=0;i<n;i++)
	{
		x=(float)i*0.0625f;
		glNewList(m_nFontList+i,GL_COMPILE);
		glBegin(GL_QUADS);
		glColor3f(0.0f,0.7128f,0.0f);
		glTexCoord2f(x,y-0.0625f);glVertex2i(0,0);
		glTexCoord2f(x+0.0625f,y-0.0625f);glVertex2i(CHARSIZE,0);
		glTexCoord2f(x+0.0625f,y);glVertex2i(CHARSIZE,CHARSIZE);
		glTexCoord2f(x,y);glVertex2i(0,CHARSIZE);
		glEnd();
		glTranslated(CHARSPACE,0,0);		
		glEndList();
		if ((i+1)%16==0)
		{
			y-=0.0625f;
			x=0.0f;
		}
	}
}

GLvoid CGLBitmapFont::SetDrawState()
{
	glFrontFace(GL_CCW);
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBlendFunc(GL_ONE,GL_ZERO);
	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_nFont);
	//glEnable(GL_BLEND);	
}

GLvoid CGLBitmapFont::RestoreDrawState()
{
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	//glBlendFunc(GL_ONE,GL_ZERO);		
	glDisable(GL_BLEND);		
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

}

}
