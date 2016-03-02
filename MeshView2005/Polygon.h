#pragma once
#include "HelperMacros.h"
#include "VectorMath.h"
#include <MeshSurface.h>

namespace Scene
{

class CPolygon
{
public:
	long Vertices[VERTICESPERPOLY];
	//Vector<float><float> normal;
};

class CPolygons
{
public:
	long m_nPolygons;
	CPolygon * m_Polygons;
	Vector<float> * m_Normals;
	inline CPolygon * GetPolygons(void) { return m_Polygons; };
	inline CPolygon * GetPolygon(long i) {return &m_Polygons[i]; };
	inline Vector<float> * GetNormals(void) { return m_Normals; };
	inline Vector<float> * GetNormal(long i) { return &m_Normals[i]; };
	inline long GetCount(void) { return m_nPolygons; };
	void SetPointToPolyIndirection(void);
	void Setup(void);
	void CleanUp(void);
	CPolygons() : m_Polygons(NULL), m_Normals(NULL), m_nPolygons(0) {};
	~CPolygons() { CleanUp(); };
};

}