#pragma once
#include <vector>
#include "Viewport.h"

namespace Scene
{
class CGLViewportManager :
	public std::vector<CCamera>
{
	int m_act;
public:
	inline CCamera GetActive(void) { return this->operator[](m_act); };
	inline void SetActive(int i) { m_act = i; };
	CGLViewportManager(void);
	~CGLViewportManager(void);
};
}

