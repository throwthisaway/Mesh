#pragma once
//#include "MeshElements.h"
#include "FileReader.h"
using namespace IO;
namespace Scene
{
//
//typedef enum eMESHLOADER_ERROR
//{
//	FILE_OK,
//	FILE_CANT_OPEN,
//	FILE_CHUNK_READ_ERROR,
//	FILE_INVALID_TAG,
//	FILE_POINTPOOL_READ_ERROR,
//	FILE_POLYGONPOOL_READ_ERROR,
//	FILE_SURFACE_READ_ERROR,
//	FILE_NO_DATA
//}MESHLOADER_ERROR;
//
//class CChunk
//{
//public:
//	long tag, size, element;
//};
//
//// TODO: abstract interface class
//class CMeshLoader
//{
//	CFileReader *m_FileLoader;
//	MESHLOADER_ERROR LoadChunk(const char * tag, CChunk * chunk);
//	MESHLOADER_ERROR LoadHeader();
//public:	
//	const char * m_pszMeshName;
//	MESHLOADER_ERROR LoadVertices(CVertices&);
//	MESHLOADER_ERROR LoadPolygons(CPolygons&);
//	MESHLOADER_ERROR LoadSurfaces(Surfaces&);
//	//MESHLOADER_ERROR LoadVMaps(CVMaps&);
//	MESHLOADER_ERROR _LoadUVMaps(_CVMaps& vmaps);
//	MESHLOADER_ERROR _LoadDVMaps(_CVMaps& vmaps);
//	BOOL IsFileOpen(void);
//	CMeshLoader(const char *);
//	~CMeshLoader(void);
//};

}