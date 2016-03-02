#include <windows.h>
#include <lwserver.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwtxtr.h>
#include "serializer\serializer.h"
//#include "linked_list.h"
#include "panel.h"
#include "vmap.h"

static unsigned short nSurfaceID;

static Ser_Base serBase, serBaseUV;


void indirectThroughSurfaces( GlobalFunc *global )
{
	LWSurfaceID surfaceID;
	LWSurfaceFuncs *surff;
	surff = (LWSurfaceFuncs*)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	surfaceID = surff->first();
}
void indirectThroughLayers( GlobalFunc *global )
{
	int maxlayer = 0, i;
	LWObjectFuncs * objfunc;
  objfunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	for (i = 0; i < objfunc->maxLayers(0); i++)
		if (objfunc->layerExists(0,i))
			maxlayer++;
}

#define CHUNKHEADERSIZE sizeof(long)*3

long Tag(const char * szTag)
{
	return TAG(szTag);
}

long WriteTag(FILE *f, const char * szTag,const long nSize,const long nElement) // File must be open...
{
	long tag;
	tag = Tag(szTag);
	fwrite(&tag, sizeof(long), 1, f);
	fwrite(&nSize, sizeof(long), 1, f);
	fwrite(&nElement, sizeof(long), 1, f);
	return ftell(f) - CHUNKHEADERSIZE;
}
void WriteSize(FILE *f, const long pos, const size_t size)
{
	long fpos;
	fpos = ftell(f);
	fseek(f, SEEK_SET, pos);
	fwrite(&size, sizeof(size), 1, f);
	fseek(f, SEEK_SET, fpos);
}

void DumpPoints(const unsigned int nPoint, const EDPointInfo * points, FILE * f)
{
	unsigned int i;
	_FLOAT temp[3];
	if (!f)
		return;
	WriteTag(f, PNTS, nPoint * 3 * sizeof(_FLOAT), nPoint);
	for (i = 0; i < nPoint; i++)
	{
		temp[0] = (_FLOAT)points[i].position[0];
		temp[1] = (_FLOAT)points[i].position[1];
		temp[2] = (_FLOAT)points[i].position[2];
		fwrite((_FLOAT*)temp, sizeof(_FLOAT), 3, f);
	}
}

void CollectPtrImage(Ser_Base * serBase, Image * img)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, img, sizeof(Image));
	serBase->CollectPtr(serNode, (void**)&(img->path));
}

void CollectPtrUVMaps(Ser_Base * serBase, UVMap * uvMap, size_t arrayLength)
{
	size_t i;
	Ser_Node * serNode;
	//Add the UVMap array to the node list
	//serBase->AddSerNode(serBase, uvMap, sizeof(UVMap) * arrayLength);
	for (i = 0;i<arrayLength;i++)
	{
		serNode = serBase->AddSerNode(serBase, &uvMap[i], sizeof(UVMap));
		serBase->CollectPtr(serNode, (void**)&(uvMap[i].uv));		
#ifdef _DEBUG
	uvMap[i].tag = TAG(VMAP);
#endif
	}
}

void CollectPtrSurface(Ser_Base * serBase, Surface * surf, size_t arrayLength)
{
	for (unsigned int i = 0;i<arrayLength;i++)
	{
		Ser_Node * serNode = serBase->AddSerNode(serBase, &surf[i], sizeof(Surface));
		serBase->CollectPtr(serNode, (void**)&(surf[i].surface_infos)); // Only the address needed now...
		serBase->CollectPtr(serNode, (void**)&(surf[i].reflection_map));
		serBase->CollectPtr(serNode, (void**)&(surf[i].refraction_map));
		serBase->CollectPtr(serNode, (void**)&(surf[i].name));
		serBase->CollectPtr(serNode, (void**)&(surf[i].program_name));
		//serBase->CollectPtr(serNode, (void**)&(surf[i].poly_list));
#ifdef _DEBUG
		surf[i].tag = TAG(SRFC);
#endif	
	}
}

void CollectPtrSurfaces(Ser_Base * serBase, Surfaces * surf)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, surf, sizeof(Surfaces));
	serBase->CollectPtr(serNode, (void**)&(surf->m_Surface));	// !!!TEST
}
void CollectPtrSurfaceInfo(Ser_Base *serBase, SurfInfo * surfInfo, size_t arrayLength)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, surfInfo, sizeof(SurfInfo) * arrayLength);
	for (unsigned int i = 0;i<arrayLength;i++)
	{		
		serBase->CollectPtr(serNode, (void**)&(surfInfo[i].layers));
		//serBase->CollectPtr(serNode, (void**)&(surfInfo->next));
#ifdef _DEBUG
		surfInfo[i].tag = TAG(SINF);
#endif	
	}
}

void CollectPtrSurfaceLayer(Ser_Base * serBase, SurfLayer * surfLayer)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, surfLayer, sizeof(SurfLayer));
	serBase->CollectPtr(serNode, (void**)&(surfLayer->image));
	serBase->CollectPtr(serNode, (void**)&(surfLayer->next));
#ifdef _DEBUG
	surfLayer->tag = TAG(SLYR);
#endif	
}

void CollectPtrRefMap(Ser_Base * serBase, RefMap * refMap)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase,refMap, sizeof(RefMap));
	serBase->CollectPtr(serNode, (void**)&(refMap->map));
#ifdef _DEBUG
	refMap->tag = TAG(RMAP);
#endif	
}

const char * lwSurfMapTypes[]=  {SURF_COLR,SURF_LUMI,			SURF_DIFF,	SURF_SPEC,			SURF_GLOS,		 SURF_REFL,			SURF_TRAN,			 SURF_RIND,						SURF_TRNL,			 SURF_BUMP,NULL};
const MAP_TYPE meshMapTypes[]={COLOR_MAP,LUMINOSITY_MAP,DIFFUSE_MAP,SPECULARITY_MAP,GLOSSINESS_MAP,REFLECTION_MAP,TRANSPARENCY_MAP,REFRACTION_INDEX_MAP,TRANSLUCENCY_MAP,BUMP_MAP}; //LW surface map type equvivalents
EDPointInfo * LWPoints;
int cPoint;
EDPolygonInfo * LWPolys;
int cPoly;

void DumpPolygons(MeshEditOp *meshEdit, const EDPolygonInfo * polygons, FILE * f)
{
	long i;
	if (!f)
		return;
	if (cPoly)
	{
		WriteTag(f, POLS, cPoly * VERTICESPERPOLY * sizeof(long), cPoly);
		for (i = 0; i < cPoly; i++)
		{
				EDPointInfo * pointInfo = meshEdit->pointInfo(meshEdit->state, polygons[i].points[ 0 ] );
				fwrite(pointInfo->userData, sizeof(long), 1, f);		
				pointInfo = meshEdit->pointInfo(meshEdit->state, polygons[i].points[ 1 ] );
				fwrite(pointInfo->userData, sizeof(long), 1, f);
				pointInfo = meshEdit->pointInfo(meshEdit->state, polygons[i].points[ 2 ] );
				fwrite(pointInfo->userData, sizeof(long), 1, f);
		}
	}
}

typedef struct
{
	EDPointInfo * points;
	long index;
}PSInfo;

EDError pointScan(PSInfo * psInfo, const EDPointInfo * pointInfo)
{
	EDPointInfo * p;
	memcpy(p = &psInfo->points[cPoint++], pointInfo, sizeof(EDPointInfo));
	*(( int * )( p->userData )) = psInfo->index++;
	return EDERR_NONE;
}
EDError polyScan(EDPolygonInfo * polys, const EDPolygonInfo * polyInfo)
{
	//!!! Only triangles needed...
	if (polyInfo->numPnts == VERTICESPERPOLY)
		memcpy(&polys[cPoly++], polyInfo, sizeof(EDPolygonInfo));
	return EDERR_NONE;
}

void CleanupImage(Image * img)
{
	free(img);
}

void CleanupUVMap(UVMap * uvmap)
{
	if (uvmap)
	{
		free(uvmap->uv);
		uvmap->uv = NULL;
	}
}

void CleanupUVMaps(UVMap ** uvmap, size_t size)
{
	size_t i;
	if(uvmap && *uvmap)
	{
		for (i = 0; i<size; i++)
			CleanupUVMap(&(*uvmap)[i]);
		free(*uvmap);
		*uvmap = NULL;
	}
}

void CleanupSurfLayers(SurfLayer **layer)
{
	while (*layer)
	{
		CleanupSurfLayers(&(*layer)->next);
		CleanupImage((*layer)->image);
		free(*layer);
		*layer=NULL;
	}
}

void CleanupSurfaceInfos(SurfInfo **surface_infos)
{
	for (unsigned int i = 0; i<NUM_MAP_TYPE; i++)
	{
		//CleanupSurfaceInfos(&((*surface_infos)->next));
		CleanupSurfLayers(&((*surface_infos)[i].layers));	
	}	
	free(*surface_infos);
	*surface_infos=NULL;
}
void CleanepRefMap(RefMap ** ref_map)
{
	if (!(*ref_map))
		return;
	if ((*ref_map)->map)
		CleanupImage((*ref_map)->map);
	if (*ref_map)
	{
		free(*ref_map);
		*ref_map = NULL;
	}
}
void CleanupSurface(Surface *surf)
{
	//if (surf->poly_list)
	//	free(surf->poly_list);
	CleanepRefMap(&(surf->reflection_map));
	CleanepRefMap(&(surf->refraction_map));
	CleanupSurfaceInfos(&(surf->surface_infos));
	// TODO:
}

void CleanupSurfaces(Surfaces *surf)
{
	int count = surf->m_nSurfaces;
	while (count)
	{
		CleanupSurface(&surf->m_Surface[--count]);				
	}
	free(surf->m_Surface);
	surf->m_Surface = NULL;
}
int DumpVMap(Ser_Base * serBase, UVMap * vmap, LWID type, int index, MeshEditOp * meshEdit, LWObjectFuncs * objfunc)
{
	_FLOAT * pfData;
	const char * name;	
	long i, j, size;

	// Using global LWPolys, LWPoints, cPoint and cPoly variables
	name =	objfunc->vmapName(type, index);
	// TODO:: change vmap->ID type to __int64 !!!
	vmap->ID = (long)meshEdit->pointVSet(meshEdit->state, NULL, type, name);

	vmap->uv = (_FLOAT*)calloc(cPoly, size = (VERTICESPERPOLY * sizeof(_FLOAT) * (vmap->dim = objfunc->vmapDim(type, index))));
	pfData = vmap->uv;
	vmap->count = 0;
	for (i = 0; i<cPoly; i++)	
		for (j = 0; j<VERTICESPERPOLY; j++)
		{
			meshEdit->pointVEval(meshEdit->state, LWPolys[i].points[j], LWPolys[i].pol, pfData);
			pfData += vmap->dim;
			vmap->count++;
		}	
	serBase->AddSerNode(serBase, vmap->uv, size * cPoly);
	return 0;
}
int DumpVMaps(LWObjectFuncs *objfunc, MeshEditOp * meshEdit, FILE *f)
{
	int res;
	static UVMap * uvMaps;
	long numVMaps, i;
	uvMaps = NULL;
	res = 0;
	if (numVMaps = objfunc->numVMaps(LWVMAP_TXUV))
	{

		uvMaps = (UVMap*)malloc(sizeof(UVMap) * numVMaps);	
		InitSerBase(&serBaseUV);
		CollectPtrUVMaps(&serBaseUV, uvMaps, numVMaps);
		for (i = 0, res = 0; (i < numVMaps) || (res != 0) ; i++)
		{			
			res = DumpVMap(&serBaseUV, &uvMaps[i], LWVMAP_TXUV, i, meshEdit, objfunc);
		}
		if (!res)
		{
			WriteTag(f,VMAP,serBaseUV.RelocatePtrs(&serBaseUV), numVMaps);
			serBaseUV.Serialize(&serBaseUV, f);
			serBaseUV.Release(&serBaseUV);
		}
		CleanupUVMaps(&uvMaps, numVMaps);
	}
	return res;
}
int DumpImage(Ser_Base * serBase, GlobalFunc * global, Image ** image, LWImageID imgID)
{
	Image * img;
	//
	LWImageList *imglist;
	if (!imgID)
		return 0;
	imglist = (LWImageList*)global( LWIMAGELIST_GLOBAL, GFUSE_TRANSIENT );
	
	img = (Image*)calloc(1, sizeof(Image));
	if (!img)
	{
		// ERROR HANDLING: Can't allocate...
		return -1;
	}
	img->type = STILL_IMAGE;
	img->path = (char*)imglist->filename(imgID, 0);
	*image = img;
	CollectPtrImage(serBase, img);
	serBase->AddSerNode(serBase,img->path, strlen(img->path) + 1);
	return 0;
}

int WriteSurfaceLayer(Ser_Base * serBase, SurfLayer ** pSurfLayer, LWTextureID txtID, GlobalFunc * global, MeshEditOp * meshEdit/* for UV enumeration*/)
{
	SurfLayer ** surfLayer = pSurfLayer;
	LAYER_TYPE layerType;
	int nData;
	//
	LWTextureFuncs *txfunc;
	LWTLayerID layerID;
	LWID vmapID;
	LWImageID imgID;

	txfunc = (LWTextureFuncs*)global( LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT );
	// Enumerate through texture layers...
	layerID = txfunc->firstLayer(txtID);
	while (layerID)
	{
		// Layer type...
		switch (nData = txfunc->layerType(layerID))
		{
		case 0:			// Image
			layerType = IMAGE_LAYER;
			break;
			// TODO:
		case 1:// Procedural
		case 2:// Gradient
			layerID = txfunc->nextLayer(txtID,layerID);
			continue;
			// Currently it makes no sense...
		}
		*surfLayer = (SurfLayer*)calloc(1,sizeof(SurfLayer));
		if (!*surfLayer)
		{
			// ERROR HANDLING: Can't allocate...
			return -1;
		}
		(*surfLayer)->type = layerType;
		// Blend mode...
		txfunc->getParam(layerID,TXTAG_BLEND,&nData);
		switch (nData)
		{
		case TXBLN_NORMAL:
			(*surfLayer)->blend_mode = NORMAL_BLEND;
			break;
		case TXBLN_SUBTRACT:
			(*surfLayer)->blend_mode = SUBSTRACTIVE_BLEND;
			break;
		case TXBLN_DIFFERENCE:
			(*surfLayer)->blend_mode = DIFFERENCE_BLEND;
			break;
		case TXBLN_MULTIPLY:
			(*surfLayer)->blend_mode = MULTIPLY_BLEND;
			break;
		case TXBLN_DIVIDE:
			(*surfLayer)->blend_mode = DIVIDE_BLEND;
			break;
		case TXBLN_ALPHA:
			(*surfLayer)->blend_mode = ALPHA_BLEND;
			break;
		case TXBLN_DISPLACE:
			(*surfLayer)->blend_mode = TEXTURE_DISPLACEMENT_BLEND;
			break;
		case TXBLN_ADD:
			(*surfLayer)->blend_mode = ADDITIVE_BLEND;
			break;
		}		
		// Projection...
		txfunc->getParam(layerID,TXTAG_PROJ,&nData);
		switch (nData)
		{
			// We try to convert them to UVMap by evaluateUV(...)
		case TXPRJ_PLANAR:
			(*surfLayer)->projection = PLANAR_PROJECTION;
			break;
		case TXPRJ_CYLINDRICAL:
			(*surfLayer)->projection = CYLINDERAL_PROJECTION;
			break;
		case TXPRJ_SPHERICAL:
			(*surfLayer)->projection = SPHERICAL_PROJECTION;
			break;
		case TXPRJ_CUBIC:
			(*surfLayer)->projection = CUBIC_PROJECTION;
			break;
		case TXPRJ_FRONT:
			(*surfLayer)->projection = FRONT_PROJECTION;
			break;
		case TXPRJ_UVMAP:
		default:  /* 'cause we convert everything... */
			(*surfLayer)->projection = UV_PROJECTION;
			break;
		}
		// TODO: If projection is anything else than UV mapping, try to evalute it to UV coorinates...
		// TODO: txfunc->evaluateUV(  
		// If projection is UV:
		

		txfunc->getParam(layerID,TXTAG_VMAP,&vmapID);
		
		(*surfLayer)->uvmap	= vmapID;		
		
		// Get the VMAP name...
		//vmapName = objfunc->vmapName(LWVMAP_TXUV,vmapID);// We handle texture UV coordinates only...
		//vmapName = objfunc->vmapName(LWVMAP_TXUV,0);// vmapID <==> index ????

		// If projection is something else, we embed the UV coordinates, got using evaluateUV:
		
		// Get the texture ID
		txfunc->getParam(layerID,TXTAG_IMAGE,&imgID);
		DumpImage(serBase, global, &((*surfLayer)->image), imgID);
		//(*surfLayer)->image = (Image*)imgID; //temporary
		CollectPtrSurfaceLayer(serBase, *surfLayer);
		surfLayer = &((*surfLayer)->next);
		// Next text. layer...
		layerID = txfunc->nextLayer(txtID,layerID);
	}
	return 0;
}

int WriteSurfaceInfos(Ser_Base *serBase, SurfInfo ** pSurfInfo, GlobalFunc *global, MeshEditOp * meshEdit/* for UV enumeration*/, const LWSurfaceFuncs *surff, const LWSurfaceID *surfID)
{
	const char ** p;
	SurfInfo ** surfInfo = pSurfInfo;
	MAP_TYPE * pMap_Type;
	double * pfData;
	
	//
	LWTextureID txtID;
	if (!(*surfInfo = (SurfInfo*)calloc(NUM_MAP_TYPE,sizeof(SurfInfo))))
	{
		// ERROR HANDLING: Can't allocate...
		return -1;
	}
	SurfInfo * pTempSurfInfo = *surfInfo;
	for (p = lwSurfMapTypes, pMap_Type=(MAP_TYPE*)meshMapTypes; *p!=NULL;p++,pMap_Type++)
	{		
		// !!!pfData=surff->getFlt( surfID, *p );
		
		pfData=surff->getFlt((LWSurfaceID) *surfID, *p );	// Amount
		pTempSurfInfo->type=*pMap_Type;		
		pTempSurfInfo->val=(_FLOAT)*pfData;				
		if ((*pfData != 0.0F) &&(txtID = surff->getTex(*surfID,*p)))	
			WriteSurfaceLayer(serBase, &(pTempSurfInfo->layers), txtID, global, meshEdit);
		pTempSurfInfo++;
	}
	CollectPtrSurfaceInfo(serBase, *surfInfo, NUM_MAP_TYPE);
	return 0;
}
MeshExport meshExport;
void DumpSurfaces(Ser_Base *serBase, Surface *surf, GlobalFunc *global, MeshEditOp * meshEdit/* for UV enumeration*/, const LWSurfaceFuncs *surff, const LWSurfaceID *surfID, const unsigned int nPoly, const EDPolygonInfo * polygons, const unsigned int nPoint, const EDPointInfo * points)
{
	double * pfData;	
	int temp;

	pfData = surff->getFlt(*surfID,SURF_COLR);
	surf->color[0] = (_FLOAT)pfData[0];	// Indexing must be ok, TODO: check...
	surf->color[1] = (_FLOAT)pfData[1];
	surf->color[2] = (_FLOAT)pfData[2];
	pfData = surff->getFlt(*surfID,SURF_SMAN);
	surf->smoothing = (_FLOAT)*pfData;//(unsigned char)((pfData = surff->getFlt(*surfID,SURF_SMAN)) <= 0) ? 0:1; //Smoothing?
	
	surf->sidedness = ((temp = surff->getInt(*surfID, SURF_SIDE)) == 3) ? DOUBLE_SIDED:SINGLE_SIDED;
	surf->name = surff->name(*surfID);
	serBase->AddSerNode(serBase,(void*)surf->name, strlen(surf->name) + 1);
	// GL specific
	unsigned int i = 0;
	while (meshExport.surfIDList[i] != NULL)
	{
		if (meshExport.surfIDList[i] == *surfID)
			break;
		i++;
	}
	if (meshExport.surfIDList[i] == NULL)
		surf->program_name = "simple_texture_normal.program";
	else
		surf->program_name = meshExport.shaders_to_surfIDs[i].c_str();
	serBase->AddSerNode(serBase,(void*)surf->program_name, strlen(surf->program_name) + 1);
	// ...GL specific
	pfData = surff->getFlt(*surfID, SURF_REFL);
	if (*pfData > 0.0)
	{
		surf->reflection_map = (RefMap*)calloc(1, sizeof(RefMap));
		surf->reflection_map->opt = (REF_OPT)surff->getInt(*surfID, SURF_RFOP);
		pfData = surff->getFlt(*surfID, SURF_RBLR);
		surf->reflection_map->blurring = (_FLOAT)*pfData;
		pfData = surff->getFlt(*surfID, SURF_RSAN);
		surf->reflection_map->seam_angle = (_FLOAT)*pfData;
		DumpImage(serBase, global, &(surf->reflection_map->map), surff->getTex(*surfID, SURF_RIMG));
		CollectPtrRefMap(serBase, surf->reflection_map);		
	}
	if (WriteSurfaceInfos(serBase, &(surf->surface_infos), global, meshEdit, surff, surfID) < 0)
	{
		// ERROR HANDLING: Can't allocate...
		CleanupSurface(surf);
		return;
	}
	surf->poly_num = surf->poly_offset = 0;
	//Find...
	while ((surf->poly_offset < nPoly) && (strcmp(surff->name(*surfID), polygons[surf->poly_offset].surface) != 0)) surf->poly_offset++;
	//Count
	surf->poly_num  = surf->poly_offset;
	while ((surf->poly_num < nPoly) && (strcmp(surff->name(*surfID), polygons[surf->poly_num].surface) == 0)) surf->poly_num++;
	surf->poly_num -= surf->poly_offset;

	//nElementNo = surf->poly_num = 0;
	//for (i = 0; i < nPoly; i++)
	//{
	//	if ((strcmp(surff->name(*surfID), polygons[i].surface) == 0) && (polygons[i].numPnts == 3)) // Only triangles needed...
	//	{
	//		surf->poly_num++;
	//	}
	//}
	//surf->poly_list = calloc(surf->poly_num, sizeof(unsigned int));
	//if (!surf->poly_list)
	//{
	//	// ERROR HANDLING: Can't allocate...
	//	CleanupSurface(surf);
	//	return;
	//}
	//for (i = 0; i < nPoly/*surf->poly_num*/; i++)
	//{
	//	if ((strcmp(surff->name(*surfID), polygons[i].surface) == 0) && (polygons[i].numPnts == 3)) // Only triangles needed...
	//	{
	//		surf->poly_list[nElementNo++] = i;
	//	}
	//}

	//if (nElementNo)
	//	serBase->AddSerNode(serBase, surf->poly_list, nElementNo * sizeof(surf->poly_list[0]));	
}

FILE * DumpHeader(const char *fname)
{
	FILE *f;
	f = fopen(fname, "wb"); 
	//TODO:
	return f;
}

int CompareBySurfaceID(const EDPolygonInfo * p1, const EDPolygonInfo * p2)
{
	return strcmp(p1->surface, p2->surface);
}

void SortPolygonsBySurfIDs(const EDPolygonInfo * polygons, const long count)
{

	qsort((void*)polygons, count, sizeof(EDPolygonInfo), (int (__cdecl *)(const void *,const void *))CompareBySurfaceID);	
}
extern "C" void SaveMesh(GlobalFunc *global, LWModCommand *local, EltOpLayer oplayer, int selmode)
{
	FILE * f;
	EDError err;
	unsigned int nPoint, nPoly;
	long i;
	PSInfo psInfo;

	LWSurfaceFuncs *surff;
	LWStateQueryFuncs *query;
	MeshEditOp * meshEdit;
	char cmd[ 128 ];

	if (ShowPanel(global, &meshExport))
	{
		if (meshExport.convert_to_triangles)
		{
			sprintf( cmd, "TRIPLE");
			local->evaluate( local->data, cmd );	
		}
		// For enumerating vertex maps and dump them...
		LWObjectFuncs *objfunc;
		objfunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
		// Initialize variables
		nSurfaceID = 0;
		// Initialize variables
		f = DumpHeader(meshExport.output_file_name);
		meshEdit = local->editBegin(0, 0, OPSEL_GLOBAL);
		nPoint = meshEdit->pointCount(meshEdit->state, oplayer, selmode);
		nPoly = meshEdit->polyCount(meshEdit->state, oplayer, selmode);
		// Enumerate points
		LWPoints = (EDPointInfo*)calloc(nPoint, sizeof(EDPointInfo)); cPoint = 0;	
		psInfo.points = LWPoints;
		psInfo.index = 0;
		err = meshEdit->pointScan(meshEdit->state, (EDPointScanFunc(cdecl *))pointScan, &psInfo, oplayer);
		// Enumerate polygons
		LWPolys = (EDPolygonInfo*)calloc(nPoly, sizeof(EDPolygonInfo)); cPoly = 0;
		// !!! polyScan callback fct. exports only triangles
		err = meshEdit->polyScan(meshEdit->state, (EDPolyScanFunc(cdecl *))polyScan, LWPolys, oplayer);
		DumpPoints(nPoint, LWPoints, f);


		// TODO:
		SortPolygonsBySurfIDs(LWPolys, nPoly);

		DumpPolygons(meshEdit, LWPolys, f);
		// Backward compatibility hack:
		surff = NULL;
		if (!surff)
			surff = (LWSurfaceFuncs*)global( "Surface Functions 2", GFUSE_TRANSIENT );
		// UVMaps
		DumpVMaps(objfunc, meshEdit, f);
		query = (LWStateQueryFuncs*)global( LWSTATEQUERYFUNCS_GLOBAL, GFUSE_TRANSIENT );
		// TODO:: Finish it off...
		//_DumpVMaps(objfunc, query, meshEdit, oplayer, f);
#ifdef _DEBUG
		fflush(f);
#endif
		// Surfaces
		InitSerBase(&serBase);
		//i = GetSurfIDList(&meshExport);	
		meshExport.surf.m_Surface = (Surface*)calloc(meshExport.surf.m_nSurfaces, sizeof(Surface));
		// TODO: if !surf.surf...
		CollectPtrSurface(&serBase, meshExport.surf.m_Surface, meshExport.surf.m_nSurfaces);
		i = 0;	
		while (meshExport.surfIDList[i] != NULL)
		{	
			DumpSurfaces(&serBase, &meshExport.surf.m_Surface[i], global, meshEdit, surff, &meshExport.surfIDList[i], nPoly, LWPolys, nPoint, LWPoints);
			i++;		
		}	
		WriteTag(f,SURF,serBase.RelocatePtrs(&serBase),i);
		serBase.Serialize(&serBase, f);
		serBase.Release(&serBase);	
		fclose(f);
		CleanupSurfaces(&meshExport.surf);	
		CleanupMeshExport();
		meshEdit->done(meshEdit->state, EDERR_NONE, 0);

		free(LWPoints);
		free(LWPolys);		
	}
	
}

