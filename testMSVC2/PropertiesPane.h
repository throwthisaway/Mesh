#pragma once

class CPropertiesToolBar2 : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CPropertiesPane
class CPropertiesPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertiesPane)
	void AdjustLayout();
	void SetPropListFont();
	void InitPropList();
public:
	CPropertiesPane();
	virtual ~CPropertiesPane();
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar2 m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	// Attributes
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	void ClearProperties(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


