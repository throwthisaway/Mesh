#include "StdAfx.h"
#include "Sphere.h"
#include <vector>
#include "HelperMacros.h"
using namespace DirectX;
namespace Scene
{
	GLuint CSphere::vbo[2];
	BOOL CSphere::init = FALSE;
	std::vector<FLOAT> CSphere::vertices, CSphere::normals;
	static const int depth/*T*/ = 2;
	static const FLOAT radius/*R*/ = 0.295f,
		X = 0.525731112119133606f,
		Z = 0.850650808352039932f,
		vdata[][3] = { { -X, 0.0, Z },
		{ X, 0.0, Z },
		{ -X, 0.0, -Z },
		{ X, 0.0, -Z },
		{ 0.0, Z, X },
		{ 0.0, Z, -X },
		{ 0.0, -Z, X },
		{ 0.0, -Z, -X },
		{ Z, X, 0.0 },
		{ -Z, X, 0.0 },
		{ Z, -X, 0.0 },
		{ -Z, -X, 0.0 } };
	static const int tindices[][3] = {
			{ 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
			{ 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
			{ 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
			{ 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 } };
	void CSphere::Subdivide(XMVECTOR& v1, XMVECTOR& v2, XMVECTOR& v3, int depth) {
		if (depth <= 0) {
			vertices.push_back(XMVectorGetX(v1));
			vertices.push_back(XMVectorGetY(v1));
			vertices.push_back(XMVectorGetZ(v1));
			vertices.push_back(XMVectorGetX(v2));
			vertices.push_back(XMVectorGetY(v2));
			vertices.push_back(XMVectorGetZ(v2));
			vertices.push_back(XMVectorGetX(v3));
			vertices.push_back(XMVectorGetY(v3));
			vertices.push_back(XMVectorGetZ(v3));
			XMVECTOR n = XMVector3Normalize(v1);
			normals.push_back(XMVectorGetX(n));
			normals.push_back(XMVectorGetY(n));
			normals.push_back(XMVectorGetZ(n));
			n = XMVector3Normalize(v2);
			normals.push_back(XMVectorGetX(n));
			normals.push_back(XMVectorGetY(n));
			normals.push_back(XMVectorGetZ(n));
			n = XMVector3Normalize(v3);
			normals.push_back(XMVectorGetX(n));
			normals.push_back(XMVectorGetY(n));
			normals.push_back(XMVectorGetZ(n));
			return;
		}

		XMVECTOR v12 = v1 + v2, v23 = v2 + v3, v31 = v3 + v1;
		v12 = XMVector3Normalize(v12);
		v23 = XMVector3Normalize(v23);
		v31 = XMVector3Normalize(v31);
		Subdivide(v1, v12, v31, depth - 1);
		Subdivide(v2, v23, v12, depth - 1);
		Subdivide(v3, v31, v23, depth - 1);
		Subdivide(v12, v23, v31, depth - 1);
	}
	void CSphere::Init(void)
	{
		if (vertices.size() > 0)
			return;
		for (int i = 0; i < sizeof(tindices) / sizeof(tindices[0]); i++) {
			XMVECTOR v1 = XMVectorSet(vdata[tindices[i][0]][0], vdata[tindices[i][0]][1], vdata[tindices[i][0]][2], 0.0f),
				v2 = XMVectorSet(vdata[tindices[i][1]][0], vdata[tindices[i][1]][1], vdata[tindices[i][1]][2], 1.0f),
				v3 = XMVectorSet(vdata[tindices[i][2]][0], vdata[tindices[i][2]][1], vdata[tindices[i][2]][2], 1.0f);
			Subdivide(v1, v2, v3, depth);
		}
	}

	CSphere::CSphere(ProgramMap& shaders, Vector3<FLOAT>& pos, Vector3<FLOAT>& rot, Vector3<FLOAT>& scale, Vector<FLOAT>& col) :
		pos(pos),
		rot(rot),
		scale(scale),
		col(col),
		shininess(80.0f),
		shaders(shaders)
	{
		pProgram = shaders[_T("specular.program")];
		if (!init)
		{
			Init();
			glGenBuffersARB(VBOS, vbo);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * sizeof(FLOAT), &(*vertices.begin()), GL_STATIC_DRAW_ARB);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, normals.size() * sizeof(FLOAT), &(*normals.begin()), GL_STATIC_DRAW_ARB);
			_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			init = TRUE;
		}
	}


	CSphere::~CSphere()
	{
	}
	void CSphere::Draw(void)
	{
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glPushMatrix();
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glTranslatef(pos[0], pos[1], pos[2]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glScalef(scale[0], scale[1], scale[2]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		//glEnable(GL_COLOR_MATERIAL);
		//glColorMaterial(GL_FRONT, GL_SPECULAR);
		//_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		//glDisable(GL_CULL_FACE);
		glEnable(GL_CULL_FACE);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glEnable(GL_BLEND);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		pProgram->UseProgram();
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glNormalPointer(GL_FLOAT, 0, (char *)NULL);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glColor4f(0.45f, 0.45f, 0.45f, 1.0f);

		glCullFace(GL_FRONT);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() * 3);
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));

		//glCullFace(GL_BACK);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() * 3);
		//_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
		glPopMatrix();
		_ASSERT_EXPR_A(!(error = glGetError()), (const char*)gluErrorString(error));
	}
	void CSphere::Cleanup(void)
	{
		if (init)
		{
			glDeleteBuffersARB(VBOS, vbo);
		}
	}
}