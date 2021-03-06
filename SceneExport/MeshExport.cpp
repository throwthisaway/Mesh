#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwtxtr.h>
#include "MeshExport.h"
#include "mesh_panel.h"
#include "Path.h"
#include <algorithm>
#include "HelperMacros.h"
#include <assert.h>
using namespace MeshLoader;
//////////////////////////////////////////////
// globals...
namespace {
	const char * lwSurfMapTypes[] = { SURF_COLR,SURF_LUMI,			SURF_DIFF,	SURF_SPEC,			SURF_GLOS,		 SURF_REFL,			SURF_TRAN,			 SURF_RIND,						SURF_TRNL,			 SURF_BUMP,NULL };
	const MAP_TYPE meshMapTypes[] = { COLOR_MAP,LUMINOSITY_MAP,DIFFUSE_MAP,SPECULARITY_MAP,GLOSSINESS_MAP,REFLECTION_MAP,TRANSPARENCY_MAP,REFRACTION_INDEX_MAP,TRANSLUCENCY_MAP,BUMP_MAP }; //LW surface map type equvivalents

	//int cmpVert2(const void * v1, const void * v2)
	//{
	//	// TODO:: point index must be smaller than 2^31 !!!
	//	return (int)v1 - (int)((Vertex*)v2)->id;
	//}

	size_t pointScan(void * data, LWPntID pntID)
	{
		CMeshExport * meshExport = (CMeshExport*)data;
		LWFVector vector;
		meshExport->meshInfo->pntBasePos(meshExport->meshInfo, pntID, vector);
		meshExport->vertices.push_back({ { vector[0], vector[1], -vector[2] }, pntID });
		return 0;
	}

	//long pointIndex(Vertex * vertices, LWPntID pntID, long cVertex)
	//{
	//	return (Vertex *)bsearch(pntID, vertices, cVertex, sizeof(Vertex), cmpVert2)-vertices;
	//}
	//
	//int cmpPoly(const void * p1, const void * p2)
	//{
	//	return strcmp(((Poly*)p1)->surf, ((Poly*)p2)->surf);
	//}
	struct PolyScanData {
		LWMeshInfo * meshInfo;
		std::vector<Poly>& lines;
		std::vector<Poly>& polygons;
		std::vector<Vertex>& vertices;
		int& layer;
	};
	// #define CCW
	size_t polygonScan(void * data, LWPolID polID)
	{
		PolyScanData * psd = (PolyScanData*)data;
		LWMeshInfo * meshInfo = psd->meshInfo;
		if (meshInfo->polType(meshInfo, polID) != LWPOLTYPE_FACE)
			return 0;
		auto vCount = meshInfo->polSize(meshInfo, polID);
		if (vCount != VERTICESPERPOLY && vCount != VERTICESPERLINE)
			return 0;
		auto less = [](const Vertex& v1, const LWPntID& id) {
			return v1.id < id;
		};
		LWPntID pntID = meshInfo->polVertex(meshInfo, polID, 0);
		auto it = std::lower_bound(std::begin(psd->vertices), std::end(psd->vertices), pntID, less);
		assert(it != std::end(psd->vertices));
		index_t v1 = (index_t)std::distance(std::begin(psd->vertices), it);
		pntID = meshInfo->polVertex(meshInfo, polID, 1);
		it = std::lower_bound(std::begin(psd->vertices), std::end(psd->vertices), pntID, less);
		assert(it != std::end(psd->vertices));
		index_t v2 = (index_t)std::distance(std::begin(psd->vertices), it);
		const char* surf = meshInfo->polTag(meshInfo, polID, LWPTAG_SURF);
		if (vCount == VERTICESPERLINE) {
			psd->lines.push_back({ v1, v2, 0u, polID, surf, (size_t)psd->layer });
			return 0;
		}
		pntID = meshInfo->polVertex(meshInfo, polID, 2);
		it = std::lower_bound(std::begin(psd->vertices), std::end(psd->vertices), pntID, less);
		assert(it != std::end(psd->vertices));
		index_t v3 = (index_t)std::distance(std::begin(psd->vertices), it);
#ifdef CCW
		psd->polygons.push_back({ v3, v2, v1, polID, surf, (size_t)psd->layer });
#else
		psd->polygons.push_back({ v1, v2, v3, polID, surf, (size_t)psd->layer });
#endif
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
	void CollectPtrLayers(Ser_Base * serBase, MeshLoader::Layer * layers, size_t arrayLength)
	{
		size_t i;
		Ser_Node * serNode;
		for (i = 0; i < arrayLength; i++) {
			serNode = serBase->AddSerNode(serBase, &layers[i], sizeof(MeshLoader::Layer));
			serBase->CollectPtr(serNode, (void**)&(layers[i].poly.sections));
			serBase->CollectPtr(serNode, (void**)&(layers[i].line.sections));
		}
		for (i = 0; i < arrayLength; i++) {
			serBase->AddSerNode(serBase, layers[i].poly.sections, layers[i].poly.count * sizeof(MeshLoader::Layer::Sections::Section));
			serBase->AddSerNode(serBase, layers[i].line.sections, layers[i].line.count * sizeof(MeshLoader::Layer::Sections::Section));
		}
	}

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
		for (i = 0; i < arrayLength; i++)
		{
			serNode = serBase->AddSerNode(serBase, &uvMap[i], sizeof(_UVMap));
			serBase->CollectPtr(serNode, (void**)&(uvMap[i].uv));
#ifdef _TAG
			uvMap[i].tag = Tag(VMP2);
#endif
		}
	}

	void CollectPtrDVMaps(Ser_Base * serBase, _DVMap * dvMap, size_t arrayLength)
	{
		size_t i;
		Ser_Node * serNode;
		for (i = 0; i < arrayLength; i++)
		{
			serNode = serBase->AddSerNode(serBase, &dvMap[i], sizeof(_DVMap));
			serBase->CollectPtr(serNode, (void**)&(dvMap[i].dv));
#ifdef _TAG
			dvMap[i].tag = Tag(DVMP);
#endif
		}
	}

	void CollectPtrSurface(Ser_Base * serBase, Surface * surf, size_t arrayLength)
	{
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			Ser_Node * serNode = serBase->AddSerNode(serBase, &surf[i], sizeof(Surface));
			serBase->CollectPtr(serNode, (void**)&(surf[i].surface_infos)); // Only the address needed now...
			serBase->CollectPtr(serNode, (void**)&(surf[i].reflection_map));
			serBase->CollectPtr(serNode, (void**)&(surf[i].refraction_map));
			serBase->CollectPtr(serNode, (void**)&(surf[i].name));
			serBase->CollectPtr(serNode, (void**)&(surf[i].program_name));
			//serBase->CollectPtr(serNode, (void**)&(surf[i].poly_list));
#ifdef _TAG
			surf[i].tag = Tag(SRFC);
#endif	
		}
	}

	void CollectPtrSurfaces(Ser_Base * serBase, Surfaces * surf)
	{
		Ser_Node * serNode = serBase->AddSerNode(serBase, surf, sizeof(Surfaces));
		serBase->CollectPtr(serNode, (void**)&(surf->surface));	// !!!TEST
	}
	void CollectPtrSurfaceInfo(Ser_Base *serBase, SurfInfo * surfInfo, size_t arrayLength)
	{
		Ser_Node * serNode = serBase->AddSerNode(serBase, surfInfo, sizeof(SurfInfo) * arrayLength);
		for (unsigned int i = 0; i < arrayLength; i++)
		{
			serBase->CollectPtr(serNode, (void**)&(surfInfo[i].layers));
			//serBase->CollectPtr(serNode, (void**)&(surfInfo->next));
#ifdef _TAG
			surfInfo[i].tag = Tag(SINF);
#endif	
		}
	}

	void CollectPtrSurfaceLayer(Ser_Base * serBase, SurfLayer * surfLayer)
	{
		Ser_Node * serNode = serBase->AddSerNode(serBase, surfLayer, sizeof(SurfLayer));
		serBase->CollectPtr(serNode, (void**)&(surfLayer->image));
		serBase->CollectPtr(serNode, (void**)&(surfLayer->next));
#ifdef _TAG
		surfLayer->tag = Tag(SLYR);
#endif	
	}

	void CollectPtrRefMap(Ser_Base * serBase, RefMap * refMap)
	{
		Ser_Node * serNode = serBase->AddSerNode(serBase, refMap, sizeof(RefMap));
		serBase->CollectPtr(serNode, (void**)&(refMap->map));
#ifdef _TAG
		refMap->tag = Tag(RMAP);
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
			*layer = NULL;
		}
	}

	void CleanupSurfaceInfos(SurfInfo **surface_infos)
	{
		for (unsigned int i = 0; i < NUM_MAP_TYPE; i++)
		{
			//CleanupSurfaceInfos(&((*surface_infos)->next));
			CleanupSurfLayers(&((*surface_infos)[i].layers));
		}
		free(*surface_infos);
		*surface_infos = NULL;
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
		int count = surf->count;
		while (count)
		{
			CleanupSurface(&surf->surface[--count]);
		}
		free(surf->surface);
		surf->surface = NULL;
	}
}
const std::string CMeshExport::defPrg("Default.program");
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

bool CMeshExport::CollectVMap(int object, _UVMap * uvmap, _DVMap * dvmap, LWID type, int index) {
	float pfData[2];
	const char * name;	
	LWObjectFuncs * objFunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	LWID _uvmap;
	if (objFunc->layerExists(object, 0)) {
		meshInfo = objFunc->layerMesh(object, 0);
		name =	objFunc->vmapName(type, index);
		_uvmap = reinterpret_cast<LWID>(meshInfo->pntVLookup( meshInfo, type, name ));
		vmapList.push_back(_uvmap);
	}
	else return false;
	uvmap->count = dvmap->count =  0;
	for (int layer = 0; layer < objFunc->maxLayers(object); ++layer) {
		if (!objFunc->layerExists(object, layer)) continue;
		meshInfo = objFunc->layerMesh(object, layer);
		if (!meshInfo)
			continue;
		meshInfo->pntVSelect(meshInfo, reinterpret_cast<void*>(_uvmap));
		for (unsigned long i = 0;i<vertices.size();i++)
		{
			if (meshInfo->pntVGet(meshInfo, vertices[i].id, pfData))
				uvmap->count++;
		}
		for (size_t i = 0; i<polygons.size();i++) {
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v1].id, polygons[i].id, pfData))
				++dvmap->count;
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v2].id, polygons[i].id, pfData))
				++dvmap->count;
			if (meshInfo->pntVPGet(meshInfo, vertices[polygons[i].v3].id, polygons[i].id, pfData))
				++dvmap->count;
		}
		if (meshInfo->destroy)
			meshInfo->destroy(meshInfo);
		if (!m_bAllLayers)
			break;
		layer++;
	}
	uvmap->uv = new UV[uvmap->count];
	dvmap->dv = new DV[dvmap->count];
	UV *pUV = uvmap->uv;
	DV *pDV = dvmap->dv;
	// Get u,v coords
	for (int layer = 0; layer < objFunc->maxLayers(object); ++layer) {
		if (!objFunc->layerExists(object, layer)) continue;
		meshInfo = objFunc->layerMesh(object, layer);
		if (!meshInfo)
			continue;
		meshInfo->pntVSelect(meshInfo, reinterpret_cast<void*>(_uvmap));
		for (unsigned long i=0;i<vertices.size();i++)
			if (meshInfo->pntVGet(meshInfo, vertices[i].id, &pUV->u))
			{
				pUV->point = i;
				pUV++;
			}
		for (index_t i = 0; i < polygons.size(); i++) {
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
	qsort(uvmap->uv, uvmap->count, sizeof(UV), cmpUV);
	qsort(dvmap->dv, dvmap->count, sizeof(DV), cmpDV);
	serBaseUV.AddSerNode(&serBaseUV, uvmap->uv, uvmap->count*sizeof(UV));
	serBaseDV.AddSerNode(&serBaseDV, dvmap->dv, dvmap->count*sizeof(DV));
	return true;
}

void CMeshExport::DumpVMaps(int object) {
	_UVMap * uvMaps;
	_DVMap * dvMaps;
	long numVMaps, i;
	uvMaps = NULL;
	LWObjectFuncs * objfunc = (LWObjectFuncs*)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
	if (numVMaps = objfunc->numVMaps(LWVMAP_TXUV)) {
		uvMaps = new _UVMap[numVMaps];
		dvMaps = new _DVMap[numVMaps];
		InitSerBase(&serBaseUV);
		CollectPtrUVMaps(&serBaseUV, uvMaps, numVMaps);
		InitSerBase(&serBaseDV);
		CollectPtrDVMaps(&serBaseDV, dvMaps, numVMaps);
		for (i = 0; i < numVMaps; i++)
			CollectVMap(object, &uvMaps[i], &dvMaps[i], LWVMAP_TXUV, i);
		auto size = serBaseUV.RelocatePtrs(&serBaseUV);
		WriteTag(VMP2, size, numVMaps);
		serBaseUV.Serialize(&serBaseUV, m_hFile);
		serBaseUV.Release(&serBaseUV);
		Pad(size);
		size = serBaseDV.RelocatePtrs(&serBaseDV);
		WriteTag(DVMP, size, numVMaps);
		serBaseDV.Serialize(&serBaseDV, m_hFile);
		serBaseDV.Release(&serBaseDV);
		Pad(size);
		CleanupUVMaps(&uvMaps, numVMaps);
		CleanupDVMaps(&dvMaps, numVMaps);
	}
}

int CMeshExport::CollectImage(Image ** image, LWImageID imgID)
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

int CMeshExport::CollectSurfaceLayer(SurfLayer ** pSurfLayer, LWTextureID txtID)
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
		(*surfLayer)->uvmap = (index_t)vmapList.size();
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
		CollectImage(&((*surfLayer)->image), imgID);
		//(*surfLayer)->image = (Image*)imgID; //temporary
		CollectPtrSurfaceLayer(&serBase, *surfLayer);
		surfLayer = &((*surfLayer)->next);
		// Next text. layer...
		layerID = txfunc->nextLayer(txtID,layerID);
	}
	return 0;
}

void CollectNodeInputs(NodeID node, LWNodeInputFuncs *nodeInputFuncs) {
	NodeInputID nodeInput = nodeInputFuncs->first(node);
	while (nodeInput) {
		int connected = nodeInputFuncs->check(nodeInput);
		const char * name = nodeInputFuncs->name(nodeInput);
		int i = 0;
		nodeInput = nodeInputFuncs->next(nodeInput);
	}
}

int CMeshExport::CollectSurfaceInfos(SurfInfo ** pSurfInfo, const LWSurfaceID *surfID)
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
			CollectSurfaceLayer(&(pTempSurfInfo->layers), txtID);
		pTempSurfInfo++;
	}

	NodeEditorID nodeEditorID = surff->getNodeEditor(*surfID);
	LWNodeEditorFuncs* nodeEditorFuncs = (LWNodeEditorFuncs*)global(LWNODEEDITORFUNCS_GLOBAL, GFUSE_TRANSIENT);
	LWNodeInputFuncs* nodeInputFuncs = (LWNodeInputFuncs*)global(LWNODEINPUTFUNCS_GLOBAL, GFUSE_TRANSIENT);
	LWNodeOutputFuncs* nodeOutputFuncs = (LWNodeOutputFuncs*)global(LWNODEOUTPUTFUNCS_GLOBAL, GFUSE_TRANSIENT);

	LWNodeFuncs* nodef = (LWNodeFuncs*)global(LWNODEFUNCS_GLOBAL, GFUSE_TRANSIENT);

	if (nodeEditorFuncs->getState(nodeEditorID)/*Enabled*/) {
		NodeID rootNode = nodeEditorFuncs->getRootNodeID(nodeEditorID);
		const char * nodeName = nodef->nodeName(rootNode);
	
		NodeInputID nodeInput = nodeInputFuncs->first(rootNode);
		while (nodeInput) {
			int connected = nodeInputFuncs->check(nodeInput);
			const char * name = nodeInputFuncs->name(nodeInput);
			if (connected) {
				NodeOutputID nodeOutput = nodeInputFuncs->connectedOutput(nodeInput);
				const char* noudOutputName = nodeInputFuncs->name(nodeOutput);
				NodeID node = nodeOutputFuncs->node(nodeOutput);
				CollectNodeInputs(node, nodeInputFuncs);
				const char * nodeName = nodef->nodeName(node);
				int i = 0;
			}
			int i = 0;
			nodeInput = nodeInputFuncs->next(nodeInput);
		}
		//
	}
	CollectPtrSurfaceInfo(&serBase, *surfInfo, NUM_MAP_TYPE);
	return 0;
}

void CMeshExport::CollectSurfaces(Surface *surf, const LWSurfaceID *surfID, const std::string& prgName) {
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
		CollectImage(&(surf->reflection_map->map), surff->getTex(*surfID, SURF_RIMG));
		CollectPtrRefMap(&serBase, surf->reflection_map);		
	}
	if (CollectSurfaceInfos(&(surf->surface_infos), surfID) < 0)
	{
		// ERROR HANDLING: Can't allocate...
		CleanupSurface(surf);
		return;
	}
	//surf->line_offset = surf->line_count = surf->poly_count = surf->poly_offset = 0;
	////Find...
	//while ((surf->poly_offset < polygons.size()) && (strcmp(surff->name(*surfID), polygons[surf->poly_offset].surf) != 0)) ++surf->poly_offset;
	////Count polygons
	//surf->poly_count = surf->poly_offset;
	//while ((surf->poly_count <  polygons.size()) && (strcmp(surff->name(*surfID), polygons[surf->poly_count].surf) == 0)) ++surf->poly_count;
	//surf->poly_count -= surf->poly_offset;
	//if (!surf->poly_count)
	//	surf->poly_offset = 0;
	////Count lines
	//while ((surf->line_offset < lines.size()) && (strcmp(surff->name(*surfID), lines[surf->line_offset].surf) != 0)) ++surf->line_offset;
	//surf->line_count = surf->line_offset;
	//while ((surf->line_count < lines.size()) && (strcmp(surff->name(*surfID), lines[surf->line_count].surf) == 0)) ++surf->line_count;
	//surf->line_count -= surf->line_offset;
	//if (!surf->line_count)
	//	surf->line_offset = 0;
}

CMeshExport::CMeshExport(GlobalFunc *global) :
m_hFile(NULL),
m_bAllLayers(TRUE),
m_bShowMeshExportPanel(TRUE)
{
	this->global = global;
}

CMeshExport::~CMeshExport() = default;

DWORD CMeshExport::Pad(size_t val) {
	DWORD written;
	auto aligned = AlignTo(val, alignment);
	const uint8_t padding[alignment] = {};
	::WriteFile(m_hFile, &padding, aligned - val, &written, nullptr);
	return written;
}
void CMeshExport::WriteHeader() {
	Header header = { Tag("MESH"), 1, alignment, sizeof(tag_t), sizeof(index_t), sizeof(poly_t) };
	DWORD written;
	::WriteFile(m_hFile, &header, sizeof(header), &written, nullptr);
	Pad(sizeof(header));
}

void CMeshExport::WriteTag(const char * tag, size_t size, size_t count) {
	DWORD written;
	Chunk chunk = { Tag(tag), size, count };
	::WriteFile(m_hFile, &chunk, sizeof(chunk), &written, nullptr);
	Pad(sizeof(chunk));
}

void CMeshExport::DumpPoints() {
	if (vertices.empty()) return;
	DWORD written;
	const auto size = vertices.size() * sizeof(vertices.front().pos);
	WriteTag(VERT, size, vertices.size());
	for (auto& v : vertices)
		::WriteFile(m_hFile, v.pos, sizeof(v.pos), &written, nullptr);
	Pad(size);
}

void CMeshExport::DumpPolygons() {
	if (polygons.empty()) return;
	DWORD written;
	const auto size = VERTICESPERPOLY * sizeof(poly_t) * polygons.size();
	WriteTag(POLS, size, polygons.size());
	for (const auto& p : polygons) {
		::WriteFile(m_hFile, &p.v1, sizeof(poly_t), &written, NULL);
		::WriteFile(m_hFile, &p.v2, sizeof(poly_t), &written, NULL);
		::WriteFile(m_hFile, &p.v3, sizeof(poly_t), &written, NULL);
	}
	Pad(size);
}

void CMeshExport::DumpLines() {
	if (lines.empty()) return;
	DWORD written;
	const auto size = VERTICESPERLINE * sizeof(poly_t) * lines.size();
	WriteTag(LINE, size, lines.size());
	for (const auto& l : lines)	{
		::WriteFile(m_hFile, &l.v1, sizeof(poly_t), &written, NULL);
		::WriteFile(m_hFile, &l.v2, sizeof(poly_t), &written, NULL);
	}
	Pad(size);
}
void CMeshExport::DumpLayers(MeshLoader::Layer *layers, size_t count) {
	//DWORD written = 0;
	//size_t size = 0;
	//for (auto& l : layers) {
	//	size += sizeof(l.pivot);
	//	size += sizeof(size_t);
	//	size += l.lineSections.size() * sizeof(l.lineSections);
	//	size += sizeof(size_t);
	//	size += l.polySections.size() * sizeof(l.polySections);
	//}
	//WriteTag(LAYR, size, layers.size());
	//for (auto& l : layers)
	//{
	//	::WriteFile(m_hFile, &l.pivot, sizeof(l.pivot), &written, NULL);
	//	size = l.polySections.size();
	//	::WriteFile(m_hFile, &size, sizeof(size_t), &written, NULL);
	//	::WriteFile(m_hFile, &l.polySections.front(), sizeof(l.polySections) * l.polySections.size(), &written, NULL);
	//	size = l.lineSections.size();
	//	::WriteFile(m_hFile, &size, sizeof(size_t), &written, NULL);
	//	::WriteFile(m_hFile, &l.lineSections.front(), sizeof(l.lineSections) * l.lineSections.size(), &written, NULL);
	//}
	if (!count) return;
	Ser_Base serBase;
	InitSerBase(&serBase);
	CollectPtrLayers(&serBase, layers, count);
	DWORD written = 0;
	auto size = serBase.RelocatePtrs(&serBase);
	WriteTag(LAYR, size, count);
	serBase.Serialize(&serBase, m_hFile);
	serBase.Release(&serBase);
	Pad(size);
}
int CMeshExport::GetSurfIDList()
{
	//Get surfaceID list
	LWSurfaceFuncs *surff = (LWSurfaceFuncs*)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );
	surfIDList = surff->byObject(name);
	surf.count = 0;
	while (surfIDList[surf.count] != NULL)
    {
        ++surf.count;	// Count the number of surfaces
        shaders_to_surfIDs.push_back(defPrg);
    }
	return surf.count;
}
size_t CMeshExport::FindSurfaceIndexInSurfIdList(const char* name) {
	LWSurfaceFuncs *surff = (LWSurfaceFuncs*)global(LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT);
	size_t i = 0;
	while (surfIDList[i] != NULL)
	{
		const LWSurfaceID surfID = surfIDList[i];
		if (!strcmp(surff->name(surfID), name))
			return i;
		++i;
	}
	assert(false);
	return 0;
}
void CMeshExport::GatherLayerSurfaceSections(std::vector<Poly>& polygons, size_t start, size_t count, MeshLoader::Layer::Sections& section) {
	std::vector<MeshLoader::Layer::Sections::Section> sections;
	auto end = start + count;
	while (start < end) {
		count = 0u;
		const char* surf_name = polygons[start].surf;
		while (start + count < end && surf_name == polygons[start + count].surf) ++count;
		auto surf_index = FindSurfaceIndexInSurfIdList(surf_name);
		sections.push_back({Tag(SECT), (unsigned int)surf_index, (unsigned int)((count == 0) ? 0 : start), (unsigned int)count});
		start += count;
		//++start;
	}
	section.sections = (section.count = (index_t)sections.size()) ? new MeshLoader::Layer::Sections::Section[section.count] : nullptr;
	std::copy(sections.begin(), sections.end(), section.sections);
}

//void CMeshExport::GatherLayerSurfaceSections(std::vector<Poly>& polygons, size_t start, size_t count, std::vector<Layer::SurfSection>& sections) {
//	auto end = start + count;
//	while (start < end) {
//		count = 0u;
//		const char* surf_name = polygons[start].surf;
//		while (start + count < end && surf_name == polygons[start + count].surf) ++count;
//		auto surf_index = FindSurfaceIndexInSurfIdList(surf_name);
//		sections.push_back({ surf_index, (count == 0) ? 0 : start, count });
//		start += count;
//		++start;
//	}
//}
void CMeshExport::Save(LWObjectFuncs * objFunc, int object)
{
	surff = (LWSurfaceFuncs*)global(LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT);
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
		size_t nPolygons = 0, nVertices = 0;
		for (int layer = 0; layer < objFunc->maxLayers(object); ++layer) {
			if (!objFunc->layerExists(object, layer)) continue;
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)	continue;
			nVertices += meshInfo->numPoints(meshInfo);
			nPolygons += meshInfo->numPolygons(meshInfo);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
		}
		polygons.reserve(nPolygons);
		vertices.reserve(nVertices);
		// Scan points...
		for (int layer = 0; layer < objFunc->maxLayers(object);++layer) {
			if (!objFunc->layerExists(object, layer)) continue;
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)	continue;
			meshInfo->scanPoints(meshInfo, pointScan, this);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
		}
		std::sort(vertices.begin(), vertices.end(), [](const Vertex& v1, const Vertex& v2) {
			return intptr_t(v1.id) < intptr_t(v2.id); });
		DumpPoints();
		for (int layer = 0; layer < objFunc->maxLayers(object); ++layer) {
			if (!objFunc->layerExists(object, layer)) {
				layers.push_back({});
				continue;
			}
			PolyScanData psd{ meshInfo, lines, polygons, vertices, layer };
			meshInfo = objFunc->layerMesh(object, layer);
			if (!meshInfo)
				continue;
			auto poly_start = polygons.size(), line_start = lines.size();
			meshInfo->scanPolys(meshInfo, polygonScan, &psd);
			if (meshInfo->destroy)
				meshInfo->destroy(meshInfo);
			if (!m_bAllLayers)
				break;
			LWFVector pivot;
			objFunc->pivotPoint(object, layer, pivot);
			size_t poly_count = polygons.size() - poly_start;
			if (!poly_count) poly_start = 0;
			size_t line_count = lines.size() - line_start;
			if (!line_count) line_start = 0;
			layers.push_back({ { pivot[0], pivot[1], pivot[2] }, { poly_start, poly_count }, { line_start, line_count } });
		}
		auto srfIDCmp = [](const Poly& p1, const Poly& p2) {
			return strcmp(p1.surf, p2.surf)<0;};
		for (auto&l : layers) {
			std::sort(polygons.begin() + l.poly.start, polygons.begin() + l.poly.start + l.poly.count, srfIDCmp);
			std::sort(lines.begin() + l.line.start, lines.begin() + l.line.start + l.line.count, srfIDCmp);
		}

		DumpPolygons();
		DumpLines();
		DumpVMaps(object);
		InitSerBase(&serBase);
		GetSurfIDList();
		surf.surface = (Surface*)calloc(surf.count, sizeof(Surface));
		// TODO: if !surf.surf...
		CollectPtrSurface(&serBase, surf.surface, surf.count);
		long i = 0;
		while (surfIDList[i] != NULL)
		{
			CollectSurfaces(&surf.surface[i], &surfIDList[i], shaders_to_surfIDs[i]);
			i++;
		}
		auto size = serBase.RelocatePtrs(&serBase);
		WriteTag(SURF, size,i);
		serBase.Serialize(&serBase, m_hFile);
		Pad(size);
		serBase.Release(&serBase);	
		CleanupSurfaces(&surf);
		vmapList.clear();

		// Layers
		MeshLoader::Layer *layers2 = new MeshLoader::Layer[layers.size()];
		size_t count = 0;
		for (auto& l : layers) {
			//GatherLayerSurfaceSections(polygons, l.poly.start, l.poly.count, l.polySections);
			//GatherLayerSurfaceSections(lines, l.line.start, l.line.count, l.lineSections);
			GatherLayerSurfaceSections(polygons, l.poly.start, l.poly.count, layers2[count].poly);
			GatherLayerSurfaceSections(lines, l.line.start, l.line.count, layers2[count].line);
			layers2[count].pivot.x = l.pivot.x;
			layers2[count].pivot.y = l.pivot.y;
			layers2[count].pivot.z = l.pivot.z;
			layers2[count].tag = Tag(LAYR);
			++count;
			//l.surfInfo.push_back({});
			//auto& current = l.surfInfo.back();
			//current.line.start = l.line.start; current.line.count = l.line.count;
			//current.poly.start = l.poly.start; current.poly.count = l.poly.count;
			//auto layerSectionFct = [this](LWSurfaceFuncs *surff, Layer::SurfInfo::Section& section, const LWSurfaceID *surfID) {
			//	auto end = section.start + section.count;
			//	while ((section.start < end) && (strcmp(surff->name(*surfID), polygons[section.start].surf) != 0)) ++section.start;
			//	//Count polygons
			//	section.count = section.start;
			//	while ((section.count < polygons.size()) && (strcmp(surff->name(*surfID), polygons[section.count].surf) == 0)) ++section.count;
			//	section.count -= section.start;
			//	if (!section.count)
			//		section.start = 0;
			//};
			//layerSectionFct(surff, current->line, surfID);
			//layerSectionFct(surff, current->poly, surfID);
		}
		DumpLayers(layers2, count);
		delete[] layers2;
		::CloseHandle(m_hFile);
	}
}
void CMeshExport::CleanUp(void)
{
	// TODO::
}
