#include "stdafx.h"
#include "Center.h"
#include "HelperMacros.h"

namespace Scene
{
	GLuint CCenter::vbo;
	BOOL CCenter::init = FALSE;
	std::vector<FLOAT> CCenter::vertices;
	CCenter::CCenter(ProgramMap& shaders, Vector3<FLOAT>& pos, Vector3<FLOAT>& rot, Vector3<FLOAT>& scale, Vector<FLOAT>& col) :
		pos(pos),
		rot(rot),
		scale(scale),
		col(col),
		shaders(shaders)
	{
		if (!init)
		{
			vertices.reserve(3 * 6);
			vertices.push_back(-1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
			vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
			vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
			vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
			vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(-1.0f);
			vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);
			glGenBuffersARB(1, &vbo);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * sizeof(FLOAT), &(*vertices.begin()), GL_STATIC_DRAW_ARB);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			init = TRUE;
		}
	}
	CCenter::~CCenter()
	{
		//Cleanup();
	}
	void CCenter::Cleanup(void)
	{
		if (init)
		{
			glDeleteBuffersARB(1, &vbo);
		}
	}
	void CCenter::Draw(void)
	{
		glPushAttrib(GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
		glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
		glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
		glScalef(scale[0], scale[1], scale[2]);
		glUseProgramObjectARB(0);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glColor4fv(col);
		glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size() * 3);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glPopMatrix();
		glPopAttrib();
	}
}