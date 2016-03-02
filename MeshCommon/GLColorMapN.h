#pragma once
#include "GLColorMap.h"
namespace Scene
{
	class CGLColorMapN :
		public CGLColorMap
	{
	public:
		CGLColorMapN(const unsigned int uvmap, GLuint extID = 0);
		virtual ~CGLColorMapN(void);
		virtual void Init(GLenum nAct, GLfloat val);
	};
}
