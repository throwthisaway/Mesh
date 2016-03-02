#pragma once
#include <windows.h>
namespace Scene
{
	class GLConfiguration
	{
	public:
		static BOOL Treat8BitAsGreyScale;		// Treat the color indexed bitmaps as greayscale images
		static BOOL PreserveImageData;			// Preserve the image data in memory after texture generation
	};
}

