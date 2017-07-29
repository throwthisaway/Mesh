#pragma once
#include "GLTextureMap.h"
#include <Image.h>
#include <map>
namespace Scene
{
	class CGLTextureManager
	{
		//int LoadImage(Image*, CGLTextureMap *);
	public:
		CGLTextureManager(void);
		~CGLTextureManager(void);
		void Cleanup(void);
		typedef std::map<MeshLoader::Image*, CGLTextureMap*, MeshLoader::Image::st_Less> ImageMap;
		ImageMap _images;

		//void Add(GLfloat val, SurfLayer * pLayer);
		//void Add(RefMap * pRefMap);
		//int LoadImages(void);
	};
}
