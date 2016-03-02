#pragma once
#ifdef WIN32
//#include "stdafx.h"
#endif
#include "w32types.h"
#include "HelperMacros.h"

namespace Img
{
	#define ID_IMG_OK ID_OK
	#define ID_IMG_UBPP 0x3006
	typedef enum {PF_UNKNOWN, PF_GREYSCALE8, PF_RGB, PF_RGBA, PF_BGR, PF_BGRA}PIXELFORMAT;
	
	typedef struct stImgData
	{
		unsigned char * data;
		UINT size;
		UINT width,height;
		BYTE bpp;
		PIXELFORMAT pf;
		void Cleanup(void) { delete [] data; data = NULL; };
		struct stImgData(void) : data(NULL) {};
		~stImgData(void) { Cleanup(); };
	}ImgData;

	class CImg
	{
	public:
		virtual ImgData& GetImage(void) = 0;
		virtual int Load(const char * fname) = 0;
		virtual void Cleanup(void) = 0;
		int PixelFormat(int bpp, PIXELFORMAT& pf);
		CImg(void);
		virtual ~CImg(void);
	};
}
