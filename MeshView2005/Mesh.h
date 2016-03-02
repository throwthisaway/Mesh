#pragma once
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "MeshLoader.h"
#include "GLExtensions.h"
#include "GLSurface.h"
#include "Editor.h"
#include <map>
#include <vector>
#include <memory>

namespace Scene
{

typedef enum eMESH_ERROR
{
	MESH_OK = 0,
	MESH_FAIL = 1
}MESH_ERROR;

class CMesh;
// (Mesh) Instance Specific Data
typedef struct st_ISD
{
    Vector<float> _pos;
    Vector<float> _rot;
}ISD;
typedef std::vector<std::pair<std::shared_ptr<CMesh>, ISD>> Meshes;
class CMesh : public CGLExtensions
{
	ProgramMap& shaders;
protected:
	GLenum error;
	std::vector<CEditor*> m_Editors;
	//unsigned int m_nVertices, m_nPolys, m_nSurfaces;	
	//long * m_VertexToPolyIndices;
	CVertices m_Vertices;
	CPolygons m_Polygons;
	Surfaces m_Surfaces;	
	GLuint m_nVBOVertices, m_nVBONormals;
	//CVMaps m_VMaps;
	_CVMaps m_VMaps_;
	void CleanUpSurfaces(void);
	void CleanUp(void);
	long CalcPolygonNormals(void);
	std::auto_ptr<float> CalcVertexNormals(void);
    std::auto_ptr<char> DumpColoring(void);
	//
	float * data;
	int c;
	//
	GLuint m_nList;
    CGLProgram fixedFunc;
    int ExportData(LPCSTR vertexData, const void* data, const size_t byteLen);
public:
	//
	std::vector<CGLSurface> _glSurfaces;
	//
	std::string m_szName;
	inline Surfaces& GetSurfaces(void) { return m_Surfaces; }
	inline _CVMaps * GetVMaps(void) {return &m_VMaps_; }
    inline GLuint GetRawVertexCount(void) {return m_Polygons.m_nPolygons * VERTICESPERPOLY; }
	//inline void SetVBOForVertices(unsigned int vbovertices) { m_nVBOVertices = vbovertices; }
	//void BuildList(void);
    MESH_ERROR Load(std::auto_ptr<CMeshLoader> meshLoader);
	virtual void Draw(void);
	void Setup(unsigned int vbovertices);
	void SetupGLSurfaces(ProgramMap& shaders);
    CGLProgram * PrgOverride(Surface& srf);

	CMesh(ProgramMap& shaders);
	virtual ~CMesh(void);
	
};

}