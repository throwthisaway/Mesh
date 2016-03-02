#include "GLColorMapN.h"
namespace Scene
{
	CGLColorMapN::CGLColorMapN(const unsigned int uvmap, GLuint extID) : CGLColorMap(uvmap, extID)
	{
	}

	CGLColorMapN::~CGLColorMapN(void)
	{
	}
	void CGLColorMapN::Init(GLenum nAct, GLfloat val)
	{
		//_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_REPLACE));
		_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_COMBINE));
		_texEnv.push_back(TexEnv::value_type(GL_COMBINE_RGB, GL_REPLACE));
		_texEnv.push_back(TexEnv::value_type(GL_SRC0_RGB, GL_PREVIOUS));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND0_RGB, GL_SRC_COLOR));
	}
}