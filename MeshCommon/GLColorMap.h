#pragma once
#include "GLTextureMap.h"
#include "Img.h"
#include "GLTypes.h"

namespace Scene
{
	class CGLColorMap :
		public CGLTextureMap
	{
	protected:
		Img::CImg *_pImg;
	public:
		TexEnv _texEnv;
		CGLColorMap(const unsigned int uvmap, GLuint extID = 0);
		virtual ~CGLColorMap(void);
		virtual void Bind(GLenum nAct);
		virtual void Init(GLenum nAct, GLfloat val);
		virtual GLenum FromImage(Img::CImg * pImg);
	};
}
