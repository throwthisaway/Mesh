//#ifdef WIN32
#include "stdafx.h"
//#endif
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include <MeshSurface.h>
#include <new>

void _UVMap::Relocate(long size, void * p)
{	
	if (!p)
		return;
	RELOCATEPTR(size, UVS,p,uvs);	
	//((long long)ptr != size)? ptr = new((type*)((long long)base+(long long)ptr)) type: NULL
	RELOCATEPTR(size, DUVS,p,duvs);
	
}