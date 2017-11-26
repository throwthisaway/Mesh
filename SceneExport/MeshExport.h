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
	MeshLoader::index_t v1, v2, v3;
	LWPolID id;
	const char * surf;
	size_t layer;
};
struct Layer {
	struct Pivot {
		float x, y, z;
	}pivot;
	struct {
		size_t start, count;
	}poly, line;
	struct SurfSection {
		size_t index, start, count;
	};
	std::vector<SurfSection> polySections, lineSections;
};
struct Vertex
{
	float pos[3];
	LWPntID id;	
};

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
	// TODO:: remove this
	LWSurfaceFuncs *surff;
	std::string fname; // File name
	const char * name; // Object name
	MeshLoader::Surfaces surf;
	LWSurfaceID *surfIDList;
	std::vector<std::string> shaders_to_surfIDs;
	std::vector<Poly> lines;
	std::vector<Poly> polygons;
	std::vector<Vertex> vertices;
	std::vector<::Layer> layers;
private:
	int obj;
	DWORD Pad(size_t val);
	void WriteTag(const char * tag, size_t size, size_t count);
	void WriteHeader();
	void CleanupUVMaps(MeshLoader::_UVMap ** uvmap, size_t size);
	void CleanupDVMaps(MeshLoader::_DVMap ** dvmap, size_t size);
	void DumpPoints();
	void DumpPolygons();
	void DumpLines();
	void DumpLayers(MeshLoader::Layer *layers, size_t count);
	bool CollectVMap(int object, MeshLoader::_UVMap * uvmap, MeshLoader::_DVMap * dvmap, LWID type, int index);
	void DumpVMaps(int object);
	int CollectImage(MeshLoader::Image ** image, LWImageID imgID);
	int CollectSurfaceLayer(MeshLoader::SurfLayer ** pSurfLayer, LWTextureID txtID);
	int CollectSurfaceInfos(MeshLoader::SurfInfo ** pSurfInfo, const LWSurfaceID *surfID);
	void CollectSurfaces(MeshLoader::Surface *surf, const LWSurfaceID *surfID, const std::string& prgName);
	size_t FindSurfaceIndexInSurfIdList(const char* name);
	//void GatherLayerSurfaceSections(std::vector<Poly>& polygons, size_t start, size_t count, std::vector<Layer::SurfSection>& sections);
	void GatherLayerSurfaceSections(std::vector<Poly>& polygons, size_t start, size_t count, MeshLoader::Layer::Sections& section);
public:
	CMeshExport(GlobalFunc *global);
	int GetSurfIDList(void);
	void Save(LWObjectFuncs * objFunc, int object);
	void CleanUp(void);
	~CMeshExport(void);
};
