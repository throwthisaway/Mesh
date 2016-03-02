#include "stdafx.h"
#include "Torus.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "VectorMath.h"
namespace Scene
{
	CTorus::CTorus(ProgramMap& shaders) : CMesh(shaders)
	{
	}


	CTorus::~CTorus(void)
	{
		int error = glGetError();
		glDeleteBuffersARB(1, &m_nVBOVertices);
		error = glGetError();
		glDeleteBuffersARB(1, &m_nVBOVertices);
		error = glGetError();
	}

	void CTorus::Setup(float r1, float r2, int s1, int s2)
	{
		const unsigned int vertexCount = (s1 + 2) * 2 * s2;
		//
		this->m_Vertices.CreateAndRetrieveInternalData(vertexCount);
		this->m_Vertices.SetupNormals();
		CVertices::VertexArr& vArr = this->m_Vertices._vertices;
		CVertices::VectorArr& nArr = this->m_Vertices._normals;
        float ds1 = 2* (float)M_PI / s1;
		float ds2 = 2* (float)M_PI / s2;
		//

		/*for (int u = 0, i = 0;u<s2;u++)
		{
			for (int v = 0;v<s1;v++)
			{
				vArr[i].x = (r2 + r1 * cos(ds1 * v)) * cos(ds2 * u);
				vArr[i].y = r1 * sin(ds1 * v);
				vArr[i].z = (r2 + r1 * cos(ds1 * v)) * sin(ds2 * u);
				i++;
			}
		}*/
		for (int u = 0, i = 0, v;u<s2;u++)
		{
			for (v = 0;v<=s1;v++)
			{
                CreateVertex(i, u + 1, v, r1, r2, ds1, ds2);
                i++;
                CreateVertex(i, u, v, r1, r2, ds1, ds2);
                i++;
			}
            CreateVertex(i, u + 1, v - 1, r1, r2, ds1, ds2);
            i++;
            CreateVertex(i, u + 1, v - 1, r1, r2, ds1, ds2);
            i++;
		}
		glGenBuffersARB(1, &m_nVBOVertices);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_Vertices._vertices.size() * sizeof(CVertices::VertexArr::value_type), m_Vertices._vertices.data(), GL_STATIC_DRAW_ARB );
		glGenBuffersARB(1, &m_nVBONormals);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_Vertices._normals.size() * sizeof(CVertices::VectorArr::value_type), m_Vertices._normals.data(), GL_STATIC_DRAW_ARB );
		
	}
    void CTorus::CreateVertex(int i, int u, int v, float r1, float r2, float ds1, float ds2)
    {
        CVertex& vert = m_Vertices._vertices[i];
        Vector<float>& n = m_Vertices._normals[i];
        vert.x = (r2 + r1 * cos(ds1 * v)) * cos(ds2 * u);
		vert.y = r1 * sin(ds1 * v);
		vert.z = (r2 + r1 * cos(ds1 * v)) * sin(ds2 * u);
		n.x = vert.x; n.y = vert.y; n.z = vert.z;
		CVectorMath<float>::Normalize(n);
    }
	void CTorus::Draw(void)
	{
		int error = glGetError();
		//glDisable(GL_LIGHTING);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glBegin(GL_POINTS);
		//	//glDrawArrays(GL_POINTS, 0,
		//for (CVertices::VertexArr::iterator it =  m_Vertices._vertices.begin(); it != m_Vertices._vertices.end(); it++)
		//{
		//	glVertex3f(it->x, it->y, it->z);
		//}
		//glEnd();
		glDisable(GL_BLEND);
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		error = glGetError();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState( GL_NORMAL_ARRAY );	
		error = glGetError();
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBONormals );
		glNormalPointer(GL_FLOAT, sizeof(CVertices::VectorArr::value_type), NULL);
		error = glGetError();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Vertices._vertices.size());
		error = glGetError();
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState( GL_NORMAL_ARRAY );	
	}

}