#pragma once
#include "GLColorMap.h"
namespace Scene
{
	class CGLColorMapAlpha :
		public CGLColorMap
	{
	public:
		CGLColorMapAlpha(const unsigned int uvmap, GLuint extID = 0);
		virtual ~CGLColorMapAlpha(void);
		virtual void Init(GLenum nAct, GLfloat val);
	};
}
