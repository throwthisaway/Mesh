#include "stdafx.h"
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "Vertex.h"
#include <new>
#include "Log.h"

namespace Scene
{

	void CVertices::SetupNormals(void)
	{
		try
		{
			_normals.reserve(_vertices.size());
			_normals.resize(_vertices.size());
			::ZeroMemory(_normals.data(), _normals.size() * sizeof(VectorArr::value_type));
		}catch ( std::bad_alloc &ba)
		{		
			Log::CLog::Write("CVertices::SetupNormals: normal array allocation failed: %s\r\n",ba.what());
			throw;
		}
	}
	void CVertices::Setup(void)
	{
		SetupNormals();
		try
		{
			_polyRefs.reserve(_vertices.size());
			_polyRefs.resize(_vertices.size());
		}
		catch ( std::bad_alloc &ba)
		{		
			Log::CLog::Write("CVertices::Setup polyref allocation failed: %s\r\n",ba.what());
			throw;
		}
	}
	void* CVertices::CreateAndRetrieveInternalData(VertexArr::size_type nVertices)
	{
		try
		{
			_vertices.reserve(nVertices);
			_vertices.resize(nVertices);
			Log::CLog::Write("CVertices::CreateAndRetrieveInternalData: size:%d capacity:%d\r\n", _vertices.size(), _vertices.capacity());
		}catch ( std::bad_alloc &ba)
		{		
			Log::CLog::Write("CVertices::CreateAndRetrieveInternalData: vertex array allocation failed: %s\r\n",ba.what());
			throw;
		}
		return _vertices.data();
	}

	void CVertices::CleanUp(void)
	{
		_vertices.clear();
		_normals.clear();
		_polyRefs.clear();
	}

}