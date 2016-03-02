// PropertyGridPane.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "PropertyGridPane.h"
#include "propertyGrid.h"

// CPropertyGridPane

IMPLEMENT_DYNAMIC(CPropertyGridPane, CDockablePane)

CPropertyGridPane::CPropertyGridPane()
{

}

CPropertyGridPane::~CPropertyGridPane()
{
}


BEGIN_MESSAGE_MAP(CPropertyGridPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CPropertyGridPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndPropGrid.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertyGridPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	HWND hPropertyGrid = New_PropertyGrid(this->GetSafeHwnd(), (DWORD)2/*?*/);
	if (!m_wndPropGrid.Attach(hPropertyGrid))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	
	AdjustLayout();
	InitPropList();
	m_wndPropGrid.ShowWindow(TRUE);
	return 0;
}

void CPropertyGridPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropGrid.SetFocus();
}

void CPropertyGridPane::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertyGridPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertyGridPane::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropGrid.SetFont(&m_fntPropList);
}

void CPropertyGridPane::InitPropList()
{
	SetPropListFont();
    static PROPGRIDITEM item = {_T("Catalog1"), _T("Dummy"), NULL, _T("Test property"), (LPARAM)_T("value"), PIT_EDIT};
    // TODO::
    PropGrid_AddItem(m_wndPropGrid.GetSafeHwnd(), &item);
	 // TODO::
	/*m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();*/
}

void CPropertyGridPane::ClearProperties()
{
	// TODO::
	//for (int i = m_wndPropList.GetPropertyCount() - 1;i>=0;--i)
	//{
	//	CMFCPropertyGridProperty* pProp = m_wndPropList.GetProperty(i);
	//	m_wndPropList.DeleteProperty(pProp, 0, 0);
	//}
}
// CPropertyGridPane message handlers




void CPropertyGridPane::OnDestroy()
{
	//HWND hWnd = m_wndPropGrid.GetSafeHwnd();
	//m_wndPropGrid.Detach();
	//::DestroyWindow(hWnd);
	CDockablePane::OnDestroy();
}
