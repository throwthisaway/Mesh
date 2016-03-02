#include "GLTypes.h"
#include "glext.h"
namespace Scene
{
	GLenum GLPixelFormatConverter(Img::PIXELFORMAT pf)
	{
		switch(pf)
		{
		case Img::PF_GREYSCALE8:
			return GL_RED;
		case Img::PF_RGB:
			return GL_RGB;
		case Img::PF_RGBA:
			return GL_RGBA;
		case Img::PF_BGR:
			return GL_BGR;
		case Img::PF_BGRA:
			return GL_BGRA;
		}
		return GL_RGBA;
	}
}
