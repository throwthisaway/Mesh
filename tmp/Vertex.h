#pragma once
#include "HelperMacros.h"
#include "VectorMath.h"
#include "Polygon.h"

namespace Scene
{

class CVertex
{
public:
	_FLOAT x,y,z;
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
	long m_nVertices;
	CVertex * m_Vertices;	
	Vector<_FLOAT> * m_Normals;
	CPolygonRef * m_PolygonRefs;
	inline CVertex * GetVertices(void) { return m_Vertices; };
	inline CVertex * GetVertex(long i) { return &m_Vertices[i]; };
	inline Vector<_FLOAT> * GetNormals(void) { return m_Normals; };
	inline Vector<_FLOAT> * GetNormal(long i) { return &m_Normals[i]; };
	inline CPolygonRef * GetPoygonRefs(void) { return m_PolygonRefs; };
	inline long GetCount(void) { return m_nVertices; };
	void Setup(void);
	void CleanUp(void);
	CVertices():m_Vertices(NULL),m_Normals(NULL),m_nVertices(0),m_PolygonRefs(NULL){};
	~CVertices() { CleanUp(); };
};

}
