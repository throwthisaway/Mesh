#pragma once
#include "afxpropertygridctrl.h"
#include <functional>

class CMFCOnUpdateValueCallback
{
public:
	typedef std::tr1::function<void (DWORD_PTR, const COleVariant&)> OnUpdateValueCallback;
	OnUpdateValueCallback m_onUpdateValue;
	CMFCOnUpdateValueCallback(OnUpdateValueCallback onUpdateCallback) : m_onUpdateValue(onUpdateCallback) {};
};

class CMFCOnUpdateValueCallbackProperty :
	public CMFCPropertyGridProperty, public CMFCOnUpdateValueCallback
{
public:
	CMFCOnUpdateValueCallbackProperty(const CString& strName, const COleVariant& varValue, CMFCOnUpdateValueCallback::OnUpdateValueCallback& onUpdateCallback, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	virtual ~CMFCOnUpdateValueCallbackProperty(void);
	virtual BOOL OnUpdateValue(void);
};

class CMFCOnUpdateValueCallbackColorProperty :
	public CMFCPropertyGridColorProperty
{
public:
	typedef std::tr1::function<void (DWORD_PTR, const COLORREF)> OnUpdateValueCallback;
	OnUpdateValueCallback m_onUpdateValue;
	CMFCOnUpdateValueCallbackColorProperty(const CString& strName, 
		const COLORREF& color, 
		CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback& onUpdateCallback,
		CPalette* pPalette = NULL, 
		LPCTSTR lpszDescr = NULL,
		DWORD_PTR dwData = 0);
	virtual BOOL OnUpdateValue(void);
};
