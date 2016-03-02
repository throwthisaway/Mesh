#include "StdAfx.h"
#include "MFCOnUpdateValueCallbackProperty.h"

//Create a new class derived from CMFCPropertyGridProperty.
//Override HasButton() and OnClickButton() 
CMFCOnUpdateValueCallbackProperty::CMFCOnUpdateValueCallbackProperty(const CString& strName, const COleVariant& varValue, CMFCOnUpdateValueCallback::OnUpdateValueCallback& onUpdateCallback, LPCTSTR lpszDescr, DWORD_PTR dwData,
																	 LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars):
	CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData,lpszEditMask, lpszEditTemplate, lpszValidChars),
	CMFCOnUpdateValueCallback(onUpdateCallback)
{
}

CMFCOnUpdateValueCallbackProperty::~CMFCOnUpdateValueCallbackProperty(void)
{
	//DWORD prev;
	//_asm
	//{
	//	mov EAX, FS:[0]
	//	mov prev, EAX
	//}

}

BOOL CMFCOnUpdateValueCallbackProperty::OnUpdateValue(void)
{
	BOOL bResult = CMFCPropertyGridProperty::OnUpdateValue();
	if (IsModified())
	{
		DWORD_PTR dwData = GetData();
		ASSERT(dwData);
		ASSERT(CMFCOnUpdateValueCallback::m_onUpdateValue);
		if (CMFCOnUpdateValueCallback::m_onUpdateValue)
			CMFCOnUpdateValueCallback::m_onUpdateValue(dwData, this->GetValue());
	}
	return bResult;
}

CMFCOnUpdateValueCallbackColorProperty::CMFCOnUpdateValueCallbackColorProperty(const CString& strName, 
		const COLORREF& color, 
		CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback& onUpdateCallback,
		CPalette* pPalette, 
		LPCTSTR lpszDescr,
		DWORD_PTR dwData) :
			CMFCPropertyGridColorProperty(strName, 
			color, 
			pPalette, 
			lpszDescr,
			dwData), 
			m_onUpdateValue(onUpdateCallback)
{
}

BOOL CMFCOnUpdateValueCallbackColorProperty::OnUpdateValue(void)
{
	BOOL bResult = CMFCPropertyGridColorProperty::OnUpdateValue();
	if (IsModified())
	{
		DWORD_PTR dwData = GetData();
		ASSERT(dwData);
		ASSERT(m_onUpdateValue);
		if (m_onUpdateValue)
			m_onUpdateValue(dwData, this->GetColor());
	}
	return bResult;
}
