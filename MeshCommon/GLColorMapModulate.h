#pragma once
#include "GLColorMap.h"
namespace Scene
{
	class CGLColorMapModulate :
		public CGLColorMap
	{
	public:
		CGLColorMapModulate(const unsigned int uvmap, GLuint extID = 0);
		virtual ~CGLColorMapModulate(void);
		virtual void Init(GLenum nAct, GLfloat val);
	};
}
