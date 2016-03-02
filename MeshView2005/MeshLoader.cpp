//#ifdef WIN32
#include "StdAfx.h"
//#endif
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "MeshLoader.h"
#include <new>
#include "Log.h"
using namespace Log;
namespace Scene
{

CMeshLoader::CMeshLoader(const char * fname)
{
	m_pszMeshName = fname;
	try
	{
		m_FileLoader = IO::CFileReader::Open(fname);
	}catch(CFileNotFoundException e)
	{
		//TODO: Log...
		CLog::Write("Can't open file: %s",fname);		
		throw;
	} 
}

CMeshLoader::~CMeshLoader(void)
{
	if (m_FileLoader)
		delete m_FileLoader;
}

MESHLOADER_ERROR CMeshLoader::LoadChunk(const char * tag, CChunk * chunk)
{	
	long read = 0, pos = m_FileLoader->Pos();
	read = m_FileLoader->Read(chunk,sizeof(CChunk));
	while ((read>0) && (TAG(tag) != chunk->tag))
	{
		m_FileLoader->Seek(SEEK_POS,chunk->size);
		read = m_FileLoader->Read(chunk,sizeof(CChunk));
	}
	if (read<1)
	{
		m_FileLoader->Seek(SEEK_BEGIN, pos);
		return FILE_CHUNK_READ_ERROR;
	}
	return FILE_OK;
}

MESHLOADER_ERROR CMeshLoader::LoadHeader()
{
	if (!m_FileLoader)		
		return FILE_CANT_OPEN;
	//TODO:
	return FILE_OK;
}

MESHLOADER_ERROR CMeshLoader::LoadVertices(CVertices& Vertices)
{
	MESHLOADER_ERROR error;
	CChunk chunk;
	if (error = LoadChunk(PNTS,&chunk))
		return error;
	if (!chunk.element)
		return FILE_NO_DATA;
	if (m_FileLoader->Read(Vertices.CreateAndRetrieveInternalData(chunk.element), chunk.size) < 1)
		return FILE_POINTPOOL_READ_ERROR;
	return FILE_OK;
}

MESHLOADER_ERROR CMeshLoader::LoadPolygons(CPolygons& polygons)
{
	MESHLOADER_ERROR error;
	CChunk chunk;
	if (error = LoadChunk(POLS,&chunk))
		return error;
	if (!chunk.element)
		return FILE_NO_DATA;
	polygons.m_Polygons = new CPolygon[chunk.element];
	polygons.m_nPolygons = chunk.element;
	if (m_FileLoader->Read(polygons.m_Polygons, chunk.size) < 1)
		return FILE_POLYGONPOOL_READ_ERROR;
	return FILE_OK;
}

MESHLOADER_ERROR CMeshLoader::LoadSurfaces(Surfaces& surfaces)
{
	MESHLOADER_ERROR error;
	CChunk chunk;
#ifdef _WIN32
	ZeroMemory(&surfaces, sizeof(Surfaces));
#else
	memset(&surfaces, 0, sizeof(Surfaces));
#endif
	if (error = LoadChunk(SURF, &chunk))
		return error;
	char * ptr = new char[chunk.size];	
	RELOCATEPTRARRAY(chunk.size, Surface, ptr, surfaces.m_Surface, chunk.element);
	//surfaces.m_Surface = new(ptr) Surface[chunk.element];
	if (m_FileLoader->Read(surfaces.m_Surface, chunk.size) < 1)
		return FILE_POLYGONPOOL_READ_ERROR;
	surfaces.m_nSurfaces = chunk.element;
	for (int i=0;i<surfaces.m_nSurfaces;i++)
	{
		surfaces.m_Surface[i].Relocate(chunk.size, surfaces.m_Surface);
	}
	//while (!(error = LoadChunk(file,SURF,&chunk)))
	//{
	//	char * ptr = new char[chunk.size];
	//	// TODO: handle bad_alloc
	//	//(ptr)
	//	m_Surfaces = new(ptr) Surfaces();
	//	m_nSurfaces = chunk.element;
	//	for (int i=0;i<chunk.element;i++)
	//	{
	//		RELOCATEPTR(chunk.size,Surface,m_Surfaces,m_Surfaces->m_Surface[i]);			
	//		m_Surfaces->m_Surface[i]->Relocate(chunk.size, m_Surfaces);
	//		// Call ctor /someway/:
	//		m_Surfaces->m_Surface[i] = new(ptr+i*sizeof(Surface*)) Surface();	
	//	}
	//	if (m_FileLoader->Read(m_Surfaces, chunk.size) < 1)
	//		return FILE_SURFACE_READ_ERROR;	
	//}	
	return FILE_OK;
}
//MESHLOADER_ERROR CMeshLoader::LoadVMaps(CVMaps& vmaps)
//{
//	MESHLOADER_ERROR error;
//	CChunk chunk;
//	if (error = LoadChunk(VMAP, &chunk))
//		return FILE_OK;	// VMAP isn't neccesary(?)
//	char * ptr = new char[chunk.size];	
////#ifdef _WIN32
////	ZeroMemory(vmaps, sizeof(CVMaps));
////#else
////	memset(vmaps, 0, sizeof(CVMaps));
////#endif
//	RELOCATEPTRARRAY(chunk.size, UVMap, ptr, vmaps.m_UVMap, chunk.element);
//	if (m_FileLoader->Read(vmaps.m_UVMap, chunk.size) < 1)
//		return FILE_POLYGONPOOL_READ_ERROR;
//	vmaps.m_nUVMaps = chunk.element;
//	for (int i=0;i<vmaps.m_nUVMaps;i++)
//	{
//		vmaps.m_UVMap[i].Relocate(chunk.size, vmaps.m_UVMap);
//	}
//	return FILE_OK;
//}
MESHLOADER_ERROR CMeshLoader::_LoadUVMaps(_CVMaps& vmaps)
{
	MESHLOADER_ERROR error;
	CChunk chunk;
	if (error = LoadChunk(VMP2, &chunk))
		return FILE_OK;	
	char * ptr = new char[chunk.size];	
	RELOCATEPTRARRAY(chunk.size, _UVMap, ptr, vmaps.m_UVMap, chunk.element);
	if (m_FileLoader->Read(vmaps.m_UVMap, chunk.size) < 1)
		return FILE_POLYGONPOOL_READ_ERROR;
	vmaps.m_nUVMaps = chunk.element;
	for (int i=0;i<vmaps.m_nUVMaps;i++)
	{
		vmaps.m_UVMap[i].Relocate(chunk.size, vmaps.m_UVMap);
	}
	return FILE_OK;
}
MESHLOADER_ERROR CMeshLoader::_LoadDVMaps(_CVMaps& vmaps)
{
	MESHLOADER_ERROR error;
	CChunk chunk;
	if (error = LoadChunk(DVMP, &chunk))
		return FILE_OK;	
	char * ptr = new char[chunk.size];	
	RELOCATEPTRARRAY(chunk.size, _DVMap, ptr, vmaps.m_DVMap, chunk.element);
	if (m_FileLoader->Read(vmaps.m_DVMap, chunk.size) < 1)
		return FILE_POLYGONPOOL_READ_ERROR;	
	for (int i=0;i<chunk.element;i++)
	{
		vmaps.m_DVMap[i].Relocate(chunk.size, vmaps.m_DVMap);
	}
	return FILE_OK;
}
BOOL CMeshLoader::IsFileOpen()
{
	if (m_FileLoader && m_FileLoader->IsOpened())
		return TRUE;
	return FALSE;
}

}
