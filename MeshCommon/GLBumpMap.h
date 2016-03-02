#pragma once
#include "GLColorMap.h"
#include "Img.h"
#include "GLTypes.h"
namespace Scene
{
	class CGLBumpMap : public CGLColorMap
	{
	protected:
		Img::ImgData _normalMap;
		//GLuint _nNormCubeMapID;			// Normalistaion Cube Map
	public:
		CGLBumpMap(const unsigned int uvmap, GLuint extID = 0);
		~CGLBumpMap(void);
		virtual void Bind(GLenum nAct);
		virtual void Init(GLenum nAct, GLfloat val);
		virtual GLenum FromImage(Img::CImg * pImg);
	};
}
