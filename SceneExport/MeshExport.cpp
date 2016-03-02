#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwtxtr.h>
#include "MeshExport.h"
#include "mesh_panel.h"
#include "Path.h"
#include <algorithm>
#include "HelperMacros.h"

//////////////////////////////////////////////
// globals...
#define CHUNKHEADERSIZE sizeof(long)*3
const char * lwSurfMapTypes[]=  {SURF_COLR,SURF_LUMI,			SURF_DIFF,	SURF_SPEC,			SURF_GLOS,		 SURF_REFL,			SURF_TRAN,			 SURF_RIND,						SURF_TRNL,			 SURF_BUMP,NULL};
const MAP_TYPE meshMapTypes[]={COLOR_MAP,LUMINOSITY_MAP,DIFFUSE_MAP,SPECULARITY_MAP,GLOSSINESS_MAP,REFLECTION_MAP,TRANSPARENCY_MAP,REFRACTION_INDEX_MAP,TRANSLUCENCY_MAP,BUMP_MAP}; //LW surface map type equvivalents
const std::string CMeshExport::defPrg("Default.program");
int cmpVert(const void * v1, const void * v2)
{
	// TODO:: point index must be smaller than 2^31 !!!
	return (int)((Vertex*)v1)->id - (int)((Vertex*)v2)->id;
}

int cmpVert2(const void * v1, const void * v2)
{
	// TODO:: point index must be smaller than 2^31 !!!
	return (int)v1 - (int)((Vertex*)v2)->id;
}

size_t pointScan(void * data, LWPntID pntID)
{	
	CMeshExport * meshExport = (CMeshExport*)data;
	LWFVector vector;
	meshExport->meshInfo->pntBasePos(meshExport->meshInfo, pntID, vector);
	Vertex * v = &meshExport->vertices[meshExport->cVertex++];
	v->pos[0] = vector[0];
	v->pos[1] = vector[1];
	v->pos[2] = -vector[2];
	v->id = pntID;
	return 0;
}

long pointIndex(Vertex * vertices, LWPntID pntID, long cVertex)
{
	return (Vertex *)bsearch(pntID, vertices, cVertex, sizeof(Vertex), cmpVert2)-vertices;
}

int cmpPoly(const void * p1, const void * p2)
{
	return strcmp(((Poly*)p1)->surf, ((Poly*)p2)->surf);
}

size_t polygonScan(void * data, LWPolID polID)
{
	CMeshExport * meshExport = (CMeshExport*)data;
	LWMeshInfo * meshInfo = meshExport->meshInfo;
	if (meshInfo->polType(meshInfo, polID) != LWPOLTYPE_FACE)
		return 0;
	if (meshInfo->polSize(meshInfo, polID) == VERTICESPERPOLY) {
		long v1, v2, v3;
		LWPntID pntID = meshInfo->polVertex(meshInfo, polID, 0);
		v3 = pointIndex(meshExport->vertices, pntID, meshExport->cVertex);
		pntID = meshInfo->polVertex(meshInfo, polID, 1);
		v2 = pointIndex(meshExport->vertices, pntID, meshExport->cVertex);
		pntID = meshInfo->polVertex(meshInfo, polID, 2);
		v1 = pointIndex(meshExport->vertices, pntID, meshExport->cVertex);
		auto surf = meshInfo->polTag(meshInfo, polID, LWPTAG_SURF);
		meshExport->polygons.push_back({ v1, v2, v3, polID, surf });
	}
	else if (meshInfo->polSize(meshInfo, polID) == VERTICESPERLINE) {
		long v1, v2;
		LWPntID pntID = meshInfo->polVertex(meshInfo, polID, 0);
		v1 = pointIndex(meshExport->vertices, pntID, meshExport->cVertex);
		pntID = meshInfo->polVertex(meshInfo, polID, 1);
		v2 = pointIndex(meshExport->vertices, pntID, meshExport->cVertex);
		const char* surf = meshInfo->polTag(meshInfo, polID, LWPTAG_SURF);
		meshExport->lines.push_back({v1, v2, polID, surf});
	}
	return 0;
}

int cmpUV(const void * p1, const void * p2)
{
	// TODO:: point index must be smaller than 2^31 !!!
	return ((UV*)p1)->point - ((UV*)p2)->point;
}

int cmpDV(const void * p1, const void * p2)
{
	// TODO:: point index must be smaller than 2^31 !!!
	return ((DV*)p1)->poly - ((DV*)p2)->poly;
}
// ...globals
//////////////////////////////////////////////

void CollectPtrImage(Ser_Base * serBase, Image * img)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, img, sizeof(Image));
	serBase->CollectPtr(serNode, (void**)&(img->path));
}

void CollectPtrUVMaps(Ser_Base * serBase, _UVMap * uvMap, size_t arrayLength)
{
	size_t i;
	Ser_Node * serNode;
	//Add the UVMap array to the node list
	//serBase->AddSerNode(serBase, uvMap, sizeof(UVMap) * arrayLength);
	for (i = 0;i<arrayLength;i++)
	{
		serNode = serBase->AddSerNode(serBase, &uvMap[i], sizeof(_UVMap));
		serBase->CollectPtr(serNode, (void**)&(uvMap[i].uv));		
#ifdef _TAG
		uvMap[i].tag = TAG(VMP2);
#endif
	}
}

void CollectPtrDVMaps(Ser_Base * serBase, _DVMap * dvMap, size_t arrayLength)
{
	size_t i;
	Ser_Node * serNode;
	for (i = 0;i<arrayLength;i++)
	{
		serNode = serBase->AddSerNode(serBase, &dvMap[i], sizeof(_DVMap));
		serBase->CollectPtr(serNode, (void**)&(dvMap[i].dv));		
#ifdef _TAG
		dvMap[i].tag = TAG(DVMP);
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
#ifdef _TAG
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
#ifdef _TAG
		surfInfo[i].tag = TAG(SINF);
#endif	
	}
}

void CollectPtrSurfaceLayer(Ser_Base * serBase, SurfLayer * surfLayer)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase, surfLayer, sizeof(SurfLayer));
	serBase->CollectPtr(serNode, (void**)&(surfLayer->image));
	serBase->CollectPtr(serNode, (void**)&(surfLayer->next));
#ifdef _TAG
	surfLayer->tag = TAG(SLYR);
#endif	
}

void CollectPtrRefMap(Ser_Base * serBase, RefMap * refMap)
{
	Ser_Node * serNode = serBase->AddSerNode(serBase,refMap, sizeof(RefMap));
	serBase->CollectPtr(serNode, (void**)&(refMap->map));
#ifdef _TAG
	refMap->tag = TAG(RMAP);
#endif	
}

void CleanupImage(Image * img)
{
	free(img);
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

void CMeshExport::CleanupUVMaps(_UVMap ** uvmap, size_t size)
{	
	if(uvmap && *uvmap)
	{
		DEL_ARRAY((*uvmap)->uv);
		DEL_ARRAY(*uvmap);		
	}
}

void CMeshExport::CleanupDVMaps(_DVMap ** dvmap, size_t size)
{	
	if(dvmap && *dvmap)
	{
		DEL_ARRAY((*dvmap)->dv);
		DEL_ARRAY(*dvmap);
	}
}

int CMeshExport::DumpVMap(int object, _UVMap * uvmap, _DVMap * dvmap, LWID type, int index)
{
	float pfData[2];
	const char * name;	
	LWObjectFuncs * objFunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	void * _uvmap;
	if (objFunc->layerExists(object, 0))
	{
		meshInfo = objFunc->layerMesh(object, 0);
		name =	objFunc->vmapName(type, index);
		_uvmap = meshInfo->pntVLookup( meshInfo, type, name );
		vmapList.push_back((LWID)_uvmap);
	}
	else
		return 0;
	int layer;
	uvmap->nV = dvmap->nV = layer = 0;
	while(objFunc->layerExists(object, layer))
	{
		meshInfo = objFunc->layerMesh(object, layer);
		if (!meshInfo)
			continue;
		meshInfo->pntVSelect(meshInfo, _uvmap);
		for (unsigned long i = 0;i<cVertex;i++)
		{
			if (meshInfo->pntVGet(meshInfo, vertices[i].id, pfData))
				uvmap->nV++;
		}
		for (size_t i = 0; i<polygons.size();i++) {
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v1].id, polygons[i].id, pfData))
				++dvmap->nV;
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v2].id, polygons[i].id, pfData))
				++dvmap->nV;
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v3].id, polygons[i].id, pfData))
				++dvmap->nV;
		}
		if (meshInfo->destroy)
			meshInfo->destroy(meshInfo);
		if (!m_bAllLayers)
			break;
		layer++;
	}
	uvmap->uv = new UV[uvmap->nV];
	dvmap->dv = new DV[dvmap->nV];
	layer = 0;
	UV *pUV = uvmap->uv;
	DV *pDV = dvmap->dv;
	// Get u,v coords
	while(objFunc->layerExists(object, layer))
	{
		meshInfo = objFunc->layerMesh(object, layer);
		if (!meshInfo)
			continue;
		meshInfo->pntVSelect(meshInfo, _uvmap);
		for (unsigned long i=0;i<cVertex;i++)
			if (meshInfo->pntVGet(meshInfo, vertices[i].id, &pUV->u))
			{
				pUV->point = i;
				pUV++;
			}
		for (size_t i = 0; i < polygons.size(); i++) {
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v1].id, polygons[i].id, &pDV->u))
			{
				pDV->point = 0;
				pDV->poly = i;
				pDV++;
			}
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v2].id, polygons[i].id, &pDV->u))
			{
				pDV->point = 1;
				pDV->poly = i;
				pDV++;
			}
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v3].id, polygons[i].id, &pDV->u))
			{
				pDV->point = 2;
				pDV->poly = i;
				pDV++;
			}
		}
		if (meshInfo->destroy)
			meshInfo->destroy(meshInfo);
		if (!m_bAllLayers)
			break;
		layer++;
	}
	qsort(uvmap->uv, uvmap->nV, sizeof(UV), cmpUV);
	qsort(dvmap->dv, dvmap->nV, sizeof(DV), cmpDV);
	serBaseUV.AddSerNode(&serBaseUV, uvmap->uv, uvmap->nV*sizeof(UV));
	serBaseDV.AddSerNode(&serBaseDV, dvmap->dv, dvmap->nV*sizeof(DV));
	return 0;
}

int CMeshExport::DumpVMaps(int object)
{
	int res;
	_UVMap * uvMaps;
	_DVMap * dvMaps;
	long numVMaps, i;
	uvMaps = NULL;
	res = 0;
	LWObjectFuncs * objfunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	if (numVMaps = objfunc->numVMaps(LWVMAP_TXUV))
	{
		uvMaps = new _UVMap[numVMaps];
		dvMaps = new _DVMap[numVMaps];
		InitSerBase(&serBaseUV);		
		CollectPtrUVMaps(&serBaseUV, uvMaps, numVMaps);
		InitSerBase(&serBaseDV);
		CollectPtrDVMaps(&serBaseDV, dvMaps, numVMaps);
		for (i = 0, res = 0; (i < numVMaps) || (res != 0) ; i++)
		{			
			res = DumpVMap(object, &uvMaps[i], &dvMaps[i], LWVMAP_TXUV, i);
		}
		if (!res)
		{
			WriteTag(VMP2,serBaseUV.RelocatePtrs(&serBaseUV), numVMaps);
			serBaseUV.Serialize(&serBaseUV, m_hFile);
			serBaseUV.Release(&serBaseUV);
			WriteTag(DVMP,serBaseDV.RelocatePtrs(&serBaseDV), numVMaps);
			serBaseDV.Serialize(&serBaseDV, m_hFile);
			serBaseDV.Release(&serBaseDV);
		}
		CleanupUVMaps(&uvMaps, numVMaps);
		CleanupDVMaps(&dvMaps, numVMaps);
	}
	return res;
}

int CMeshExport::DumpImage(Image ** image, LWImageID imgID)
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
	CollectPtrImage(&serBase, img);
	serBase.AddSerNode(&serBase,img->path, strlen(img->path) + 1);
	return 0;
}

int CMeshExport::DumpSurfaceLayer(SurfLayer ** pSurfLayer, LWTextureID txtID)
{
	SurfLayer ** surfLayer = pSurfLayer;
	LAYER_TYPE layerType;
	int nData;
	//
	LWTextureFuncs *txfunc;
	LWTLayerID layerID;
	__int64 data;
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
		

		txfunc->getParam(layerID,TXTAG_VMAP, &data);
		unsigned int i=0;
		// TODO:: change (*surfLayer)->uvmap type to size_t
		(*surfLayer)->uvmap = vmapList.size();
		for (std::list<LWID>::const_iterator it = vmapList.begin(); it!=vmapList.end();it++,i++)
		{
            LWID vmapID = (LWID)data;
			if (*it == vmapID)
			{
				(*surfLayer)->uvmap	= i;
				break;
			}
		}	
		
		// Get the VMAP name...
		//vmapName = objfunc->vmapName(LWVMAP_TXUV,vmapID);// We handle texture UV coordinates only...
		//vmapName = objfunc->vmapName(LWVMAP_TXUV,0);// vmapID <==> index ????

		// If projection is something else, we embed the UV coordinates, got using evaluateUV:
		
		// Get the texture ID
		txfunc->getParam(layerID,TXTAG_IMAGE,&imgID);
		DumpImage(&((*surfLayer)->image), imgID);
		//(*surfLayer)->image = (Image*)imgID; //temporary
		CollectPtrSurfaceLayer(&serBase, *surfLayer);
		surfLayer = &((*surfLayer)->next);
		// Next text. layer...
		layerID = txfunc->nextLayer(txtID,layerID);
	}
	return 0;
}

int CMeshExport::DumpSurfaceInfos(SurfInfo ** pSurfInfo, const LWSurfaceID *surfID)
{
	const char ** p;
	SurfInfo ** surfInfo = pSurfInfo;
	MAP_TYPE * pMap_Type;
	double * pfData;
	LWSurfaceFuncs *surff = (LWSurfaceFuncs*)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
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
		pTempSurfInfo->val=(float)*pfData;				
		if ((*pfData != 0.0F) &&(txtID = surff->getTex(*surfID,*p)))	
			DumpSurfaceLayer(&(pTempSurfInfo->layers), txtID);
		pTempSurfInfo++;
	}
	CollectPtrSurfaceInfo(&serBase, *surfInfo, NUM_MAP_TYPE);
	return 0;
}

void CMeshExport::DumpSurfaces(Surface *surf, const LWSurfaceID *surfID, const std::string& prgName)
{
	double * pfData;	
	int temp;
	LWSurfaceFuncs *surff = (LWSurfaceFuncs*)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	pfData = surff->getFlt(*surfID,SURF_COLR);
	surf->color[0] = (float)pfData[0];	// Indexing must be ok, TODO: check...
	surf->color[1] = (float)pfData[1];
	surf->color[2] = (float)pfData[2];
	pfData = surff->getFlt(*surfID,SURF_SMAN);
	surf->smoothing = (float)*pfData;//(unsigned char)((pfData = surff->getFlt(*surfID,SURF_SMAN)) <= 0) ? 0:1; //Smoothing?
	
	surf->sidedness = ((temp = surff->getInt(*surfID, SURF_SIDE)) == 3) ? DOUBLE_SIDED:SINGLE_SIDED;
	surf->name = surff->name(*surfID);
	serBase.AddSerNode(&serBase,(void*)surf->name, strlen(surf->name) + 1);
	// GL specific
	unsigned int i = 0;
	// TODO::
	/*while (meshExport.surfIDList[i] != NULL)
	{
		if (meshExport.surfIDList[i] == *surfID)
			break;
		i++;
	}
	if (meshExport.surfIDList[i] == NULL)
		surf->program_name = "simple_texture_normal.program";
	else
		surf->program_name = meshExport.shaders_to_surfIDs[i].c_str();*/
    surf->program_name = prgName.c_str();
	//surf->program_name = "multilayer.program";
	serBase.AddSerNode(&serBase,(void*)surf->program_name, strlen(surf->program_name) + 1);
	// ...GL specific
	pfData = surff->getFlt(*surfID, SURF_REFL);
	if (*pfData > 0.0)
	{
		surf->reflection_map = (RefMap*)calloc(1, sizeof(RefMap));
		surf->reflection_map->opt = (REF_OPT)surff->getInt(*surfID, SURF_RFOP);
		pfData = surff->getFlt(*surfID, SURF_RBLR);
		surf->reflection_map->blurring = (float)*pfData;
		pfData = surff->getFlt(*surfID, SURF_RSAN);
		surf->reflection_map->seam_angle = (float)*pfData;
		DumpImage(&(surf->reflection_map->map), surff->getTex(*surfID, SURF_RIMG));
		CollectPtrRefMap(&serBase, surf->reflection_map);		
	}
	if (DumpSurfaceInfos(&(surf->surface_infos), surfID) < 0)
	{
		// ERROR HANDLING: Can't allocate...
		CleanupSurface(surf);
		return;
	}
	surf->line_offset = surf->line_count = surf->poly_count = surf->poly_offset = 0;
	//Find...
	while ((surf->poly_offset < polygons.size()) && (strcmp(surff->name(*surfID), polygons[surf->poly_offset].surf) != 0)) ++surf->poly_offset;
	//Count polygons
	surf->poly_count = surf->poly_offset;
	while ((surf->poly_count <  polygons.size()) && (strcmp(surff->name(*surfID), polygons[surf->poly_count].surf) == 0)) ++surf->poly_count;
	surf->poly_count -= surf->poly_offset;
	if (!surf->poly_count)
		surf->poly_offset = 0;
	//Count lines
	while ((surf->line_offset < lines.size()) && (strcmp(surff->name(*surfID), lines[surf->line_offset].surf) != 0)) ++surf->line_offset;
	surf->line_count = surf->line_offset;
	while ((surf->line_count < lines.size()) && (strcmp(surff->name(*surfID), lines[surf->line_count].surf) == 0)) ++surf->line_count;
	surf->line_count -= surf->line_offset;
	if (!surf->line_count)
		surf->line_offset = 0;
}

CMeshExport::CMeshExport(GlobalFunc *global) :
m_hFile(NULL),
m_bAllLayers(TRUE),
m_bShowMeshExportPanel(TRUE)
{
	this->global = global;
}

CMeshExport::~CMeshExport(void)
{
}


void CMeshExport::WriteHeader()
{
	// TODO::
}

long CMeshExport::WriteTag(const char * szTag,const long nSize,const long nElement) // File must be open...
{
	long tag;
	tag = TAG(szTag);
	DWORD written;
	::WriteFile(m_hFile, &tag, sizeof(long),&written, NULL);
	::WriteFile(m_hFile, &nSize, sizeof(long),&written, NULL);
	::WriteFile(m_hFile, &nElement, sizeof(long),&written, NULL);
	return ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT) - CHUNKHEADERSIZE;
}

void CMeshExport::DumpPoints(void)
{
	DWORD written = 0;
	WriteTag(PNTS, cVertex * 3 * sizeof(float), cVertex);
	for (unsigned long i = 0; i < cVertex; i++)
	{
		::WriteFile(m_hFile, vertices[i].pos, sizeof(float) * 3, &written, NULL);
	}
}

void CMeshExport::DumpPolygons(void)
{
	DWORD written = 0;
	WriteTag(POLS, VERTICESPERPOLY * sizeof(long) * polygons.size(), polygons.size());
	for (size_t i = 0; i <  polygons.size(); i++)
	{
		::WriteFile(m_hFile, &polygons[i].v1, sizeof(long), &written, NULL);
		::WriteFile(m_hFile, &polygons[i].v2, sizeof(long), &written, NULL);
		::WriteFile(m_hFile, &polygons[i].v3, sizeof(long), &written, NULL);
	}	
}

void CMeshExport::DumpLines(void)
{
	DWORD written = 0;
	WriteTag(LINE, VERTICESPERLINE * sizeof(long) * lines.size(), lines.size());
	for (size_t i = 0; i < lines.size(); i++)
	{
		::WriteFile(m_hFile, &lines[i].v1, sizeof(long), &written, NULL);
		::WriteFile(m_hFile, &lines[i].v2, sizeof(long), &written, NULL);
	}
}

int CMeshExport::GetSurfIDList()
{
	//Get surfaceID list
	surff = (LWSurfaceFuncs*)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	surfIDList = surff->byObject(name);
	surf.m_nSurfaces = 0;
	while (surfIDList[surf.m_nSurfaces] != NULL) 
    {
        ++surf.m_nSurfaces;	// Count the number of surfaces
        shaders_to_surfIDs.push_back(defPrg);
    }
	return surf.m_nSurfaces;
}

void CMeshExport::Save(LWObjectFuncs * objFunc, int object)
{
	//Get current object	
	obj = object;
	name = objFunc->filename(obj);
	const char * p = strrchr(name, '\\');
	fname = (p)?++p:name;	
	fname.replace(fname.find('.'), std::string::npos, ".mesh");
	if (!m_bShowMeshExportPanel || ShowPanel(this))
	{

		std::string outFName = IO::CPath::GetPath(std::string(name)) + fname;
		m_hFile = ::CreateFileA(outFName.c_str(), GENERIC_WRITE, 0,  NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			// TODO:: MessageBox
			CleanUp();
			return;
		}
		WriteHeader();
		// Get point and polygon count...
		int layer = 0;
		size_t nPolygons = 0;
		nVertices = 0;
		while(objFunc->layerExists(object, layer))
		{
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)
				continue;
			nVertices += meshInfo->numPoints(meshInfo);
			nPolygons += meshInfo->numPolygons(meshInfo);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
			layer++;
		}
		polygons.reserve(nPolygons);
		vertices = new Vertex[nVertices];
		cVertex = 0;
		// Scan points...
		layer = 0;
		while(objFunc->layerExists(object, layer))
		{
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)
				continue;
			meshInfo->scanPoints(meshInfo, pointScan, this);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
			layer++;
		}
		qsort(vertices, cVertex, sizeof(Vertex), cmpVert);
		DumpPoints();
		layer = 0;
		while(objFunc->layerExists(object, layer))
		{
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)
				continue;
			meshInfo->scanPolys(meshInfo, polygonScan, this);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
			layer++;
		}
		std::sort(polygons.begin(), polygons.end(), [](const Poly& p1, const Poly& p2) {
			return strcmp(p1.surf, p2.surf)<0;
		});
		std::sort(lines.begin(), lines.end(), [](const PolyLine& p1, const PolyLine& p2) {
			return strcmp(p1.surf, p2.surf)<0;
		});
		DumpPolygons();
		DumpLines();
		DumpVMaps(object);
		InitSerBase(&serBase);
		GetSurfIDList();
		surf.m_Surface = (Surface*)calloc(surf.m_nSurfaces, sizeof(Surface));
		// TODO: if !surf.surf...
		CollectPtrSurface(&serBase, surf.m_Surface, surf.m_nSurfaces);
		long i = 0;	
		while (surfIDList[i] != NULL)
		{	
			DumpSurfaces(&surf.m_Surface[i], &surfIDList[i], shaders_to_surfIDs[i]);
			i++;		
		}	
		WriteTag(SURF,serBase.RelocatePtrs(&serBase),i);
		serBase.Serialize(&serBase, m_hFile);
		serBase.Release(&serBase);	
		CleanupSurfaces(&surf);
		vmapList.clear();
		DEL_ARRAY(vertices);
		::CloseHandle(m_hFile);
	}
}
void CMeshExport::CleanUp(void)
{
	// TODO::
}