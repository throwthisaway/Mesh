#pragma once
#include <w32types.h>
#include <lwserver.h>
#include <vector>
#include <list>
#include <string>
#include <lwsurf.h>
#include <lwpanel.h>
#include "serializer\serializer.h"
#include <MeshSurface.h>

struct Poly
{
	long v1, v2, v3;
	LWPolID id;
	const char * surf;
};
struct PolyLine {
	long v1, v2;
	LWPolID id;
	const char * surf;
};
typedef struct
{
	float pos[3];
	LWPntID id;	
}Vertex;

class CMeshExport
{
	HANDLE m_hFile;
	BOOL m_bAllLayers, m_bShowMeshExportPanel;
	Ser_Base serBase, serBaseUV, serBaseDV;
	std::list<LWID> vmapList;
    static const std::string defPrg;
public:
	GlobalFunc * global;
	LWMeshInfo * meshInfo;
	LWSurfaceFuncs *surff;

	std::string fname; // File name
	const char * name; // Object name
	Surfaces surf;
	LWSurfaceID *surfIDList;
	std::vector<std::string> shaders_to_surfIDs;
	unsigned int cVertex, nVertices;
	std::vector<PolyLine> lines;
	std::vector<Poly> polygons;
	Vertex * vertices;
private:
	int obj;
	long WriteTag(const char * szTag,const long nSize,const long nElement);
	void WriteHeader(void);
	void SortPolygonsBySurfIDs();
	void CleanupUVMaps(_UVMap ** uvmap, size_t size);
	void CleanupDVMaps(_DVMap ** dvmap, size_t size);
	void DumpPoints(void);
	void DumpPolygons(void);
	void DumpLines(void);
	int DumpVMap(int object, _UVMap * uvmap, _DVMap * dvmap, LWID type, int index);
	int DumpVMaps(int object);	
	int DumpImage(Image ** image, LWImageID imgID);
	int DumpSurfaceLayer(SurfLayer ** pSurfLayer, LWTextureID txtID);
	int DumpSurfaceInfos(SurfInfo ** pSurfInfo, const LWSurfaceID *surfID);
	void DumpSurfaces(Surface *surf, const LWSurfaceID *surfID, const std::string& prgName);
public:
	CMeshExport(GlobalFunc *global);
	int GetSurfIDList(void);
	void Save(LWObjectFuncs * objFunc, int object);
	void CleanUp(void);
	~CMeshExport(void);
};