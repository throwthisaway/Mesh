#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwmeshedt.h>
#include "serializer\serializer.h"
#include "meshsurface.h"
#include "vmap.h"


void _CleanupUVMap(_UVMap * uvmap)
{
	if (uvmap)
	{
		free(uvmap->uvs);
		free(uvmap->duvs);
	}
}

void _CleanupUVMaps(_UVMap ** uvmaps, size_t size)
{
	size_t i;
	if(uvmaps && *uvmaps)
	{
		for (i = 0; i<size; i++)
			_CleanupUVMap(&(*uvmaps)[i]);
		free(*uvmaps);
		*uvmaps = NULL;
	}
}

void _CollectPtrUVMaps(Ser_Base * serBase, _UVMap * uvMap, size_t arrayLength)
{
	size_t i;
	Ser_Node * serNode;
	//Add the UVMap array to the node list
	//serBase->AddSerNode(serBase, uvMap, sizeof(_UVMap) * arrayLength);
	for (i = 0;i<arrayLength;i++)
	{
		serNode = serBase->AddSerNode(serBase, &uvMap[i], sizeof(_UVMap));
		serBase->CollectPtr(serNode, (void**) &(uvMap[i].uvs));
		serBase->CollectPtr(serNode, (void**) &(uvMap[i].duvs));
		serBase->AddSerNode(serBase, uvMap[i].uvs, sizeof(UVS) * uvMap[i].perpoint_count);
		serBase->AddSerNode(serBase, uvMap[i].duvs, sizeof(DUVS) * uvMap[i].perpoly_count);
#ifdef _DEBUG
	uvMap[i].tag = TAG(VMAP);
#endif
	}
	// Discontinous VMap information
	//	Ser_Node * serNode = serBase->AddSerNode(serBase, dvMap, sizeof(_DVMap));
//	serBase->CollectPtr(serNode, (void**) &(dvMap->point));
//	serBase->CollectPtr(serNode, (void**) &(dvMap->map));
//#ifdef _DEBUG
//	dvMap->tag = TAG(DVMA);
//#endif
}
long WriteTag(FILE *f, const char * szTag,const long nSize,const long nElement);
extern EDPointInfo * LWPoints;
extern int cPoint;
extern EDPolygonInfo * LWPolys;
extern int cPoly;
static _UVMap * uvmaps = NULL;

void _DumpVMaps(LWObjectFuncs *objfunc, LWStateQueryFuncs *query, MeshEditOp * meshEdit, EltOpLayer oplayer, FILE * f)
{
	long nvmaps;
	_UVMap * current;
	unsigned int i, j;
	const char * vmapName;
	Ser_Base serBase;
	size_t vmapsize, temp;
	float pfData[2];
	
	LWMeshInfo *layerMesh;
	layerMesh = objfunc->layerMesh(CURRENT_OBJECT/*if there is*/, oplayer);
	if (!(nvmaps = objfunc->numVMaps(LWVMAP_TXUV)))
		return;
	
	uvmaps = (_UVMap*)calloc(nvmaps, sizeof(_UVMap));
	vmapsize = nvmaps * sizeof(_UVMap);
	InitSerBase(&serBase);
	_CollectPtrUVMaps(&serBase, uvmaps, nvmaps);
	for (i=0; i<nvmaps; i++)
	{
		current = &uvmaps[i];
		if (!(current->ID = (long)layerMesh->pntVLookup(layerMesh, LWVMAP_TXUV, vmapName = objfunc->vmapName(LWVMAP_TXUV, i)))) continue; //Get the VMap name
		// Dump per-point vmap
		layerMesh->pntVSelect((LWMeshInfoID)layerMesh, (void*)current->ID);		
		//current->ID = meshEdit->pointVSet(meshEdit->state, NULL, LWVMAP_TXUV, vmapName);
		// Count per-point uv coords...
		for (j = 0, current->perpoint_count = 0; j<cPoint; j++)
		{
			if (layerMesh->pntVGet(layerMesh, LWPoints[j].pnt, pfData)) current->perpoint_count++;
		}
		if (current->perpoint_count)
		{			
			current->uvs = (UVS*)malloc(temp = sizeof(UVS) * current->perpoint_count);
			//serBase->AddSerNode(serBase, uvMap[i].uvs, sizeof(struct st_uv) * uvMap[i].perpoint_count);
			vmapsize += temp;
			for (j = 0, current->perpoint_count = 0; j<cPoint; j++)
			{
 				if (!layerMesh->pntVGet(layerMesh, LWPoints[j].pnt, (float*)&current->uvs[current->perpoint_count].uv))
					continue;				
				current->uvs[current->perpoint_count].point = *(( int * )( LWPoints[j].userData ));
				current->perpoint_count++;
			}
		}
	    // Count per-poly uv coords...
		for (j = 0, current->perpoly_count = 0; j<cPoly; j++)
		{
			for (int k = 0; k<LWPolys[j].numPnts;k++)
			{
				if (layerMesh->pntVPGet(layerMesh, LWPolys[j].points[k], LWPolys[j].pol, pfData)) current->perpoly_count++;
			}
		}
		if (current->perpoly_count)
		{			
			current->duvs = (DUVS*)malloc(temp = sizeof(DUVS) * current->perpoly_count);
			//serBase->AddSerNode(serBase, uvMap[i].uvs, sizeof(struct st_uv) * uvMap[i].perpoint_count);
			vmapsize += temp;
			for (j = 0, current->perpoly_count = 0; j<cPoly; j++)
			{
				for (int k = 0; k<LWPolys[j].numPnts;k++)
				{
 					if (!layerMesh->pntVPGet(layerMesh, LWPolys[j].points[k], LWPolys[j].pol, (float*)&current->duvs[current->perpoly_count].uv))
						continue;				
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
					//TODO: LWPolys[j].points[k] in LWPoints[x].pnt =>LWPoints[x].userData
					current->duvs[current->perpoly_count].point = k; // TODO: test k-th vertex????
					current->duvs[current->perpoly_count].poly = j;
					current->perpoly_count++;
				}
			}
		}
	}
	// TODO: discontinous uv maps...
	/*current->duvmap = NULL;	
	_CollectPtrUVMaps(&serBase,*/
	WriteTag(f, "VMP2", vmapsize, nvmaps);
	serBase.Serialize(&serBase, f);
	serBase.Release(&serBase);
	_CleanupUVMaps(&uvmaps, nvmaps);
//	if (layerMesh->destroy)
//		layerMesh->destroy(layerMesh);
///*		cpolys = 0; vmapsize = 0;			
//		for (j=0; j<cPoly; j++)
//		{
//			for (k=0, npointhasuvperpoly = 0; k<3; k++)
//			{
//					// !!! Maybe PointLookUp() fct. needed here for point enumeration...
//					if (layerMesh->pntVPGet(layerMesh, LWPolys[j].points[k], &LWPolys[j], &pfData))	//???
//						npointhasuvperpoly++;
//			}
//			if (npointhasuvperpoly)
//			{
//				vmapsize+=WriteInt(f, j);
//				cpolys++;	
//			}
//			for (k=0; k<3; k++)
//			{
//					// !!! Maybe PointLookUp() fct. needed here for point enumeration...
//					if (!layerMesh->pntVPGet(layerMesh, LWPolys[j].points[k], &LWPolys[j],&pfData)) //???
//						continue;
//					vmapsize+=WriteUChar(f, k|npointhasuvperpoly<<4);	// The actual per-poly point index on the lower 4 bit, and the count of points per poly having UV.
//					vmapsize+=Write_FLOAT(f, *pfData);	// U
//					pfData++;
//					vmapsize+=Write_FLOAT(f, *pfData); // V
//			}
//		}
//	}
//	// Chunk size: VMap name + 1 (\0) + 1 byte VMap type (0x00) + (cpolys*sizeof(int) + 2 * sizeof(_FLOAT)+sizeof(unsigned char) * cpolys (UV coordinates)) * 3
//	WriteSize(f, vmapSizePos, strlen(vmapName)+1+vmapsize);
//	// Hack: logical size
//	WriteSize(f, vmapSizePos + sizeof(unsigned int), cpolys);*/
}