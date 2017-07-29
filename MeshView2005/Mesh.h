#pragma once
#include "../../MeshLoader/types.h"
#include "GLExtensions.h"
#include "GLSurface.h"
#include "Editor.h"
#include <map>
#include <vector>
#include <memory>
#include "VectorMath.h"
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
class CMesh : public CGLExtensions, public MeshLoader::Mesh
{
	ProgramMap& shaders;
protected:
	GLenum error;
	std::vector<CEditor*> m_Editors;
	GLuint m_nVBOVertices, m_nVBONormals;
	void CleanUpSurfaces(void);
	void CleanUp(void);

    CGLProgram fixedFunc;
    int ExportData(LPCSTR vertexData, const void* data, const size_t byteLen);
public:
	uint8_t* rawData = nullptr;
	//
	std::vector<CGLSurface> _glSurfaces;
	//
	std::string m_szName;
	virtual void Draw(void);
	void Setup(unsigned int vbovertices);
	void SetupGLSurfaces(ProgramMap& shaders);
    CGLProgram * PrgOverride(MeshLoader::Surface& srf);

	CMesh(ProgramMap& shaders);
	virtual ~CMesh(void);
	
};

}