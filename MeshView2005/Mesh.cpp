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
	error(0), fixedFunc()
{
    fixedFunc.handle = 0;
}

CMesh::~CMesh(void)
{
	CleanUp();
}

void CMesh::CleanUp(void)
{
	while (m_Editors.size())
	{	
		delete m_Editors.back();
		m_Editors.pop_back();
	}
	//glDeleteBuffersARB(1, &m_nVBOVertices);
	glDeleteBuffersARB(1, &m_nVBONormals);
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
	//GLfloat specular;
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

	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	for(const auto& l : layers) {
		{
			for (size_t i = 0; i < l.poly.n; ++i) {
				const auto& section = l.poly.sections[i];

				CGLSurface * pGLSurface = &_glSurfaces[section.index];
				pGLSurface->Setup();
				_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
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
				_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
				if (CGLExtensions::ShaderSupported)
				{
					CGLProgram * pProgram = pGLSurface->_program;
					glUseProgramObjectARB(pProgram->handle);
					_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
					//GLint tex1 = glGetUniformLocationARB(pProgram->handle, "tex1");
				   //GLint tex2 = glGetUniformLocationARB(pProgram->handle, "tex2");
				   //GLint tex3 = glGetUniformLocationARB(pProgram->handle, "tex3");
				   //glUniform1iARB(tex1, 0);
				   //glUniform1iARB(tex2, 1);
				   //glUniform1iARB(tex3, 2);
				}
				// !!!
			  //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDrawArrays(GL_TRIANGLES, section.start * VERTICESPERPOLY, section.count * VERTICESPERPOLY);
				_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
				pGLSurface->TearDown();
				_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			}
		}
	}
	//glDisable(GL_BLEND);
	//glDisableClientState( GL_NORMAL_ARRAY );	
	//glDisableClientState( GL_VERTEX_ARRAY );	
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	//glPopMatrix();
}

void CMesh::Setup(unsigned int vbovertices)
{
 	m_nVBOVertices = vbovertices;
    _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	std::vector<XMFLOAT3> data(polygons.size() * VERTICESPERPOLY);
	for (size_t i = 0, dst = 0; i < polygons.size(); ++i) {
		data[dst++] = vertices[polygons[i].v[0]];		
		data[dst++] = vertices[polygons[i].v[1]];
		data[dst++] = vertices[polygons[i].v[2]];
	}

	glBufferDataARB( GL_ARRAY_BUFFER_ARB, data.size() * sizeof(XMFLOAT3), &data.front(), GL_STATIC_DRAW_ARB );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	// Normals...
	glGenBuffersARB( 1, &m_nVBONormals );
    _ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
	std::vector<XMFLOAT3> normals(polygons.size() * VERTICESPERPOLY);
	for (size_t i = 0, dst = 0; i < polygons.size(); ++i) {
		// TODO:: ccw hack
		XMFLOAT3 n;
		XMStoreFloat3(&n, XMVectorScale(XMLoadFloat3(&normalsV[i].n[0]), -1.f));
		normals[dst++] = n;
		XMStoreFloat3(&n, XMVectorScale(XMLoadFloat3(&normalsV[i].n[1]), -1.f));
		normals[dst++] = n;
		XMStoreFloat3(&n, XMVectorScale(XMLoadFloat3(&normalsV[i].n[2]), -1.f));
		normals[dst++] = n;
	}
   
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, normals.size() * sizeof(XMFLOAT3), &normals.front(), GL_STATIC_DRAW_ARB );
	_ASSERT_EXPR_A(!(error = glGetError()),(const char*)gluErrorString(error));

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
	_glSurfaces.reserve(surfaces.size());
	for (long i = 0; i<surfaces.size();i++)
	{
		
		CGLSurface glSrf(surfaces[i]);
		// Shader program
		Surface * pSrf = &surfaces[i];
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
