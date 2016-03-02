// Tga.h: interface for the CTga class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGA_H__DF6611FB_509C_43E3_9EAC_66AE41357381__INCLUDED_)
#define AFX_TGA_H__DF6611FB_509C_43E3_9EAC_66AE41357381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#include <gl\gl.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "FileReader.h"
#include "Img.h"

namespace Img
{
	//////////////////////////////////////////////////////////////////////
	// TGA Loader
	//////////////////////////////////////////////////////////////////////
	#define ID_TGA_OK ID_IMG_OK
	#define ID_TGA_FNAM 0x3001
	#define ID_TGA_FLEN 0x3002
	#define ID_TGA_FFMT 0x3003
	#define ID_TGA_AMEM 0x3004
	#define ID_TGA_UDAT 0x3005

	const GLubyte compressedTGA[12]={0,0,10,0,0,0,0,0,0,0,0,0};
	const GLubyte uncompressedTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	typedef struct stImgData stTga;

	static int error=0;
	//stTga * LoadTGA(const char * fname);

	//stTga * LoadcTGA(FILE * f);
	//stTga * LoaduTGA(FILE * f);
	class CTga : CImg 
	{
		IO::CFileReader * _fr;
		stTga image;
		int LoaduTGA(void);
		int LoadcTGA(void);
		int ParseHeader(void);

	public:
		virtual ImgData& GetImage(void);
		virtual int Load(const char * fname);
		virtual void Cleanup(void);
		CTga(void);
		virtual ~CTga(void);

	};
}
#endif // !defined(AFX_TGA_H__DF6611FB_509C_43E3_9EAC_66AE41357381__INCLUDED_)

