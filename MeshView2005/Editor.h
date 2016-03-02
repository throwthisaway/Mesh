#pragma once
#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include <string>
namespace Scene
{

class CEditor
{
	//HWND m_hWnd;
	std::string& m_szData;
public:
	CEditor(const std::string& title, const std::string& str);
	void Set(const std::string& title, const std::string& str);
	virtual ~CEditor(void);
};

}

