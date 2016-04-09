#include "StdAfx.h"
#include "GLImageFactory.h"
#include "AppConstants.h"
#include "Tga.h"
#include "Bmp.h"

namespace Img
{

CGLImageFactory::CGLImageFactory(void)
{
}

CGLImageFactory::~CGLImageFactory(void)
{
}

CImg * CGLImageFactory::CreateImg(const Image * image)
{
	if (!image->path)
		return NULL;
	return CreateImg(image->path);
}

CImg * CGLImageFactory::CreateImg(LPCTSTR path)
{
	const char * ext = _tcsrchr(path,'.');
	if (!ext)	
		throw CGLImageFactoryException(_T("CGLImageFactory::CreateImg: Undeterminable filetype..\r\n\tfile: %s"), path);
	ext++;
	if (!_stricmp(ext, ::CAppConstants::tga))
	{
		return (CImg *)new CTga();
	}
	else if (!_stricmp(ext, ::CAppConstants::bmp))
	{
		return (CImg *)new CBmp();
	}
	/* else if (...other filetypes...) */
	else
		throw CGLImageFactoryException("CGLImageFactory::CreateImg: Unhandled filetype..\r\n\text: %s", ext);
}
int CGLImageFactory::LoadImage(const  char * path, Img::CImg *pImg)
{
#ifndef WIN32
    char path[MAX_PATH];
    strcpy(path, LINUX_IMAGE_PATH);
    char * p = strrchr(pImage->path, '\\');
    if (!p)
        return -1;	
    strcat(path, ++p);
#else
    if (IO::CFileReader::Exist(path))
    {
        try
        {
            pImg->Load(path);
        }catch (Img::CGLImageFactoryException&)
        {
            Log::CLog::Write(" CGLSurface::LoadImage: Loading of image %s failed...\r\n", path);
            throw;
        }
    }
    else
    {
        // Try to reconstruct a different path...
        const char *p;
        if ((p = strstr(path, CAppConstants::img_fullpath_fragment)) == NULL)
            return -1;
        char path2[MAX_PATH];
        strcpy(path2, CAppConstants::img_def_path);
        strcat(path2, p+strlen(CAppConstants::img_fullpath_fragment));
        try
        {
            pImg->Load(path2);
        }catch (Img::CGLImageFactoryException&)
        {
            Log::CLog::Write("CGLSurface::LoadImage: Loading of image %s failed...\r\n", path2);
            throw;
        }
    }
#endif

    return 0;
}
}
