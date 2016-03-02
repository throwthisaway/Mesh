#include "GLBumpMap.h"
#include "Log.h"
#include "GLTypes.h"
#include "GLConfiguration.h"

namespace Scene
{
	CGLBumpMap::CGLBumpMap(const unsigned int uvmap, GLuint extID) :
		CGLColorMap(uvmap, extID)
	{
		//_nDim = 3;
	}
	GLenum CGLBumpMap::FromImage(Img::CImg * pImg)
	{
		_pImg = pImg;
		_normalMap = pImg->GetImage();
		if ((_normalMap.width & _normalMap.width - 1) || (_normalMap.height & _normalMap.height - 1))
			Log::CLog::Write("CGLColorMap::FromImage Warning: Image does not have power-of-two dimensions...\r\n");
		_normalMap.bpp = 24;
		_normalMap.pf = Img::PF_RGB;
		BYTE Bpp = _normalMap.bpp>>3;
		_normalMap.size = _normalMap.width * _normalMap.height * Bpp;
		_normalMap.data = new unsigned char[_normalMap.size];
		unsigned char * pImgData = pImg->GetImage().data;
		for (int y = _normalMap.height - 2; y >= 0; --y)
		{
			int yOffs = y * _normalMap.width;
			unsigned char * pImgDataOffs = pImgData + yOffs, * _pNMapDataOffs = _normalMap.data + yOffs * Bpp;
			for (int x = _normalMap.width - 2; x >= 0; --x)
			{
				// x
				_pNMapDataOffs[x * Bpp] = 127 + (((pImgDataOffs[x] - pImgDataOffs[x + 1]) << 7) >> 8);
				// y 
				_pNMapDataOffs[x * Bpp + 1] =  127 +  (((pImgDataOffs[x] - pImgDataOffs[x + _normalMap.height]) << 7) >> 8);
				// z 
				_pNMapDataOffs[x * Bpp + 2] = 255;
			}
			// Last column
			// x
			_pNMapDataOffs[(_normalMap.width - 1) * Bpp] =  127 +  (((pImgDataOffs[_normalMap.width - 1] - *pImgDataOffs) << 7) >> 8);	// Image data last column pixel - first column pixel
			// y 
			_pNMapDataOffs[(_normalMap.width - 1) * Bpp + 1] =  127 +  (((pImgDataOffs[_normalMap.width - 1] - pImgDataOffs[_normalMap.height]) << 7) >> 8);
			// z 
			_pNMapDataOffs[(_normalMap.width - 1) * Bpp + 2] = 255;
		}
		// Last row
		int yOffs = (_normalMap.height - 1) * _normalMap.width;
		unsigned char * pImgDataOffs = pImgData + yOffs, * _pNMapDataOffs = _normalMap.data + yOffs * Bpp;
		for (int x = _normalMap.width - 2; x >= 0; --x)
		{
			// x
			_pNMapDataOffs[x * Bpp] = 127 + ((( pImgDataOffs[x] - pImgDataOffs[x + 1]) << 7) >> 8);
			// y 
			_pNMapDataOffs[x * Bpp + 1] = 127 +  (((pImgDataOffs[x] - pImgData[x]) << 7) >> 8);	// Image data last actual row pixel - first row pixel
			// z 
			_pNMapDataOffs[x * Bpp + 2] = 255;
		}
		// Last column
		// x
		_pNMapDataOffs[(_normalMap.width - 1) * Bpp] = 127 +  (((pImgDataOffs[_normalMap.width - 1] - *pImgDataOffs) << 7) >> 8); // Image data last column pixel - first column pixel
		// y 
		_pNMapDataOffs[(_normalMap.width - 1) * Bpp + 1] = 127 +  (((pImgDataOffs[_normalMap.width - 1] - *pImgData) << 7) >> 8); // Image data last row pixel - first row pixel
		// z 
		_pNMapDataOffs[(_normalMap.width - 1) * Bpp + 2] = 255;

		GLenum pf = GLPixelFormatConverter(_normalMap.pf);

		glGenTextures(1, &_nID);
		GLenum error = glGetError();
		if (error)
		{
			_normalMap.Cleanup();
			return error;
		}
		glBindTexture(GL_TEXTURE_2D, _nID);
		glTexImage2D(GL_TEXTURE_2D, 0, Bpp, _normalMap.width, _normalMap.height, 0, pf, GL_UNSIGNED_BYTE, _normalMap.data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		if (!GLConfiguration::PreserveImageData)
			_normalMap.Cleanup();
		return glGetError();
	}

	void CGLBumpMap::Bind(GLenum nAct)
	{
		CGLTextureMap::Bind(nAct);
		for (TexEnv::const_iterator it = _texEnv.begin(); it != _texEnv.end(); it++)
		{
			glTexEnvi(GL_TEXTURE_ENV, (*it).first, (*it).second);
		}
	}
	void CGLBumpMap::Init(GLenum nAct, GLfloat val)
	{
		//_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_REPLACE));
		_texEnv.push_back(TexEnv::value_type(GL_TEXTURE_ENV_MODE, GL_COMBINE));
		_texEnv.push_back(TexEnv::value_type(GL_COMBINE_RGB, GL_DOT3_RGBA_EXT));
		_texEnv.push_back(TexEnv::value_type(GL_SOURCE0_RGB, GL_PRIMARY_COLOR));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND0_RGB, GL_SRC_COLOR));
		_texEnv.push_back(TexEnv::value_type(GL_SOURCE1_RGB, GL_TEXTURE));
		_texEnv.push_back(TexEnv::value_type(GL_OPERAND1_RGB, GL_SRC_COLOR));
	}
	CGLBumpMap::~CGLBumpMap(void)
	{
	}
}

