#include "stdafx.h"
#include "VMap.h"
#include "Log.h"

namespace Scene
{

int cmpUVVert(const void * p1, const void * p2)
{
	// TODO:: point index must be smaller than 2^31 !!!
	return (int)((__int64)p1 - ((UV*)p2)->point);
}

void _CVMaps::CleanUp(void) 
{
	DEL_ARRAY(m_UVMap);
	DEL_ARRAY(m_DVMap);
	for (long i=0;i<nuvmaps;i++)
	{
		glDeleteBuffersARB(1, &uvmaps[i]->VBO);
		DEL_ARRAY(uvmaps[i]);
	}
	DEL_ARRAY(uvmaps);
}

void _CVMaps::CreateSurfaceUVs(long n, SurfaceUVMap * uvmap, unsigned int offset, unsigned int num, CPolygons& polygons, CVertices& vertices)
{
	if (uvmap->uv)
		return;
	if (n>=m_nUVMaps)
	{
		Log::CLog::Write("CreateSurfaceUVs: not matching UV map count...\r\n");
		return;
	}
	long size;
	uvmap->uv = new float[size = (UVCOORDS * VERTICESPERPOLY * num)];
	// UVs
	for (unsigned int  i=offset, j=0;i<offset+num;i++)
	{
		for (unsigned int k=0;k<VERTICESPERPOLY;k++)
		{			
			UV *pUV = (UV*)bsearch((const void*)polygons.m_Polygons[i].Vertices[k],m_UVMap[n].uv, m_UVMap[n].nV, sizeof(UV), cmpUVVert);
			if(pUV)
			{
				uvmap->uv[j] = pUV->u;j++;
				uvmap->uv[j] = pUV->v;j++;
			}
			else
			{
				// Do nothing...
			}
		}
	}
	// Discontinous UVs
	for (unsigned long l=0;l<m_DVMap[n].nV;l++)
	{
		DV *pDV = &m_DVMap[n].dv[l];
		if((pDV->poly >= offset) && (pDV->poly<offset+num))
		{				
			float * uv = &uvmap->uv[(pDV->poly - offset) * UVCOORDS * VERTICESPERPOLY];
			uv+=pDV->point*UVCOORDS;
			*uv=pDV->u;uv++;
			*uv=pDV->v;
		}
	}
	glGenBuffersARB(1, &uvmap->VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvmap->VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size * sizeof(float), uvmap->uv, GL_STATIC_DRAW_ARB);
}

void _CVMaps::Setup(Surfaces& surfaces, CPolygons& polygons, CVertices& vertices)
{	
	if (!m_nUVMaps)
		return;
	uvmaps = new SurfaceUVMap*[nuvmaps = surfaces.m_nSurfaces];
	for (long i=0;i<surfaces.m_nSurfaces;i++)
	{
		uvmaps[i] = new SurfaceUVMap[m_nUVMaps];		
	}	
	for (long i=0;i<surfaces.m_nSurfaces;i++)
	{		
		for (unsigned int j = 0; j<NUM_MAP_TYPE;j++)
		{
			SurfLayer * pLayer = surfaces.m_Surface[i].surface_infos[j].layers;
			while (pLayer && (pLayer->uvmap != m_nUVMaps))
			{
				CreateSurfaceUVs((long)pLayer->uvmap, &uvmaps[i][(long)pLayer->uvmap], surfaces.m_Surface[i].poly_offset, surfaces.m_Surface[i].poly_num, polygons, vertices);
				pLayer->uvmap = uvmaps[i][(long)pLayer->uvmap].VBO;
				pLayer = pLayer->next;
			}	
		}
	}
}

//void CVMaps::CleanUp(void) 
//{
//	DEL_ARRAY(m_UVMap);
//	glDeleteBuffersARB(m_nUVMaps, m_pnVBOUVCoords);
//	DEL_ARRAY(m_pnVBOUVCoords);
//}

//void CVMaps::Setup(Surfaces & surfaces)
//{	
//	if (!m_nUVMaps)
//		return;
//	for (long i=0;i<surfaces.m_nSurfaces;i++)
//	{		
//		for (unsigned int j = 0; j<NUM_MAP_TYPE;j++)
//		{
//			SurfLayer * pLayer = surfaces.m_Surface[i].surface_infos[j].layers;
//			while (pLayer)
//			{
//				for (long j = 0;j<m_nUVMaps;j++)
//				{
//					if (m_UVMap[j].ID == (long)pLayer->uvmap)
//						pLayer->uvmap = j;
//						//pLayer->uvmap = &m_UVMap[(long)pLayer->uvmap];
//					
//				}
//				pLayer = pLayer->next;
//			}	
//		}
//	}
//	m_pnVBOUVCoords = new GLuint[m_nUVMaps];
//	glGenBuffersARB(m_nUVMaps, m_pnVBOUVCoords);
//	// TODO: one VBO for all maps..	
//	for (long i = 0; i<m_nUVMaps;i++)
//	{		
//		long size = m_UVMap[i].count * 2 * sizeof(float);//* VERTICESPERPOLY 
//		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_pnVBOUVCoords[i]);
//		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, m_UVMap[i].uv, GL_STATIC_DRAW_ARB);
//	}
//}

}
