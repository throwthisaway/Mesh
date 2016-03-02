#pragma once
#include "GLExtensions.h"
#include <DirectXMath.h>
#include <vector>
#include "VectorMath.h"
#include "IItem.h"
#include "GLProgram.h"
#include "GLScene.h"

namespace Scene
{
	class CSphere : public CGLExtensions, public IItem
	{
		static const int VBOS = 2;
		ProgramMap& shaders;
		static std::vector<FLOAT> vertices, normals;
		Vector3<FLOAT> pos, rot, scale;
		Vector<FLOAT> col;
		FLOAT shininess;
		static BOOL init;
		static GLuint vbo[VBOS];
		GLenum error;
		CGLProgram * pProgram;
		void Subdivide(DirectX::XMVECTOR& v1, DirectX::XMVECTOR& v2, DirectX::XMVECTOR& v3, int depth);
		void Init(void);
	public:
		CSphere(ProgramMap& shaders, Vector3<FLOAT>& pos, Vector3<FLOAT>& rot, Vector3<FLOAT>& scale, Vector<FLOAT>& col);
		virtual ~CSphere();
		virtual void Draw(void);
		virtual void Cleanup(void);
	};
}

