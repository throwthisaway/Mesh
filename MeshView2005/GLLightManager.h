#pragma once
#include "w32types.h"
#include "LightSourceParameters.h"
#include <vector>

class CGLLightManager
{
	void EnableLighting();
	void DisableLighting();
public:
	std::vector<LightSource> _lightSources;
	int Add(LightSource& lightSource);
	void Remove(int n);
	void SetupSceneLighting(SceneLighting& sceneLight);
	CGLLightManager(void);
	~CGLLightManager(void);
};
