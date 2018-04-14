#include "GLTypes.h"
#include "glext.h"
namespace Scene
{
	GLenum GLPixelFormatConverter(Img::PixelFormat pf)
	{
		switch(pf)
		{
		case Img::PixelFormat::Greyscale8:
			return GL_RED;
		case Img::PixelFormat::RGB8:
			return GL_RGB;
		case Img::PixelFormat::RGBA8:
			return GL_RGBA;
		case Img::PixelFormat::BGR8:
			return GL_BGR;
		case Img::PixelFormat::BGRA8:
			return GL_BGRA;
		}
		return GL_RGBA;
	}
}
