#include "GLColorMapModulate.h"
namespace Scene
{
	CGLColorMapModulate::CGLColorMapModulate(const unsigned int uvmap, GLuint extID) : CGLColorMap(uvmap, extID)
	{
	}

	CGLColorMapModulate::~CGLColorMapModulate(void)
	{
	}

	void CGLColorMapModulate::Init(GLenum nAct, GLfloat val)
	{
		_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_COMBINE));
		_texEnv.push_back(TexEnv::value_type(GL_COMBINE_RGB, GL_MODULATE));
		_texEnv.push_back(TexEnv::value_type(GL_SRC0_RGB, GL_PREVIOUS));
		_texEnv.push_back(TexEnv::value_type(GL_SRC1_RGB, nAct));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND0_RGB, GL_SRC_COLOR));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND1_RGB, GL_SRC_COLOR));
	}
}
