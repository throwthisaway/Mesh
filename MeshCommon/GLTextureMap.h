#pragma once
#include "GLExtensions.h"
#include <meshsurface.h>
#include "gl/gl.h"
namespace Scene
{
	class CGLTextureMap : public CGLExtensions
	{
	public:
		GLint _nDim;
		GLuint _nID;
		GLint _nUV;
		CGLTextureMap(const unsigned int uvmap, GLuint extID = 0);
		virtual void Cleanup(void);
	public:
		class ActiveTextureProvider
		{
			static GLenum _nAct;
			ActiveTextureProvider() {};
		public:
			static GLenum First(void) { return GL_TEXTURE0; }
			static void Reset(void) { _nAct = First(); }
			static GLenum Next(void) { return _nAct++; }
			static GLenum Peek(void) { return _nAct; }
		};
		virtual ~CGLTextureMap(void);
		void Bind(void);
		virtual void Bind(GLenum nAct);
		void TearDown(void);
		void TearDown(GLenum nAct);
		virtual void Init(GLenum nAct, GLfloat val) = 0;
        inline GLuint GetID(void) { return _nID; }
	};
}
