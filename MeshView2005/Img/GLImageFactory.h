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
	static CImg * CreateImg(const MeshLoader::Image * image);
	static CImg * CreateImg(LPCTSTR path);
	static int LoadImage(LPCTSTR path, Img::CImg *pImg);
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
