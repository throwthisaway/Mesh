#pragma once
#include "HelperMacros.h"
#include "GLExtensions.h"
#include "Vertex.h"

namespace Scene
{

class _CVMaps  : CGLExtensions
{
	typedef struct st_SurfaceUVMap
	{
		long n;
		float * uv;
		GLuint VBO;
		st_SurfaceUVMap() : uv(NULL), VBO(0), n(0) {};
		~st_SurfaceUVMap() { DEL_ARRAY(uv); glDeleteBuffersARB(1, &VBO); };
	}SurfaceUVMap;
	void CreateSurfaceUVs(long n, SurfaceUVMap * uvmap, unsigned int  offset, unsigned int  num, CPolygons& polygons, CVertices& vertices);
public:
	long nuvmaps;
	SurfaceUVMap** uvmaps;	
	GLsizei m_nUVMaps;
	_UVMap * m_UVMap;
	_DVMap * m_DVMap;
	void Setup(Surfaces&, CPolygons&, CVertices&);
	void CleanUp(void);
	_CVMaps() : nuvmaps(0), uvmaps(NULL), m_UVMap(NULL), m_nUVMaps(0), m_DVMap(NULL) {};
	~_CVMaps() { CleanUp(); };
};

//class CVMaps  : Scene::CGLExtensions
//{
//public:
//	GLuint * m_pnVBOUVCoords;
//	GLsizei m_nUVMaps;
//	UVMap * m_UVMap;
//	void Setup(Surfaces&);
//	void CleanUp(void);
//	CVMaps() : m_pnVBOUVCoords(0), m_UVMap(0), m_nUVMaps(0) {};
//	~CVMaps() { CleanUp(); };
//};

}
