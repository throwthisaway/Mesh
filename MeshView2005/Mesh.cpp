//#ifdef WIN32
#include "StdAfx.h"
//#endif
#include "VectorMath.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>
#ifdef WIN32
//#include <GL/glaux.h>
#else
#include <GL/glx.h>
#endif
#include "Mesh.h"
#include "Path.h"
#include "AppConstants.h"
#include "FileWriter.h"
#include "DumpData.h"
#include "Log.h"
namespace Scene
{

CMesh::CMesh(ProgramMap& shaders) : shaders(shaders),
	m_nList(0), error(0), fixedFunc()// ,m_VertexToPolyIndices(NULL)
{
    fixedFunc.handle = 0;
}

CMesh::~CMesh(void)
{
	CleanUp();
}

void CMesh::CleanUp(void)
{	
	/*if (m_VertexToPolyIndices)
	{
		delete [] m_VertexToPolyIndices;
		m_VertexToPolyIndices = NULL;
	}*/
	if (m_nList)
		glDeleteLists(m_nList, 1);
	while (m_Editors.size())
	{	
		delete m_Editors.back();
		m_Editors.pop_back();
	}
	m_Surfaces.CleanUp();
	//glDeleteBuffersARB(1, &m_nVBOVertices);
	glDeleteBuffersARB(1, &m_nVBONormals);
}

MESH_ERROR CMesh::Load(std::auto_ptr<CMeshLoader> meshLoader)
{
	MESHLOADER_ERROR error = FILE_OK;
	if (!meshLoader.get())	// TODO: auto_ptr ???
		return MESH_FAIL;
	m_szName = meshLoader->m_pszMeshName;
	if (error = meshLoader->LoadVertices(m_Vertices))
	{
		Log::CLog::Write("Failed to load vertices...\r\n");
		CleanUp();
		return MESH_FAIL;
	}
	else if (error = meshLoader->LoadPolygons(m_Polygons))
	{
		Log::CLog::Write("Failed to load polygons...\r\n");
		CleanUp();
		return MESH_FAIL;
	}
	//else if (error = m_MeshLoader->LoadVMaps(m_VMaps))
	//{
	//	Log::CLog::Write("Failed to load vmaps...\r\n");
	//	CleanUp();
	//}
	else if (error = meshLoader->_LoadUVMaps(m_VMaps_))
	{
		Log::CLog::Write("Failed to load vmaps(2)...\r\n");
		CleanUp();
		return MESH_FAIL;
	}
	else if (error = meshLoader->_LoadDVMaps(m_VMaps_))
	{
		Log::CLog::Write("Failed to load discontinous vmaps...\r\n");
		CleanUp();
		return MESH_FAIL;
	}
	else if (error = meshLoader->LoadSurfaces(m_Surfaces))
	{		
		Log::CLog::Write("Failed to load surfaces...\r\n");
		CleanUp();
		return MESH_FAIL;
	}
	if (error == FILE_OK)
	{
		// TODO:: ...
	}
	return MESH_OK;
}
#define POLYGONSPOINT(i,n) m_Polygons.m_Polygons[i].Vertices[n]
#define POINT(i) m_Vertices.m_Vertices[i]
#define POINTX(i) POINT(i).x
#define POINTY(i) POINT(i).y
#define POINTZ(i) POINT(i).z

#define SURFACE(surf) m_Surfaces.m_Surface[surf]
#define SURFACECOLOR(surf) SURFACE(surf).color
#define SURFACECOLORR(surf) SURFACE(surf).color[0]
#define SURFACECOLORG(surf) SURFACE(surf).color[1]
#define SURFACECOLORB(surf) SURFACE(surf).color[2]

#define SURFACEPOLY(surf,poly) SURFACE(surf).poly_list[poly]


#define _POLYGONSPOINT(i,n) m_Polygons.GetPolygons()[i].Vertices[n]
#define _POINT(i) m_Vertices.GetVertices()[i]
#define _POINTX(i) _POINT2(i).x
#define _POINTY(i) _POINT2(i).y
#define _POINTZ(i) _POINT2(i).z

#define _POLYGONSNORMAL(i) m_Polygons.GetNormals()[i]
#define _POLYGONSNORMALA(i) _POLYGONSNORMAL(i).a
#define _POLYGONSNORMALB(i) _POLYGONSNORMAL(i).b
#define _POLYGONSNORMALC(i) _POLYGONSNORMAL(i).c
void SetupSurface(GLenum face, Surface *surf)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat specular;
	glColor3f(surf->color[0], surf->color[1], surf->color[2]);
	//if ((specular = surf->surface_infos[SPECULARITY_MAP].val) > 0.0f)
	//{
	//	GLfloat mat_spec[] = {surf->color[0], surf->color[1], surf->color[2]/*specular, specular, specular*/, 1.0f};
	//	glColorMaterial(face, GL_SPECULAR);
	//	glMaterialfv(face, GL_SPECULAR, mat_spec);
	//	glMaterialf(face, GL_SHININESS, surf->surface_infos[GLOSSINESS_MAP].val*128.0f);
	//}
	//else
	//{
	//	GLfloat diffuse = surf->surface_infos[DIFFUSE_MAP].val;
	//	GLfloat mat_dif[] = {surf->color[0], surf->color[1], surf->color[2]/*diffuse, diffuse, diffuse*/, 1.0f};
	//	glColorMaterial(face, GL_DIFFUSE);
	//	glMaterialfv(face, GL_DIFFUSE, mat_dif);
	//}
}

//void CMesh::BuildList(void)
//{
//	// Not working with ATI...
//	m_nList = glGenLists(1);
//	Log::CLog::Write("Creating list for mesh: %d...\r\n", glGetError());
//	GLenum error;
//	glNewList(m_nList,GL_COMPILE);	
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	glEnableClientState( GL_VERTEX_ARRAY );		
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	//GLfloat * data = (GLfloat*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_ONLY);
//	glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL);
//	/*float *data2 = new float[c];
//	glBufferData(GL_ARRAY_BUFFER_ARB, c, data, GL_STATIC_DRAW);*/
//#ifdef _DEBUG
//	error = glGetError();
//#endif	
//	glEnableClientState( GL_NORMAL_ARRAY );
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBONormals);
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	glNormalPointer(GL_FLOAT, 0, (char *) NULL );
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	long i = 0;
//	glDisable(GL_TEXTURE_2D);
//	for (Surface * surf=m_Surfaces.GetSurfaces();i<m_Surfaces.m_nSurfaces; i++, surf++)
//	{
//		//GLfloat lit_dif[] = {0.5f,0.5f,0.5f,0.5f};
//		//glLightfv(GL_LIGHT0, GL_DIFFUSE,lit_dif/*surf->surface_infos[DIFFUSE_MAP].val*/);		
//		/*GLfloat mat_dif[] = {surf->color[0], surf->color[1], surf->color[2],0.0f};
//		//surf->surface_infos[DIFFUSE_MAP].val
//		glDisable(GL_COLOR_MATERIAL);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE,mat_dif);		*/		
//		if (surf->sidedness == SINGLE_SIDED)
//		{
//			//surf->sidedness = GL_FRONT;
//			glDisable(GL_VERTEX_PROGRAM_TWO_SIDE);
//			glEnable(GL_CULL_FACE);
//			glCullFace(GL_BACK);
//			SetupSurface(GL_FRONT, surf);
//			
//		}
//		else
//		{
//			//surf->sidedness = GL_FRONT_AND_BACK;
//			glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
//			glDisable(GL_CULL_FACE);
//			SetupSurface(GL_FRONT_AND_BACK, surf);			
//		}
//		glColor3f(surf->color[0], surf->color[1], surf->color[2]);
//		
//		glUseProgramObjectARB(surf->handle);
//		SurfLayer *colLayer;
//		GLenum activeTexture = GL_TEXTURE0;
//		if (surf->surface_infos && (colLayer = surf->surface_infos->layers))
//		{				
//			while (colLayer && colLayer->type != IMAGE_LAYER) colLayer = colLayer->next;
//			if (colLayer->type == IMAGE_LAYER)
//			{
//				if (colLayer->image && colLayer->image->ref)
//				{
//					glEnable(GL_TEXTURE_2D);
//					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
//					glActiveTextureARB(activeTexture);activeTexture++;
//					Scene::GLImage * img = (Scene::GLImage*)(colLayer->image->ref);
//					glBindTexture(GL_TEXTURE_2D,img->ID);
//					glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VMaps.m_pnVBOUVCoords[(long)colLayer->uvmap]); //it's still an index, @ commented out in CVMaps::Setup
//					glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL );
//				}
//			}
//		}
//		//long poly_end = SURFACE(surf).poly_num + SURFACE(surf).poly_offset;
//
//		glDrawArrays(GL_TRIANGLES, surf->poly_offset * VERTICESPERPOLY , surf->poly_num * VERTICESPERPOLY);
//		
//		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//		glDisable( GL_TEXTURE_2D );
//	}	
//	glEndList();
//}

void CMesh::Draw(void)
{
	/*UVMap * uvmap = NULL;
	for (long surf=0;surf<m_Surfaces.m_nSurfaces;surf++)
	{
		//glColor3f(SURFACECOLORR(surf),SURFACECOLORG(surf),SURFACECOLORB(surf));
		long poly_end = SURFACE(surf).poly_num + SURFACE(surf).poly_offset;
		for (long _poly=SURFACE(surf).poly_offset;_poly<poly_end;_poly++)
		{		
			//Vector<float> * normal = &_POLYGONSNORMAL(_poly);
			SurfLayer *colLayer;
			if (SURFACE(surf).surface_infos && (colLayer = SURFACE(surf).surface_infos->layers))
			{				
				while (colLayer && colLayer->type != IMAGE_LAYER) colLayer = colLayer->next;
				if (colLayer->type == IMAGE_LAYER)
				{
					uvmap = &m_VMaps.m_UVMap[(long)colLayer->uvmap];
					if (colLayer->image && colLayer->image->ref)
					{
						Scene::GLImage * img = (Scene::GLImage*)(colLayer->image->ref);
						glBindTexture(GL_TEXTURE_2D,img->ID);
					}
				}
				
			}
			
			glBegin(GL_TRIANGLES);	
				//glNormal3f(normal->a, normal->b, normal->c);
				Vector<float> * normal = m_Vertices.GetNormal(POLYGONSPOINT(_poly,0));				
				glNormal3f(normal->a, normal->b, normal->c);
				//UVCoords(POLYGONSPOINT(_poly,0), uvmap);
				glTexCoord2f(m_VMaps.m_UVMap[0].uv[_poly*6], m_VMaps.m_UVMap[0].uv[_poly*6+1]);
				glVertex3f(POINTX(POLYGONSPOINT(_poly,0)),POINTY(POLYGONSPOINT(_poly,0)),POINTZ(POLYGONSPOINT(_poly,0)));
				normal = m_Vertices.GetNormal(POLYGONSPOINT(_poly,1));
				glNormal3f(normal->a, normal->b, normal->c);
				glTexCoord2f(m_VMaps.m_UVMap[0].uv[_poly*6+2], m_VMaps.m_UVMap[0].uv[_poly*6+3]);
				//UVCoords(POLYGONSPOINT(_poly,1), uvmap);
				glVertex3f(POINTX(POLYGONSPOINT(_poly,1)),POINTY(POLYGONSPOINT(_poly,1)),POINTZ(POLYGONSPOINT(_poly,1)));
				normal = m_Vertices.GetNormal(POLYGONSPOINT(_poly,2));
				glNormal3f(normal->a, normal->b, normal->c);
				glTexCoord2f(m_VMaps.m_UVMap[0].uv[_poly*6+4], m_VMaps.m_UVMap[0].uv[_poly*6+5]);
				//UVCoords(POLYGONSPOINT(_poly,2), uvmap);
				glVertex3f(POINTX(POLYGONSPOINT(_poly,2)),POINTY(POLYGONSPOINT(_poly,2)),POINTZ(POLYGONSPOINT(_poly,2)));				
			glEnd();
		}
	}*/
	/*glDisable(GL_TEXTURE_2D);
	glColor3f(0.5,0.5,0.5);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
	glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
	error = glGetError();
	glDrawArrays(GL_TRIANGLES, 0,  m_Polygons.GetCount());
	error = glGetError();
	for(long _poly = 0; _poly<432;)
	{
			glBegin(GL_TRIANGLES);	
				//glNormal3f(normal->a, normal->b, normal->c);
				glVertex3f(vert[_poly++],vert[_poly++],vert[_poly++]);
				glVertex3f(vert[_poly++],vert[_poly++],vert[_poly++]);
				glVertex3f(vert[_poly++],vert[_poly++],vert[_poly++]);				
			glEnd();
	}*/
	/*long _poly = 0;
	glBegin(GL_TRIANGLES);	

				glVertex3f(data[0],data[1],data[2]);
				glVertex3f(data[3],data[4],data[5]);
				glVertex3f(data[6],data[7],data[8]);					
	glEnd();*/
	/*glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f,0.0f,1.0f);
	glVertex3f(1.0f,0.0f,1.0f);
	glVertex3f(0.5f,1.0f,1.0f);
	glEnd();*/
	/*for (int i = 0; i<c;)
	{
		glBegin(GL_TRIANGLES);	
		glVertex3f(data[i],data[i+1],data[i+2]);i+=3;
		glVertex3f(data[i],data[i+1],data[i+2]);i+=3;
		glVertex3f(data[i],data[i+1],data[i+2]);i+=3;
		glEnd();
	} 
	glFlush();*/


//	glEnableClientState( GL_VERTEX_ARRAY );		
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	//GLfloat * data = (GLfloat*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_ONLY);
//	glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL);
//	/*float *data2 = new float[c];
//	glBufferData(GL_ARRAY_BUFFER_ARB, c, data, GL_STATIC_DRAW);*/
//#ifdef _DEBUG
//	error = glGetError();
//#endif	
//	glEnableClientState( GL_NORMAL_ARRAY );
//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBONormals);
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	glNormalPointer(GL_FLOAT, 0, (char *) NULL );
//#ifdef _DEBUG
//	error = glGetError();
//#endif
//	long i = 0;
//	glDisable(GL_TEXTURE_2D);
//	for (Surface * surf=m_Surfaces.GetSurfaces();i<m_Surfaces.m_nSurfaces; i++, surf++)
//	{
//		//GLfloat lit_dif[] = {0.5f,0.5f,0.5f,0.5f};
//		//glLightfv(GL_LIGHT0, GL_DIFFUSE,lit_dif/*surf->surface_infos[DIFFUSE_MAP].val*/);		
//		/*GLfloat mat_dif[] = {surf->color[0], surf->color[1], surf->color[2],0.0f};
//		//surf->surface_infos[DIFFUSE_MAP].val
//		glDisable(GL_COLOR_MATERIAL);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE,mat_dif);		*/		
//		if (surf->sidedness == SINGLE_SIDED)
//		{
//			//surf->sidedness = GL_FRONT;
//			glDisable(GL_VERTEX_PROGRAM_TWO_SIDE);
//			glEnable(GL_CULL_FACE);
//			glCullFace(GL_BACK);
//			SetupSurface(GL_FRONT, surf);
//			
//		}
//		else
//		{
//			//surf->sidedness = GL_FRONT_AND_BACK;
//			glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
//			glDisable(GL_CULL_FACE);
//			SetupSurface(GL_FRONT_AND_BACK, surf);			
//		}
//		glColor3f(surf->color[0], surf->color[1], surf->color[2]);
//		
//		glUseProgramObjectARB(surf->handle);
//		GLint tex1 = glGetUniformLocationARB(surf->handle, "tex1");
//		GLint tex2 = glGetUniformLocationARB(surf->handle, "tex2");
//		GLint tex3 = glGetUniformLocationARB(surf->handle, "tex3");
//		glUniform1iARB(tex1, 0);
//		glUniform1iARB(tex2, 1);
//		glUniform1iARB(tex3, 2);
//		SurfLayer *colLayer;
//		GLenum activeTexture = GL_TEXTURE0;
//		if (surf->surface_infos && (colLayer = surf->surface_infos->layers))
//		{			
//			while (colLayer)
//			{
//				while (colLayer && colLayer->type != IMAGE_LAYER) colLayer = colLayer->next;
//				if (colLayer->type == IMAGE_LAYER)
//				{
//					if (colLayer->image && colLayer->image->ref)
//					{
//						glEnable(GL_TEXTURE_2D);
//						glActiveTextureARB(activeTexture);
//						glClientActiveTextureARB(activeTexture); activeTexture++;						
//						glEnableClientState( GL_TEXTURE_COORD_ARRAY );						
//						Scene::GLImage * img = (Scene::GLImage*)(colLayer->image->ref);
//						glBindTexture(GL_TEXTURE_2D,img->ID);
//						//textureIDs[t] = t; t++;
//						//glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VMaps.m_pnVBOUVCoords[(long)colLayer->uvmap]); //it's still an index, @ commented out in CVMaps::Setup
//						glBindBufferARB(GL_ARRAY_BUFFER_ARB, colLayer->uvmap);
//						glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);						
//					}
//				}
//				else
//					break;
//				colLayer = colLayer->next;
//			}
//		}
//		//long poly_end = SURFACE(surf).poly_num + SURFACE(surf).poly_offset;
//
//		glDrawArrays(GL_TRIANGLES, surf->poly_offset * VERTICESPERPOLY , surf->poly_num * VERTICESPERPOLY);
//		/*for (i = GL_TEXTURE0_ARB; i<(long)activeTexture; i++);
//		{
//			glClientActiveTextureARB(i);
//			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//		}*/
//		glDisable( GL_TEXTURE_2D );
//	}
//	//glCallList(m_nList);

//	glPushMatrix();
	glEnableClientState( GL_VERTEX_ARRAY );		
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
#ifdef _DEBUG
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
#endif	
	//glVertexPointer( 3, GL_FLOAT, 0, (char *) data);
    glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL);
#ifdef _DEBUG
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
#endif	
	glEnableClientState( GL_NORMAL_ARRAY );
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBONormals);
#ifdef _DEBUG
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
#endif
	glNormalPointer(GL_FLOAT, 0, (char *) NULL );
#ifdef _DEBUG
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
#endif
	long i = 0;
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	for (Surface * surf=m_Surfaces.GetSurfaces();i<m_Surfaces.m_nSurfaces; i++, surf++)
	{
		// TODO:: iterate through them...
		CGLSurface * pGLSurface = &_glSurfaces[i];
		pGLSurface->Setup();
        _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		/*for_each(pGLSurface->_enableCaps.begin(), pGLSurface->_enableCaps.end(), glEnable);
		for_each(pGLSurface->_disableCaps.begin(), pGLSurface->_disableCaps.end(), glDisable);
		glColor4f(pGLSurface->_color[0], pGLSurface->_color[1], pGLSurface->_color[2], pGLSurface->_color[3]);
		glCullFace(pGLSurface->_cullFace);
		pGLSurface->Blend();
		glColorMaterial(pGLSurface->_frontFace, pGLSurface->_colorMat);
		glMaterialfv(pGLSurface->_frontFace, GL_SPECULAR,pGLSurface->_spec);
		glMaterialf(pGLSurface->_frontFace, GL_SHININESS, pGLSurface->_shin);*/

		//SurfInfo& pSpecular = surf->surface_infos[SPECULARITY_MAP];
		//if (pSpecular.val > .0f)
		//{
		//	glEnable(GL_COLOR_MATERIAL);
		//	GLfloat mat_spec[] = {pSpecular.val, pSpecular.val, pSpecular.val};
		//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
		//	glMaterialf(GL_FRONT, GL_SHININESS, surf->surface_infos[GLOSSINESS_MAP].val*128.0f);
		//}

	/*	if (surf->sidedness == SINGLE_SIDED)
		{
			glDisable(GL_VERTEX_PROGRAM_TWO_SIDE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			SetupSurface(GL_FRONT, surf);
			
		}
		else
		{
			glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
			glDisable(GL_CULL_FACE);
			SetupSurface(GL_FRONT_AND_BACK, surf);			
		}*/
        pGLSurface->Bind();
         _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		if (CGLExtensions::ShaderSupported)
		{
			CGLProgram * pProgram = pGLSurface->_program;
			glUseProgramObjectARB(pProgram->handle);
             _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		     //GLint tex1 = glGetUniformLocationARB(pProgram->handle, "tex1");
			//GLint tex2 = glGetUniformLocationARB(pProgram->handle, "tex2");
			//GLint tex3 = glGetUniformLocationARB(pProgram->handle, "tex3");
			//glUniform1iARB(tex1, 0);
			//glUniform1iARB(tex2, 1);
			//glUniform1iARB(tex3, 2);
		}
        // !!!
      //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDrawArrays(GL_TRIANGLES, surf->poly_offset * VERTICESPERPOLY , surf->poly_count * VERTICESPERPOLY);
        _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
		pGLSurface->TearDown();
        _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));

	}
	//glDisable(GL_BLEND);
	//glDisableClientState( GL_NORMAL_ARRAY );	
	//glDisableClientState( GL_VERTEX_ARRAY );	
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	//glPopMatrix();
}


long CMesh::CalcPolygonNormals(void)
{
	long polyrefcount = 0;
	for (long i = 0;i<m_Polygons.GetCount();i++)
	{		
		long _point1,_point2,_point3;	//Point indices
		m_Vertices.GetPoygonRefs()[_point1 = _POLYGONSPOINT(i,0)].count++;
		m_Vertices.GetPoygonRefs()[_point2 = _POLYGONSPOINT(i,1)].count++;
		m_Vertices.GetPoygonRefs()[_point3 = _POLYGONSPOINT(i,2)].count++;
		polyrefcount +=3;
		CVertex point1 = _POINT(_point1), point2 = _POINT(_point2), point3 = _POINT(_point3);
		Vector<float>& normal = _POLYGONSNORMAL(i);
		normal.x = point1.y * (point2.z - point3.z) + point2.y * (point3.z - point1.z) + point3.y * (point1.z - point2.z);
		normal.y = point1.z * (point2.x - point3.x) + point2.z * (point3.x - point1.x) + point3.z * (point1.x - point2.x);
		normal.z = point1.x * (point2.y - point3.y) + point2.x * (point3.y - point1.y) + point3.x * (point1.y - point2.y);		
		// Normalize
		float r = (float)sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		if (r>0)
        {
		    normal.x /= r;
		    normal.y /= r;
		    normal.z /= r;
        }
	}
    for (long i = 0;i<m_Polygons.GetCount();i++)
	{
        for (long k = 0;k<VERTICESPERPOLY;k++)
        {
            CPolygonRef& polyRef = m_Vertices.GetPoygonRefs()[_POLYGONSPOINT(i,k)];
            if (!polyRef.indices)
            {
                polyRef.indices = new long[polyRef.count];
                polyRef.count = 0;
            }
            polyRef.indices[polyRef.count++] = i;
        }
    }
	return polyrefcount;
}
std::auto_ptr<char> CMesh::DumpColoring(void)
{
     char* data = new char[GetRawVertexCount()*3];
    for (long i = 0; i<m_Surfaces.GetCount(); i++)		
	{
		Surface * surface = &m_Surfaces.m_Surface[i]; 
		long poly_end = surface->poly_count + surface->poly_offset;
		for (long j = surface->poly_offset; j<poly_end; j++)
        {
            for (long k = 0; k<VERTICESPERPOLY; k++)
            {
                data[(j*VERTICESPERPOLY+k)*3] = surface->color[0]*255;
                data[(j*VERTICESPERPOLY+k)*3+1] = surface->color[1]*255;
                data[(j*VERTICESPERPOLY+k)*3+2] = surface->color[2]*255;
            }
        }
    }
    return std::auto_ptr<char>(data);
}
std::auto_ptr<float> CMesh::CalcVertexNormals(void)
{
    float* data = new float[GetRawVertexCount()*3];
	for (long i = 0; i<m_Surfaces.GetCount(); i++)		
	{
		Surface * surface = &m_Surfaces.m_Surface[i]; 
		long poly_end = surface->poly_count + surface->poly_offset;
		for (long j = surface->poly_offset; j<poly_end; j++)
		{
			CPolygon * polygon = m_Polygons.GetPolygon(j);
			float smoothing = m_Surfaces.m_Surface[i].smoothing;
            Vector<float> * normal = m_Polygons.GetNormal(j);
			for (long k = 0; k<VERTICESPERPOLY; k++)
			{
				/*Vector<float> * normal1 = m_Vertices.GetNormal(polygon->Vertices[j]);
				Vector<float> * normal2 = m_Polygons.GetNormal(surface->poly_list[j]);
				if (CVectorMath<float>::Angle(*normal1, *normal2) <= smoothing)
				{
				*/
               //  The correct way of doing it...
                data[(j*VERTICESPERPOLY+k)*3] = normal->x;
                data[(j*VERTICESPERPOLY+k)*3+1] = normal->y;
                data[(j*VERTICESPERPOLY+k)*3+2] = normal->z;
                CVertex& vert = m_Vertices.GetVertex(polygon->Vertices[k]);
                long polyRefCount = m_Vertices.GetPoygonRefs()[polygon->Vertices[k]].count;
                for (long l = 0;l<polyRefCount;++l)
                {
                    CPolygonRef& polyRef = m_Vertices.GetPoygonRefs()[polygon->Vertices[k]];
                    Vector<float> * polyRefNormal = m_Polygons.GetNormal(polyRef.indices[l]);
                    if (CVectorMath<float>::Angle(*normal, *polyRefNormal)<smoothing)
                    {
                        data[(j*VERTICESPERPOLY+k)*3] += polyRefNormal->x;
                        data[(j*VERTICESPERPOLY+k)*3+1] += polyRefNormal->y;
                        data[(j*VERTICESPERPOLY+k)*3+2] += polyRefNormal->z;
                    }
                }
               // ...The correct way of doing it

    //            // Old, erroneous way...
				//Vector<float>& n = m_Vertices.GetNormal(polygon->Vertices[k]);
    //            if (n.x == n.y && n.y==n.z)
    //            {
    //                n = *normal;
    //                data[(j*VERTICESPERPOLY+k)*3] = n.x;
    //                data[(j*VERTICESPERPOLY+k)*3+1] = n.y;
    //                data[(j*VERTICESPERPOLY+k)*3+2] += n.z;
    //            }
    //            else if (CVectorMath<float>::Angle(n, *normal)<=smoothing)
    //            {
				//    n += *normal;
    //                CVectorMath<float>::Normalize(n);
    //                data[(j*VERTICESPERPOLY+k)*3] = n.x;
    //                data[(j*VERTICESPERPOLY+k)*3+1] = n.y;
    //                data[(j*VERTICESPERPOLY+k)*3+2] += n.z;
    //            }
    //            // ...Old, erroneous way
			//vertex_normal.a += polygonRef->indices[j]-
 			}
		}
        // The correct way of doing it...
        for (long j = 0; j<GetRawVertexCount()*3; j+=3)
        {
           float r = data[j] * data[j]  +data[j+1] * data[j+1] + data[j+2] * data[j+2];
           if (r>0)
           {
               data[j] = data[j]/r;
               data[j+1] = data[j+1]/r;
               data[j+2] = data[j+2]/r;
           }
        } 
        //...The correct way of doing it

        // Old, erroneous way...
		if (surface->poly_count>0)
		{
 			for (long j = 0; j<m_Vertices.GetCount(); j++)
			{
				CVectorMath<float>::Normalize(m_Vertices.GetNormal(j));
			}
		}
        // ...Old, erroneous way
		//CPolygonRef * polygonRef = &m_Vertices.GetPoygonRefs()[i];
		//Vector<float> * vertex_normal = &m_Vertices.GetNormal()[i];
			
	}
    return std::auto_ptr<float>(data);
}
void CMesh::Setup(unsigned int vbovertices)
{	
    // data dump...
    std::string path(IO::CPath::GetPath(m_szName)), name(IO::CPath::GetFileName(m_szName)), out;
    out = path+name;
    CDumpData dump(IO::CPath::SetExt(out, "robo").c_str());
    // ...data dump
	long i, c;
	GLenum error;
	m_Vertices.Setup();
	m_Polygons.Setup();
	//m_VMaps.Setup(m_Surfaces);
	m_VMaps_.Setup(m_Surfaces, m_Polygons, m_Vertices);
	long polyrefcount = CalcPolygonNormals();
	// TODO: if smoothing off...
	
	m_nVBOVertices = vbovertices;
	// Vertices...
	GLuint size = GetRawVertexCount()*3;
	/*float **/ data = new float[size];
	for (i = 0,c = 0; i<m_Polygons.m_nPolygons;i++)
	{
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[0]).x;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[0]).y;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[0]).z;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[1]).x;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[1]).y;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[1]).z;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[2]).x;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[2]).y;
		data[c++] = m_Vertices.GetVertex(m_Polygons.GetPolygon(i)->Vertices[2]).z;
	}
    _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, size * sizeof(float), data, GL_STATIC_DRAW_ARB );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	// debug
	this->c = c;
	//
   
    dump.ExportData(data, size*sizeof(float), true);
	delete [] data;
	// Normals...
	glGenBuffersARB( 1, &m_nVBONormals );
    _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
    auto_ptr<float> data2 = CalcVertexNormals();
	// TODO: if smoothing off...
	//data = new float[size];
	//for (i = 0,c = 0; i<m_Polygons.m_nPolygons;i++)
	//{
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[0]).x;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[0]).y;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[0]).z;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[1]).x;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[1]).y;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[1]).z;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[2]).x;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[2]).y;
	//	data[c++] = m_Vertices.GetNormal(m_Polygons.GetPolygon(i)->Vertices[2]).z;
	//}
    
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, size * sizeof(float), data2.get(), GL_STATIC_DRAW_ARB );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
    dump.ExportData(data2.get(), size*sizeof(float), TRUE);

    auto_ptr<char> data3 = DumpColoring();
    dump.ExportData(data3.get(), size*sizeof(char), TRUE);
	//delete [] data;
	
	
	//m_VertexToPolyIndices = new long[polyrefcount];
	//// Vertex to polygons...
	//for (long i = 0,inc = 0; i<m_Vertices.GetCount(); i++)
	//{		
	//	CPolygonRef * polygonref = &m_Vertices.GetPoygonRefs()[i];
	//	polygonref->indices = &m_VertexToPolyIndices[inc];
	//	inc += polygonref->count;
	//	polygonref->count = 0;
	//}
	//for (long i = 0; i<m_Polygons.GetCount(); i++)
	//{				
	//	for (long j = 0; j < VERTICESPERPOLY; j++)
	//	{
	//		CPolygonRef * polygonref = &m_Vertices.GetPoygonRefs()[_POLYGONSPOINT(i,j)];
	//		polygonref->indices[polygonref->count++] = i;
	//	}		
	//}
}
// Try to load the shaders from the override location
// default dir.: <shader_dir>/<mesh_name>/<srf_name>.program
CGLProgram * CMesh::PrgOverride(Surface& srf)
{
    IO::CPath path(IO::CPath::GetFileName(m_szName));
    path.Combine(srf.name);
    path.SetExt(string(CAppConstants::program_ext));
    CGLProgram * prg  = new CGLProgram();
    if (prg->Load(path, ::CAppConstants::program_dir, ::CAppConstants::shader_dir) != LOAD_OK)
    {
        delete prg;
        return NULL;
    }
	shaders[path] = prg;
    return prg;
}
void CMesh::SetupGLSurfaces(ProgramMap& shaders)
{
    _glSurfaces.clear();
	_glSurfaces.reserve(m_Surfaces.GetCount());
	for (long i = 0; i<m_Surfaces.GetCount();i++)
	{
		
		CGLSurface glSrf(m_Surfaces.GetSurfaces()[i]);
		// Shader program
		Surface * pSrf = &m_Surfaces.GetSurfaces()[i];
        CGLProgram * program = PrgOverride(*pSrf);
        if (NULL != program)
            glSrf._program = program;
        else
        {
            ProgramMap::const_iterator shaderIt = shaders.find(std::string(pSrf->program_name));
            if (shaderIt != shaders.end())
            {
                glSrf._program = shaderIt->second;
                std::string surf_name(pSrf->name);
                m_Editors.push_back(new CEditor(surf_name + ": " + shaderIt->first, std::string(((*shaderIt->second).vs.GetLines() == NULL)? "<Shader not loaded...>":(*shaderIt->second).vs.GetLines())));
                m_Editors.push_back(new CEditor(surf_name + ": " + shaderIt->first, std::string(((*shaderIt->second).fs.GetLines() == NULL)? "<Shader not loaded...>":(*shaderIt->second).fs.GetLines())));
            }
            else
            {
                glSrf._program = &fixedFunc;
                Log::CLog::Write("CMesh::SetupGLSurfaces: WARNING: Could not find any suitable shader with name: %s\r\n",pSrf->program_name);
            }
        }
        _glSurfaces.push_back(glSrf);
	}
}

}
