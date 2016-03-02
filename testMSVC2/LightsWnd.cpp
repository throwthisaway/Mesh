// LightsWnd.cpp : implementation file
//
#include "stdafx.h"
#include "testMSVC2.h"
#include "LightsWnd.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "AppConstants.h"
#include "MFCPGLightEnable.h"
#include "MFCOnUpdateValueCallbackProperty.h"

// CLightsWnd

IMPLEMENT_DYNAMIC(CLightsWnd, CDockablePane)

CLightsWnd::CLightsWnd() : _lightCount(0)
{

}

CLightsWnd::~CLightsWnd()
{
}


BEGIN_MESSAGE_MAP(CLightsWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BTNADDLIGHT, OnAdd)
	ON_UPDATE_COMMAND_UI(ID_BTNADDLIGHT, OnUpdateAdd)
END_MESSAGE_MAP()



// CLightsWnd message handlers



int CLightsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	int id = 0;
	if (!m_wndLightsList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ++id))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	//InitLightsList();
	_toolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	_toolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	_toolBar.CleanUpLockedImages();
	_toolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_LIGHTSTOOLBAR_HC : IDR_LIGHTS, 0, 0, TRUE /* Locked */);

	_toolBar.SetPaneStyle(_toolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	_toolBar.SetPaneStyle(_toolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_toolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	_toolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}
void CLightsWnd::OnUpdateAmbient(DWORD_PTR dwData, const COLORREF color)
{
	LightSource* light = (LightSource*)dwData;
	Vector<float>& c = light->ambient;
	c.x = R(color); c.y = G(color); c.z = B(color);
	light->Ambient();
}
void CLightsWnd::OnUpdateDiffuse(DWORD_PTR dwData, const COLORREF color)
{
	LightSource* light = (LightSource*)dwData;
	Vector<float>& c = light->diffuse;
	c.x = R(color); c.y = G(color); c.z = B(color);
	light->Diffuse();
}
void CLightsWnd::OnUpdateSpecular(DWORD_PTR dwData, const COLORREF color)
{
	LightSource* light = (LightSource*)dwData;
	Vector<float>& c = light->specular;
	c.x = R(color); c.y = G(color); c.z = B(color);
	light->Specular();
}
void CLightsWnd::AddLight(LightSource& light)
{
	if (light.n >= CAppConstants::MaxLights || light.n >= CAppConstants::DefLights)
		return;
	CString lightName;
	BOOL bNameValid = lightName.LoadString(IDS_LIGHT);
	ASSERT(bNameValid);
	CString tempStr;
	tempStr.Format(_T("%s%d"), lightName, light.n);
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(tempStr);
	bNameValid = tempStr.LoadString(IDS_ENABLED);
	ASSERT(bNameValid);
	CMFCPGLightEnable* pPropLEnable = new CMFCPGLightEnable(tempStr, _variant_t(light.enabled), _T(""));
	pPropLEnable->SetData((DWORD_PTR)&light);
	pGroup1->AddSubItem(pPropLEnable);
	//CMFCPropertyGridColorProperty::m_strFormatLong = "%d %d %d %d";
	// Ambient...
	bNameValid = tempStr.LoadString(IDS_LIGHT_AMBIENT);
	ASSERT(bNameValid);
	CMFCOnUpdateValueCallbackColorProperty* pColorProp = new CMFCOnUpdateValueCallbackColorProperty(tempStr, RGB(light.ambient.x*255.0, light.ambient.y*255.0, light.ambient.z*255.0)/* | (DWORD)(light.ambient.w*255.0)<<24*/, 
		CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback(OnUpdateAmbient),
		NULL, _T(""), (DWORD_PTR)&light.ambient);
	bNameValid = tempStr.LoadString(IDS_OTHER);
	ASSERT(bNameValid);
	pColorProp->EnableOtherButton(tempStr);
	pColorProp->SetData((DWORD_PTR)&light);
	pGroup1->AddSubItem(pColorProp);
	
	// Diffuse...
	bNameValid = tempStr.LoadString(IDS_LIGHT_DIFFUSE);
	ASSERT(bNameValid);
	pColorProp = new CMFCOnUpdateValueCallbackColorProperty(tempStr, RGB(light.diffuse.x*255.0, light.diffuse.y*255.0, light.diffuse.z*255.0) /*| (DWORD)(light.diffuse.w*255.0)<<24*/,
		CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback(OnUpdateDiffuse),
		NULL, _T(""), (DWORD_PTR)&light.diffuse);
	bNameValid = tempStr.LoadString(IDS_OTHER);
	ASSERT(bNameValid);
	pColorProp->EnableOtherButton(tempStr);
	pColorProp->SetData((DWORD_PTR)&light);
	pGroup1->AddSubItem(pColorProp);

	// Specular...
	bNameValid = tempStr.LoadString(IDS_LIGHT_SPECULAR);
	ASSERT(bNameValid);
	pColorProp = new CMFCOnUpdateValueCallbackColorProperty(tempStr, RGB(light.specular.x*255.0, light.specular.y*255.0, light.specular.z*255.0) /*| (DWORD)(light.specular.w*255.0)<<24*/, 
		CMFCOnUpdateValueCallbackColorProperty::OnUpdateValueCallback(OnUpdateSpecular),
		NULL,  _T(""), (DWORD_PTR)&light.specular);
	bNameValid = tempStr.LoadString(IDS_OTHER);
	ASSERT(tempStr.LoadString(IDS_OTHER));
	pColorProp->EnableOtherButton(tempStr);
	pColorProp->SetData((DWORD_PTR)&light);
	pGroup1->AddSubItem(pColorProp);

	// Position...
	bNameValid = tempStr.LoadString(IDS_POSITION);
	ASSERT(bNameValid);
	CMFCPropertyGridProperty* pPosGroup = new CMFCPropertyGridProperty(tempStr);
	bNameValid = tempStr.LoadString(IDS_POSITIONX);
	ASSERT(bNameValid);
	CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.position.x, _T(""), (DWORD_PTR)&light.position.x);
	pPosGroup->AddSubItem(pPos);
	bNameValid = tempStr.LoadString(IDS_POSITIONY);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.position.y, _T(""), (DWORD_PTR)&light.position.y);
	pPosGroup->AddSubItem(pPos);
	bNameValid = tempStr.LoadString(IDS_POSITIONZ);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.position.z, _T(""), (DWORD_PTR)&light.position.z);
	pPosGroup->AddSubItem(pPos);
	bNameValid = tempStr.LoadString(IDS_POSITIONW);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.position.w, _T(""), (DWORD_PTR)&light.position.w);
	pPosGroup->AddSubItem(pPos);
	pGroup1->AddSubItem(pPosGroup);

	// Spot direction...
	bNameValid = tempStr.LoadString(IDS_SPOTDIRECTION);
	ASSERT(bNameValid);
	pPosGroup = new CMFCPropertyGridProperty(tempStr);
	bNameValid = tempStr.LoadString(IDS_POSITIONX);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.spotDirection.x, _T(""), (DWORD_PTR)&light.spotDirection.x);
	pPosGroup->AddSubItem(pPos);
	bNameValid = tempStr.LoadString(IDS_POSITIONY);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.spotDirection.y, _T(""), (DWORD_PTR)&light.spotDirection.y);
	pPosGroup->AddSubItem(pPos);
	bNameValid = tempStr.LoadString(IDS_POSITIONZ);
	ASSERT(bNameValid);
	pPos = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.spotDirection.z, _T(""), (DWORD_PTR)&light.spotDirection.z);
	pPosGroup->AddSubItem(pPos);
	pGroup1->AddSubItem(pPosGroup);

	// Spot exponent...
	bNameValid = tempStr.LoadString(IDS_SPOTEXPONENT);
	ASSERT(bNameValid);
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.spotExponent, _T(""), (DWORD_PTR)&light.spotExponent);
	pGroup1->AddSubItem(pProp);
	// Spot cutoff...
	bNameValid = tempStr.LoadString(IDS_SPOTCUTOFF);
	ASSERT(bNameValid);
	pProp = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.spotCutoff, _T(""), (DWORD_PTR)&light.spotCutoff);
	pGroup1->AddSubItem(pProp);
	// Constant attenuation...
	bNameValid = tempStr.LoadString(IDS_CONSTATT);
	ASSERT(bNameValid);
	pProp = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.constantAttenuation, _T(""), (DWORD_PTR)&light.constantAttenuation);
	pGroup1->AddSubItem(pProp);
	// Linear attenuation...
	bNameValid = tempStr.LoadString(IDS_LINEARATT);
	ASSERT(bNameValid);
	pProp = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.linearAttenuation, _T(""), (DWORD_PTR)&light.linearAttenuation);
	pGroup1->AddSubItem(pProp);
	// Quadratic attenuation...
	bNameValid = tempStr.LoadString(IDS_QUADRATICATT);
	ASSERT(bNameValid);
	pProp = new CMFCPropertyGridProperty(tempStr, (_variant_t)light.quadraticAttenuation, _T(""), (DWORD_PTR)&light.quadraticAttenuation);
	pGroup1->AddSubItem(pProp);
	m_wndLightsList.AddProperty(pGroup1);
}

void CLightsWnd::Clear()
{
	for (int i = m_wndLightsList.GetPropertyCount() - 1;i>=0;--i)
	{
		CMFCPropertyGridProperty* pProp = m_wndLightsList.GetProperty(i);
		m_wndLightsList.DeleteProperty(pProp, 0, 0);
	}
}

void CLightsWnd::InitLightsList(Scene::CGLScene * pGLScene)
{
	// TODO:: SetPropListFont()
	m_wndLightsList.SetVSDotNetLook();
	m_wndLightsList.EnableHeaderCtrl(FALSE);
	Clear();
	for (std::vector<LightSource>::iterator it = pGLScene->m_LightManager._lightSources.begin(); it != pGLScene->m_LightManager._lightSources.end(); it++)
	{
		AddLight(*it);
	}
}

void CLightsWnd::AdjustLayout(void)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = _toolBar.CalcFixedLayout(FALSE, TRUE).cy;
	_toolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndLightsList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CLightsWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CLightsWnd::OnAdd()
{
	// TODO: Add your command handler code here
}

void CLightsWnd::OnUpdateAdd(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}