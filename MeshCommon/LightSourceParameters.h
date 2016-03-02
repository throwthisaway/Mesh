#pragma once
#include "stdafx.h"
#include "VectorMath.h"
#include <gl\gl.h>
#include "glext.h"
#include "HelperMacros.h"
typedef enum{LIGHT_DIFFUSE, LIGHT_SPOT}eLIGHTMODEL;
typedef struct st_LightSource
{
	GLint n;
	BOOL enabled;
	eLIGHTMODEL lightModel;
	Vector<GLfloat> ambient; // Acli
	Vector<GLfloat> diffuse; // Dcli
	Vector<GLfloat> specular; // Scli
	Vector<GLfloat> position; // Ppli
	Vector<GLfloat> spotDirection; // Sdli
	GLfloat spotExponent; // Srli
	GLfloat spotCutoff; // Crli
	// (range: [0.0,90.0], 180.0)
	GLfloat constantAttenuation; // K0
	GLfloat linearAttenuation; // K1
	GLfloat quadraticAttenuation;// K2
	struct st_LightSource() :
		n(0),
		enabled(TRUE),
		lightModel(LIGHT_DIFFUSE),
		ambient(.2f, .2f, .2f, .2f),
		diffuse(1.f, 1.f, 1.f, 1.0f),
		specular(.3f, .3f, .3f, 1.0f),
		position(3.0f, 3.0f, 40.0f, 0.0f),
		spotDirection(.0f, .0f, -1.f),
		spotExponent(0.5f),
		spotCutoff(45.f),
		constantAttenuation(1.0f),
		linearAttenuation(0.0f),
		quadraticAttenuation(0.0f)
	{
		Enable();
		Position();
		//SpotDirection(n, lightSource.spotDirection.x, lightSource.spotDirection.y, lightSource.spotDirection.z);
		//Ambient(n, lightSource.ambient.x, lightSource.ambient.y, lightSource.ambient.z);
		Diffuse(); 
		//Specular(n, lightSource.specular.x, lightSource.specular.y, lightSource.specular.z); 
		//SpotExponent(n, lightSource.spotExponent); 
		//SpotCutoff(n, lightSource.spotCutoff); 
		//ConstantAttenuation(n, lightSource.constantAttenuation); 
		//LinearAttenuation(n, lightSource.linearAttenuation); 
		//QuadraticAttenuation(n, lightSource.quadraticAttenuation);
	};
	~st_LightSource()
	{
		Disable();
	}
	inline void Enable(void)
	{
		glEnable(GL_LIGHT0 + n);
	}
	inline void Disable(void)
	{
		glDisable(GL_LIGHT0 + n);
	}
	inline void Position(void)
	{
		glLightfv(GL_LIGHT0 + n, GL_POSITION, position.arr);
	}
	inline void GetPos(Vector<GLfloat>& pos)
	{
		glGetLightfv(GL_LIGHT0 + n, GL_POSITION, pos.arr);
	}
	inline void SpotDirection(void)
	{
		glLightfv(GL_LIGHT0 + n, GL_SPOT_DIRECTION, spotDirection.arr);
	}
	inline void Ambient(void)
	{
		glLightfv(GL_LIGHT0 + n, GL_AMBIENT, ambient.arr);
	}
	inline void Diffuse(void)
	{
		glLightfv(GL_LIGHT0 + n, GL_DIFFUSE, diffuse.arr);
	}
	inline void Specular(void)
	{
		glLightfv(GL_LIGHT0 + n, GL_SPECULAR, specular.arr);
	}
	inline void SpotExponent(void)
	{
		glLightf(GL_LIGHT0 + n, GL_SPOT_EXPONENT, spotExponent);
	}
	inline void SpotCutoff(void)
	{
		glLightf(GL_LIGHT0 + n, GL_SPOT_CUTOFF, spotCutoff);
	}
	inline void ConstantAttenuation(void)
	{
		glLightf(GL_LIGHT0 + n, GL_CONSTANT_ATTENUATION, constantAttenuation);
	}
	inline void LinearAttenuation(void)
	{
		glLightf(GL_LIGHT0 + n, GL_LINEAR_ATTENUATION, linearAttenuation);
	}
	inline void QuadraticAttenuation(void)
	{
		glLightf(GL_LIGHT0 + n, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
	}
}LightSource;

typedef struct st_SceneLighting
{
	Vector<GLfloat> ambient;	// acs
	bool localViewer;		// vbs
	bool twoSide;			// tbs
	GLenum colorControl;		//ces
	struct st_SceneLighting() :
		ambient(.1f, .1f, .1f, 1.f),
		localViewer(TRUE),
		twoSide(FALSE),
		colorControl(GL_SINGLE_COLOR)
	{};
}SceneLighting;

