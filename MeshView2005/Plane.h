#pragma once
#include "GLExtensions.h"
#include <vector>
#include "VectorMath.h"
#include "IItem.h"
#include "GLProgram.h"
#include "GLScene.h"

namespace Scene
{
	class CPlane : public CGLExtensions, public IItem
	{
		ProgramMap& shaders;
		static const int VBOS = 2;
		static const FLOAT DEF_SCALE;
		Vector<FLOAT> col;
		GLuint vbo[VBOS];
		CGLProgram * pProgram;
		FLOAT scale, shininess;
		GLsizei size;
		GLenum error;
	public:
		CPlane(ProgramMap& shaders, Vector<FLOAT>& plane, Vector<FLOAT>& col, FLOAT scale = DEF_SCALE);
		~CPlane();
		virtual void Draw(void);
		virtual void Cleanup(void);
	};
}
