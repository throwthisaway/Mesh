#include "StdAfx.h"
#include "GLLightManager.h"
#include <gl\gl.h>
#include "AppConstants.h"

CGLLightManager::CGLLightManager(void)
{
}

CGLLightManager::~CGLLightManager(void)
{
}

void CGLLightManager::EnableLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}
void CGLLightManager::DisableLighting()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
}
int CGLLightManager::Add(LightSource& lightSource)
{
	if (_lightSources.size() > CAppConstants::MaxLights)
		return -1;
	_lightSources.push_back(lightSource);
	if (_lightSources.size() == 1)
		EnableLighting();
	return (int)_lightSources.size();
}
void CGLLightManager::Remove(int n)
{
	if (_lightSources.size() < 1 || (size_t)n >= _lightSources.size())
		return;
	if (_lightSources.size() == 1)
	{
		_lightSources.pop_back();
		DisableLighting();
		return;
	}
	_lightSources[n] = _lightSources[_lightSources.size() - 1];
	_lightSources.pop_back();
}

//void CGLLightManager::Model(eLIGHTMODEL model)
//{
//	// TODO::
//}

void CGLLightManager::SetupSceneLighting(SceneLighting& sceneLight)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sceneLight.ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, sceneLight.localViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, sceneLight.twoSide);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, sceneLight.colorControl);
}