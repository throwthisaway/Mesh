//#ifdef WIN32
//#include "StdAfx.h"
//#else 
//#include "intel_compatibility.h"
//#endif
#include "Bmp.h"
#include "HelperMacros.h"
#include "FileReader.h"
#include "GLConfiguration.h"

using namespace IO;

namespace
{
#define BMP_HEADER_SIZE 14
	typedef struct st_BMPHeader
	{
		unsigned short magicNumber;
		unsigned int size; //size of file
		unsigned short reserved1, reserved2;
		unsigned int offset;
	}BMPHeader;
#define BMP_DIB_HEADER_SIZE 40 
	typedef enum eBI_COMP { _BI_RGB, _BI_RLE8, _BI_RLE4, _BI_BITFIELDS, _BI_JPEG, _BI_PNG }BI_COMP;
	typedef struct st_DIBHeader
	{
		unsigned int headerSize; // should be 40
		int width, height;	// in pixels
		unsigned short planes/*should be 1*/, bpp/*bts per pixel*/;
		BI_COMP comp;
		unsigned int size/*image size with padding*/, horzRes/*unreliable*/, vertRes/*unreliable*/, nColor/*no. of colors*/, iColor/*no. of importantr colors*/;
	}DIBHeader;
	struct BMP {
		BMPHeader bmpHeader;
		DIBHeader dibHeader;
		std::unique_ptr<uint8_t> pal, data;
	};

	void Convert8BitToRGB(CFileReader * fr, BMP * bmp) {
		bmp->data = std::unique_ptr<uint8_t>(new uint8_t[bmp->dibHeader.width * 3 * bmp->dibHeader.height]);
		unsigned char *data = (unsigned char *)_malloca(bmp->dibHeader.width);
		try {
			bmp->dibHeader.bpp = 24;
			for (int j = 0; j < bmp->dibHeader.height; ++j) {
				auto n = fr->Read(data, bmp->dibHeader.width);
				if (n < 1) return;
				for (int k = 0; k < bmp->dibHeader.width; ++k) {
					bmp->data.get()[(j*bmp->dibHeader.width + k) * 3] = bmp->pal.get()[data[k] * 4 + 2];//bmp->pal[data[k]];
					bmp->data.get()[(j*bmp->dibHeader.width + k) * 3 + 1] = bmp->pal.get()[data[k] * 4 + 1];
					bmp->data.get()[(j*bmp->dibHeader.width + k) * 3 + 2] = bmp->pal.get()[data[k] * 4];
				}
			}
		}
		catch (...) {
			_freea(data);
			throw;
		}
		_freea(data);
	}
	void Treat8BitAsGreyScale(CFileReader * fr, BMP * bmp) {
		bmp->data = std::unique_ptr<uint8_t>(new uint8_t[bmp->dibHeader.width * bmp->dibHeader.height]);
		if (fr->Read(bmp->data.get(), bmp->dibHeader.width * bmp->dibHeader.height) < 1) {
			Log::CLog::Write("Treat8BitAsGreyScale: File read error...\r\n");
		}
	}
	std::unique_ptr<BMP> BMPLoad(CFileReader * fr) {
		auto bmp = std::make_unique<BMP>();
		try {
			// Read bmp header
			if (!fr->Read(&bmp->bmpHeader, BMP_HEADER_SIZE)) return bmp;
			// Check
			if (bmp->bmpHeader.magicNumber != 0x4d42) return bmp;
			// Read dib header
			if (!fr->Read(&bmp->dibHeader, BMP_DIB_HEADER_SIZE)) return bmp;
			if (bmp->dibHeader.comp != _BI_RGB)
			{
				Log::CLog::Write("CBmp: Compressed bitmap...\r\n");
				return bmp;
			}
			// Read palette
			if ((bmp->dibHeader.bpp == 4) || (bmp->dibHeader.bpp == 8))
			{
				int palSize;
				bmp->pal = std::unique_ptr<uint8_t>(new uint8_t[palSize = ((bmp->dibHeader.nColor == 0) ? 1 << bmp->dibHeader.bpp : bmp->dibHeader.nColor) * 4]);
				fr->Read(bmp->pal.get(), palSize);

				int width = (bmp->dibHeader.bpp == 4) ? bmp->dibHeader.width >> 1 : bmp->dibHeader.width;
				//unsigned char *data = new unsigned char[width]; 			

				if (bmp->dibHeader.bpp == 4)
				{
					/*for (int i=0;i<bmp->dibHeader.height;i++)
					{
						fr->Read(data, width);
						for (int j = bmp->dibHeader.height-1;j>=0;j++)
						{
							for (int k = 0;k<bmp->dibHeader.width;)
							{
								int palPos = data[k>>4];
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
								palPos = data[k&8];
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
								bmp->data[j*bmp->dibHeader.width+k] = bmp->pal[palPos]; palPos++; k++;
							}
						}
					}*/
					//delete [] data;
				}
				else if (bmp->dibHeader.bpp == 8)
				{
					if (Scene::GLConfiguration::Treat8BitAsGreyScale)
						Treat8BitAsGreyScale(fr, bmp.get());
					else
						Convert8BitToRGB(fr, bmp.get());
					return bmp;
				}
			}
			//bmp->data = new unsigned char[bmp->dibHeader.size];
			//TODO:: 24 32 bit

		}
		catch (CFileNotOpenException& ex) {
			Log::CLog::Write(ex.msg.c_str());
		}
		catch (...)	{
			throw;
		}
		return bmp;
	}
}
namespace Img {
	int CBmp::Load(const char * fname)
	{
	// Obsolote
	//#ifdef WIN32
	//	AUX_RGBImageRec * bmp = NULL;
	//#endif
		FILE * f;
		Cleanup();
		if (!fname)
		{
			Log::CLog::Write("CBmp: File name is null...\r\n");
			return ID_BMP_FNAM;
		}
		f=fopen(fname,"rb");
		if (!f)
		{
			Log::CLog::Write("CBmp: File (%s) open error...\r\n", fname);
			return ID_BMP_FILE;
		}
		// Obsolote
		//#ifdef WIN32
		//// A: hack
		//bmp = auxDIBImageLoadA(fname);
		//if (!bmp)
		//{
		//	Log::CLog::Write("CBmp: Can't read file (%s)...\r\n", fname);
		//	return ID_BMP_LOAD;
		//}
		//// Set Info
		//pic.width = bmp->sizeX;
		//pic.height = bmp->sizeY;
		//// TODO:: test it==>pic.data = bmp->data;
		//pic.bpp = (bmp->bpp == 32) ? 32 :24; //TODO: ???
		//pic.pf = PF_RGB;
		//DEL_ARRAY(bmp->data);
		//DEL(bmp);
		//#else
		try
		{
			CFileReader * fr = CFileReader::Open(fname);
			auto bmp = BMPLoad(fr);
			delete fr;
			if (bmp)
			{
				pic.width = bmp->dibHeader.width;
				pic.height = bmp->dibHeader.height;
				pic.bpp = bmp->dibHeader.bpp;
				
				int res = PixelFormat(pic.bpp, pic.pf);
				pic.data = std::move(bmp->data);
			}
		}catch (CFileExceptionBase ex)	{
			Log::CLog::Write("CBmp: Can't read file (%s)...\r\n", fname);
			return ID_BMP_LOAD;
		}
		// Obsolote
		//#endif
		return ID_BMP_OK;
	}

	ImgData& CBmp::GetImage(void) {
		return pic;
	}
}
