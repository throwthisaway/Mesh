#pragma once

//class CPropertiesToolBar3 : public CMFCToolBar
//{
//public:
//	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
//	{
//		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
//	}
//
//	virtual BOOL AllowShowOnList() const { return FALSE; }
//};
// CPropertyGridPane

class CPropertyGridPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertyGridPane)
	void AdjustLayout();
	void InitPropList();
	void SetPropListFont();
public:
	CPropertyGridPane();
	virtual ~CPropertyGridPane();
	void ClearProperties(void);
protected:
	CFont m_fntPropList;
	CWnd m_wndPropGrid;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


