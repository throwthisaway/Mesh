#include "stdafx.h"
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "Polygon.h"
#include <new>
#include "Log.h"

namespace Scene
{

void CPolygons::Setup(void)
{
	try
	{
		m_Normals = new Vector<float>[m_nPolygons];
	}catch ( std::bad_alloc &ba)
	{		
		Log::CLog::Write("CPolygons: m_Normals allocation failed: %s\r\n",ba.what());
		throw;
	}
}

void CPolygons::CleanUp(void)
{
	DEL_ARRAY(m_Polygons);
	DEL_ARRAY(m_Normals);
}

void CPolygons::SetPointToPolyIndirection(void)
{
	for (long i = 0;i<m_nPolygons;i++)
	{
		//m_Polygons[i].Vertices
	}
}

}
