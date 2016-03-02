#include "StdAfx.h"
#include "GLTextureManager.h"

namespace Scene
{
	CGLTextureManager::CGLTextureManager(void)
	{
	}

	CGLTextureManager::~CGLTextureManager(void)
	{
		Cleanup();
	}

	void CGLTextureManager::Cleanup(void)
	{
		for(ImageMap::iterator it = _images.begin();it != _images.end(); it++)
		{
			delete (*it).second;
		}
		_images.erase(_images.begin(), _images.end());
	}
}