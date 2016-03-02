// MFCPGLightEnable.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "MFCPGLightEnable.h"


// CMFCPGLightEnable

CMFCPGLightEnable::CMFCPGLightEnable(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData,
									 LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : 
	CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData,lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPGLightEnable::~CMFCPGLightEnable()
{
}


// CMFCPGLightEnable member functions

BOOL CMFCPGLightEnable::OnUpdateValue(void)
{
	BOOL bResult = CMFCPropertyGridProperty::OnUpdateValue();
	if (IsModified())
	{
		LightSource* pLSP = (LightSource*)GetData();
		ASSERT(pLSP);
		pLSP->enabled = this->GetValue().boolVal;
	}
	return bResult;
}

void CMFCPGLightEnable::SetData(DWORD_PTR dwData)
{
	CMFCPropertyGridProperty::SetData(dwData);
	LightSource* light = (LightSource*)dwData;
	this->SetValue(_variant_t(light->enabled));
}
