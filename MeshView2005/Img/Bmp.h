#pragma once
//#ifdef WIN32
//#include <gl\glaux.h>
//#endif
#include "Img.h"
#include <Log.h>

namespace Img
{
	#define ID_BMP_OK ID_IMG_OK
	#define ID_BMP_FNAM 0x30001
	#define ID_BMP_FILE 0x30002
	#define ID_BMP_LOAD 0x30003

	class CBmp : public CImg
	{
		ImgData pic;
	public:
		virtual ImgData& GetImage();
		virtual int Load(const char * fname);
		virtual void Cleanup() override {};
	};
}