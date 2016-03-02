#include "GLColorMapAlpha.h"
namespace Scene
{
	CGLColorMapAlpha::CGLColorMapAlpha(const unsigned int uvmap, GLuint extID) : CGLColorMap(uvmap, extID)
	{
	}

	CGLColorMapAlpha::~CGLColorMapAlpha(void)
	{
	}

	void CGLColorMapAlpha::Init(GLenum nAct, GLfloat val)
	{
		_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_COMBINE));
		_texEnv.push_back(TexEnv::value_type(GL_COMBINE_RGB, GL_INTERPOLATE));
		_texEnv.push_back(TexEnv::value_type(GL_SRC0_RGB, GL_PREVIOUS/*nAct - 2*/));
		_texEnv.push_back(TexEnv::value_type(GL_SRC1_RGB, nAct - 1));
		_texEnv.push_back(TexEnv::value_type(GL_SRC2_RGB, nAct));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND0_RGB, GL_SRC_COLOR));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND1_RGB, GL_SRC_COLOR));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND2_RGB, GL_SRC_ALPHA));
	}
}
