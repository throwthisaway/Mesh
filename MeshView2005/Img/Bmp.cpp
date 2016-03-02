//#ifdef WIN32
#include "StdAfx.h"
//#else 
//#include "intel_compatibility.h"
//#endif
#include "Bmp.h"
#include "HelperMacros.h"
#include "FileReader.h"
#include "GLConfiguration.h"

using namespace IO;

namespace Img
{
	CBmp::CBmp(void)
	{
		pic.data = NULL;
	}

	CBmp::~CBmp(void)
	{
		Cleanup();
	}
	#define BMP_HEADER_SIZE 14
	typedef struct st_BMPHeader
	{
		unsigned short magicNumber;
		unsigned int size; //size of file
		unsigned short reserved1, reserved2;
		unsigned int offset;
	}BMPHeader;
	#define BMP_DIB_HEADER_SIZE 40 
	typedef enum eBI_COMP{_BI_RGB, _BI_RLE8, _BI_RLE4, _BI_BITFIELDS, _BI_JPEG, _BI_PNG}BI_COMP;
	typedef struct st_DIBHeader
	{
		unsigned int headerSize; // should be 40
		int width, height;	// in pixels
		unsigned short planes/*should be 1*/, bpp/*bts per pixel*/;
		BI_COMP comp;
		unsigned int size/*image size with padding*/, horzRes/*unreliable*/, vertRes/*unreliable*/, nColor/*no. of colors*/, iColor/*no. of importantr colors*/;
	}DIBHeader;
	typedef struct st_BMP
	{
		BMPHeader bmpHeader;
		DIBHeader dibHeader;
		unsigned char * pal;
		unsigned char * data;
		st_BMP() : pal(NULL), data(NULL) {};
		~st_BMP () { DEL_ARRAY(pal); DEL_ARRAY(data); };
		
	}BMP;
	void Cleanup(BMP ** bmp)
	{

		DEL_ARRAY((*bmp)->pal);
		DEL(*bmp);
	}

	void CBmp::Cleanup(void)
	{
		DEL_ARRAY(pic.data);
	}

	BMP * Convert8BitToRGB(CFileReader * fr, BMP * bmp)
	{
		bmp->data = new unsigned char[bmp->dibHeader.width * 3 * bmp->dibHeader.height];
		unsigned char *data = (unsigned char *)_malloca(bmp->dibHeader.width);
		try
		{
			for (int j = 0;j<bmp->dibHeader.height;j++)
			{
				int n = fr->Read(data, bmp->dibHeader.width);
				if (n<1)
				{
					Cleanup(&bmp);
					return NULL;
				}
				for (int k = 0;k<bmp->dibHeader.width;k++)
				{					
					bmp->data[(j*bmp->dibHeader.width+k)*3] = bmp->pal[data[k]*4+2];//bmp->pal[data[k]];
					bmp->data[(j*bmp->dibHeader.width+k)*3+1] = bmp->pal[data[k]*4+1];
					bmp->data[(j*bmp->dibHeader.width+k)*3+2] = bmp->pal[data[k]*4];						
				}
			}
		}catch(...)
		{
			_freea(data);
			throw;
		}
		_freea(data);
		return bmp;
	}	
	BMP * Treat8BitAsGreyScale(CFileReader * fr, BMP * bmp)
	{
		bmp->data = new unsigned char[bmp->dibHeader.width * bmp->dibHeader.height];
		try
		{
			if (fr->Read(bmp->data, bmp->dibHeader.width * bmp->dibHeader.height) < 1)
			{
				Cleanup(&bmp);
				return NULL;
			}
		}catch(...)
		{
			delete [] bmp->data;
			bmp->data = NULL;
			throw;
		}
		return bmp;
	}
	BMP * BMPLoad(CFileReader * fr)
	{
		BMP * bmp = new BMP();
		try
		{
			// Read bmp header
			if (!fr->Read(&bmp->bmpHeader, BMP_HEADER_SIZE))
			{
				delete bmp;
				return NULL;
			}
			// Check
			if (bmp->bmpHeader.magicNumber != 0x4d42)
			{
				Log::CLog::Write("CBmp: Not a bitmap...\r\n");
				delete bmp;
				return NULL;
			}
			// Read dib header
			if (!fr->Read(&bmp->dibHeader, BMP_DIB_HEADER_SIZE))
			{
				delete bmp;
				return NULL;
			}		
			if (bmp->dibHeader.comp != _BI_RGB)
			{
				Log::CLog::Write("CBmp: Compressed bitmap...\r\n");
				delete bmp;
				return NULL;
			}
			// Read palette
			if ((bmp->dibHeader.bpp == 4) || (bmp->dibHeader.bpp == 8))
			{
				int palSize;
				bmp->pal = new unsigned char[palSize = ((bmp->dibHeader.nColor == 0) ? 1<<bmp->dibHeader.bpp:bmp->dibHeader.nColor) * 4];
				fr->Read(bmp->pal, palSize);
				
				int width = (bmp->dibHeader.bpp == 4) ? bmp->dibHeader.width>>1:bmp->dibHeader.width;
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
						return Treat8BitAsGreyScale(fr, bmp);
					else
						return Convert8BitToRGB(fr, bmp);
				}
			}		
			//bmp->data = new unsigned char[bmp->dibHeader.size];
			//TODO:: 24 32 bit
			 		
		}catch (CFileNotOpenException& ex)
		{
			Log::CLog::Write(ex.msg.c_str());
			DEL(bmp);
		}
		catch (...)
		{
			DEL(bmp);
			throw;
		}
		return bmp;
	}
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
			BMP * bmp = BMPLoad(fr);
			delete fr;
			if (bmp)
			{
				pic.width = bmp->dibHeader.width;
				pic.height = bmp->dibHeader.height;
				pic.bpp = bmp->dibHeader.bpp;
				
				int res = PixelFormat(pic.bpp, pic.pf);
				// TODO:: handle 
				pic.data = bmp->data;
				bmp->data = NULL;
				//long size = pic.width * pic.height *(pic.bpp>>3);
				//pic.data = new unsigned char[size];
				//memcpy(pic.data, bmp->data, size);
				Img::Cleanup(&bmp);
				//!!! dtor deletes bmp->data too...
				//DEL(bmp);
			}
		}catch (CFileExceptionBase ex)
		{
			Log::CLog::Write("CBmp: Can't read file (%s)...\r\n", fname);
			return ID_BMP_LOAD;
		}
		// Obsolote
		//#endif
		return ID_BMP_OK;
	}

	ImgData& CBmp::GetImage(void)
	{
		return pic;
	}
}
