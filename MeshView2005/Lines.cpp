#include "stdafx.h"
#include "Lines.h"

namespace Scene
{

	CLines::CLines(ProgramMap& shaders, std::vector<FLOAT>& vertices, std::vector<FLOAT>& color) :
		shaders(shaders),
		size((GLsizei)vertices.size() * 3)
	{
		glGenBuffersARB(VBOS, vbo);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * sizeof(FLOAT), &(*vertices.begin()), GL_STATIC_DRAW_ARB);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, color.size() * sizeof(FLOAT), &(*color.begin()), GL_STATIC_DRAW_ARB);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}


	CLines::~CLines()
	{
		Cleanup();
	}
	void CLines::Cleanup(void)
	{
		glDeleteBuffersARB(VBOS, vbo);
	}
	void CLines::Draw(void)
	{
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glDisableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glPushAttrib(GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glUseProgramObjectARB(0);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glColorPointer(4, GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glDrawArrays(GL_LINES, 0, size);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glPopAttrib();
		glPopClientAttrib();
	}
}