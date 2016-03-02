#include "stdafx.h"
#include "Plane.h"
namespace Scene
{
	const FLOAT CPlane::DEF_SCALE = 15.0f;
	CPlane::CPlane(ProgramMap& shaders, Vector<FLOAT>& plane, Vector<FLOAT>& col, FLOAT scale) :
		shaders(shaders),
		scale(scale),
		col(col),
		shininess(80.0f)
	{
		pProgram = shaders[_T("specular.program")];
		glGenBuffersARB(VBOS, vbo);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		std::vector<FLOAT> vertices, normals;
		Vector3<FLOAT> n(plane.x, plane.y, plane.z);
		n.Normalize();
		vertices.reserve(4 * 3);
		// TODO:: create vertices

		size = (GLsizei)vertices.size();
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * sizeof(FLOAT), &(*vertices.begin()), GL_STATIC_DRAW_ARB);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		normals.reserve(4 * 3);
		// TODO:: create normals
		normals.push_back(n.x);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, normals.size() * sizeof(FLOAT), &(*normals.begin()), GL_STATIC_DRAW_ARB);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}


	CPlane::~CPlane()
	{
		Cleanup();
	}
	void CPlane::Cleanup(void)
	{
		glDeleteBuffersARB(VBOS, vbo);
	}
	void CPlane::Draw(void)
	{
		glPushMatrix();
		glScalef(scale, scale, scale);
		//glEnable(GL_COLOR_MATERIAL);
		//glColorMaterial(GL_FRONT, GL_SPECULAR);
		//_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		pProgram->UseProgram();
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glNormalPointer(GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));

		//glCullFace(GL_BACK);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() * 3);
		//_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glPopMatrix();
	}
}