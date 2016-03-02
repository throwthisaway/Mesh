#pragma once
#include "GLExtensions.h"
#include <vector>
#include "VectorMath.h"
#include "IItem.h"
#include "GLProgram.h"
namespace Scene
{

	class CLines : public CGLExtensions, public IItem
	{
		static const int VBOS = 2;
		ProgramMap& shaders;
		GLuint vbo[VBOS];
		GLenum error;
		GLsizei size;
	public:
		CLines(ProgramMap& shaders, std::vector<FLOAT>& vertices, std::vector<FLOAT>& color);
		virtual ~CLines();
		virtual void Cleanup(void);
		virtual void Draw(void);
	};
}

