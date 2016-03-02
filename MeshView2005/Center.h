#pragma once
#include "GLExtensions.h"
#include <vector>
#include "VectorMath.h"
#include "IItem.h"
#include "GLProgram.h"
namespace Scene
{
	class CCenter : public CGLExtensions, public IItem
	{
		ProgramMap& shaders;
		static std::vector<FLOAT> vertices;
		Vector3<FLOAT> pos, rot, scale;
		Vector<FLOAT> col;
		static BOOL init;
		static GLuint vbo;
		GLenum error;
	public:
		CCenter(ProgramMap& shaders, Vector3<FLOAT>& pos, Vector3<FLOAT>& rot, Vector3<FLOAT>& scale, Vector<FLOAT>& col);
		virtual ~CCenter();
		virtual void Cleanup(void);
		virtual void Draw(void);
	};
}
