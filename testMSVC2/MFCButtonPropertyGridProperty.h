#pragma once
#include "MFCOnUpdateValueCallbackProperty.h"
#include <functional>
// CMFCButtonPropertyGridProperty command target

class CMFCButtonPropertyGridProperty;
class CAdjustButton : public CButton
{
public:
	CMFCButtonPropertyGridProperty* m_pProperty;
	/*typedef std::tr1::function<void (CMFCButtonPropertyGridProperty::)(int dx, int dy)> OnAdjustValueCallback;
	OnAdjustValueCallback m_onAdjustValue;*/
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CAdjustButton() : CButton(){};
	virtual ~CAdjustButton() {};
	DECLARE_MESSAGE_MAP()

};

class CAdjustEdit : public CEdit
{
public:
	CMFCButtonPropertyGridProperty* m_pProperty;
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

class CMFCButtonPropertyGridProperty : public CMFCOnUpdateValueCallbackProperty
{
	BOOL _bSelectAll;
	HCURSOR hCursor;
protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void AdjustButtonRect();
	virtual void OnDrawButton(CDC* pDC, CRect rectButton);
	virtual void OnClickButton(CPoint point);
	//CAdjustButton m_button;
	//CRect m_rectButton;
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
public:
	CMFCButtonPropertyGridProperty(const CString& strName, const COleVariant& varValue, CMFCOnUpdateValueCallback::OnUpdateValueCallback& onUpdateCallback, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	void AdjustValue(int dx, int dy);
	virtual BOOL OnClickValue(UINT uiMsg, CPoint point);
	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual void AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin);
	virtual void OnDestroyWindow();
	virtual ~CMFCButtonPropertyGridProperty();
};


