#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <set>
#include "Img.h"
#include <vector>
namespace Scene
{
	typedef std::set<GLenum> GLEnumSet;
	typedef std::vector<std::pair<GLenum, GLenum>> TexEnv;
	GLenum GLPixelFormatConverter(Img::PixelFormat pf);
}
