#include "GLColorMap.h"
#include "Log.h"
#include "GLTypes.h"
namespace Scene
{
	CGLColorMap::CGLColorMap(const unsigned int uvmap, GLuint extID) :
		CGLTextureMap(uvmap, extID),
		_pImg(NULL)
	{
		_nDim = 2;
	}

	CGLColorMap::~CGLColorMap(void)
	{
		DEL(_pImg);
	}
	void CGLColorMap::Bind(GLenum nAct)
	{
		CGLTextureMap::Bind(nAct);
		for (TexEnv::const_iterator it = _texEnv.begin(); it != _texEnv.end(); it++)
		{
			glTexEnvi(GL_TEXTURE_ENV, (*it).first, (*it).second);
		}
	}
	void CGLColorMap::Init(GLenum nAct, GLfloat val)
	{
		_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_REPLACE));
	}
	GLenum CGLColorMap::FromImage(Img::CImg * pImg)
	{
		_pImg = pImg;
		Img::ImgData& imgData = pImg->GetImage();
		if ((imgData.width & imgData.width - 1) || (imgData.height & imgData.height - 1))
			Log::CLog::Write("CGLColorMap::FromImage Warning: Image does not have power-of-two dimensions...\r\n");
		GLenum pf = GLPixelFormatConverter(imgData.pf);
		glGenTextures(1, &_nID);
		GLenum error = glGetError();
		if (error)
			return error;
		glBindTexture(GL_TEXTURE_2D, _nID);
		// TODO: Image instance should store information about filtering...
		glTexImage2D(GL_TEXTURE_2D, 0, imgData.bpp>>3, imgData.width, imgData.height, 0, pf, GL_UNSIGNED_BYTE, imgData.data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		return glGetError();
	}
}