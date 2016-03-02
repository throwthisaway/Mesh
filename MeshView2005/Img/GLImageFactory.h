#pragma once
#include "Img.h"
#include "Image.h"
#include "Log.h"

namespace Img
{
// Determines image type by extension and creates the corresponding class...
class CGLImageFactory
{
public:
	static CImg * CreateImg(const Image * image);
	static CImg * CreateImg(LPCTSTR path);
	static int LoadImage(LPCTSTR path, Img::CImg *pImg);
	CGLImageFactory(void);
	~CGLImageFactory(void);
};

class CGLImageFactoryException
{
public:
	//CGLImageFactoryException() {};
	CGLImageFactoryException(LPCTSTR msg, LPCTSTR str)
	{
		Log::CLog::Write(msg, str);
	};
};

}
