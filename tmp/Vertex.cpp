#include "stdafx.h"
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "Vertex.h"
#include <new>
#include "Log.h"

namespace Scene
{

void CVertices::Setup(void)
{
	try
	{
		m_Normals = new Vector<_FLOAT>[m_nVertices];
		#ifdef WIN32
		ZeroMemory(m_Normals, sizeof(Vector<_FLOAT>) * m_nVertices);
		#else
		memset(m_Normals, 0,  sizeof(Vector<_FLOAT>) * m_nVertices);
		#endif
	}catch ( std::bad_alloc &ba)
	{		
		Log::CLog::Write("CVertices: m_Normals allocation failed: %s\r\n",ba.what());
		throw;
	}
	try
	{
		m_PolygonRefs = new CPolygonRef[m_nVertices];
	}
	catch ( std::bad_alloc &ba)
	{		
		Log::CLog::Write("CVertices: m_PoygonRefs allocation failed: %s\r\n",ba.what());
		throw;
	}
}

void CVertices::CleanUp(void)
{
	DEL_ARRAY(m_Vertices);
	DEL_ARRAY(m_Normals);
	DEL_ARRAY(m_PolygonRefs);
	m_nVertices = 0;
}

}