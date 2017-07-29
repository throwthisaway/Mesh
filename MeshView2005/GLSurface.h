#pragma once
#include "GLProgram.h"
#include "meshsurface.h"
#include "GLTypes.h"
#include "GLTextureMap.h"
#include <vector>
namespace Scene
{
class CGLSurface : CGLExtensions
{
	static const GLfloat _shinFactor;
	typedef std::vector<CGLTextureMap*> ColorMaps;
	ColorMaps _colorMaps;
	void Cleanup(void);
	void Blending(void);
    GLenum error;
public:
	CGLSurface(const MeshLoader::Surface& pSrf);
	~CGLSurface(void);
	GLfloat _color[4];
	GLfloat _spec[4];
	GLfloat _shin;
	GLenum _colorMat;
	typedef GLEnumSet Caps;
	Caps _enableCaps, _disableCaps;
	CGLProgram * _program;
	//GLhandleARB _handle;				//Program handle for the surface to display with
	GLenum _cullFace, _frontFace;					//GL_FRONT, GL_BACK
	void Setup(void);
	void Bind(void);
	void TearDown(void);
	void AdjustBlending(GLfloat value);
	void AdjustSpecularity(GLfloat value);
	void AdjustShininess(GLfloat value);
	void AdjustDummy(GLfloat value) { /*intentionally left blank*/};
	void AdjustCulling(BOOL bEnable, GLenum mode = 0);
};
}
