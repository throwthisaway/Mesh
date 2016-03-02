#pragma once
#include "HelperMacros.h"
#include "VectorMath.h"
#include "Polygon.h"
#include <vector>

namespace Scene
{

class CVertex
{
public:
	float x,y,z;
	/*long nPoly;
	CPolygon * poly;*/
};

class CPolygonRef
{
public:
	long count;
	long * indices;
	inline void CleanUp(void) {	DEL_ARRAY(indices); };
	CPolygonRef():indices(NULL),count(0){};
	~CPolygonRef() { CleanUp(); };
};

class CVertices
{
public:
	typedef std::vector<CVertex> VertexArr;
	typedef std::vector<Vector<float>> VectorArr;
	typedef std::vector<CPolygonRef> PolyRefArr;
	VertexArr _vertices;	
	VectorArr _normals;
	PolyRefArr _polyRefs;
	inline VertexArr& GetVertices(void) { return _vertices; };
	inline CVertex& GetVertex(long i) { return _vertices[i]; };
	inline VectorArr& GetNormals(void) { return _normals; };
	inline Vector<float>& GetNormal(long i) { return _normals[i]; };
	inline PolyRefArr& GetPoygonRefs(void) { return _polyRefs; };
	inline VertexArr::size_type GetCount(void) {return _vertices.size(); };
	void* CreateAndRetrieveInternalData(VertexArr::size_type nVertices);
	void SetupNormals(void);
	void Setup(void);
	void CleanUp(void);
	CVertices(){};
	~CVertices() { };
};

}
