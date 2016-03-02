#pragma once
#include "LightSourceParameters.h"

// CMFCPGLightEnable command target

class CMFCPGLightEnable : public CMFCPropertyGridProperty
{
public:
	CMFCPGLightEnable(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual BOOL OnUpdateValue(void);
	void SetData(DWORD_PTR dwData);
	virtual ~CMFCPGLightEnable();
};


