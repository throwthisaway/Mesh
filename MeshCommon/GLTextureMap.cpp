#include "GLTextureMap.h"

namespace Scene
{
	GLenum CGLTextureMap::ActiveTextureProvider::_nAct = GL_TEXTURE0;
	CGLTextureMap::CGLTextureMap(const unsigned int uvmap, GLuint extID) :
		_nID(extID),
		_nUV(uvmap)
	{
	}

	CGLTextureMap::~CGLTextureMap(void)
	{
		Cleanup();
	}
	void CGLTextureMap::Cleanup(void)
	{
		if (_nID)
		{
			glDeleteTextures(1, &_nID);
			_nID = 0;
		}
	}
	void CGLTextureMap::Bind(void)
	{
		Bind(ActiveTextureProvider::Next());
	}
	void CGLTextureMap::Bind(GLenum nAct)
	{				
		glClientActiveTextureARB(nAct);
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _nUV);
		_ASSERT(_nDim != 0);
		glTexCoordPointer(_nDim, GL_FLOAT, 0, (char *) NULL);
		//
		glActiveTextureARB(nAct);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _nID);
	}
	void CGLTextureMap::TearDown(void)
	{
		TearDown(ActiveTextureProvider::Next());
	}
	void CGLTextureMap::TearDown(GLenum nAct)
	{
		glActiveTextureARB(nAct);
		glDisable( GL_TEXTURE_2D );
		glClientActiveTextureARB(nAct);
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
}
