// MFCButtonPropertyGridProperty.cpp : implementation file
//

#include "stdafx.h"
#include "testMSVC2.h"
#include "MFCButtonPropertyGridProperty.h"

BEGIN_MESSAGE_MAP(CAdjustButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAdjustEdit, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CAdjustEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		static CPoint pointPrev(point.x, point.y);
		int dx = point.x - pointPrev.x;
		int dy = pointPrev.y - point.y;
		m_pProperty->AdjustValue(dx, dy);
	}
}
void CAdjustEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
}
void CAdjustEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
}
//BOOL CAdjustButton::PreTranslateMessage(MSG* pMsg)
//{
//	switch(pMsg->message)
//	{
//	case WM_LBUTTONDOWN:
//		OnLButtonDown((UINT) pMsg->wParam, CPoint(pMsg->lParam));
//		break;
//	case WM_LBUTTONUP:
//		OnLButtonUp((UINT) pMsg->wParam, CPoint(pMsg->lParam));
//		break;
//	case WM_MOUSEMOVE:
//		OnMouseMove((UINT) pMsg->wParam, CPoint(pMsg->lParam));
//		break;
//	}
//	if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
//		CButton::DefWindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
////	WindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
//	return CButton::PreTranslateMessage(pMsg);
//}
BOOL CAdjustButton::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	BOOL res = FALSE;
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
		res = CButton::OnWndMsg(message, wParam, lParam, pResult);
	switch(message)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown((UINT) wParam, CPoint(lParam));
		res = TRUE;
		break;
	case WM_LBUTTONUP:
		OnLButtonUp((UINT) wParam, CPoint(lParam));
		res = TRUE;
		break;
	case WM_MOUSEMOVE:
		OnMouseMove((UINT) wParam, CPoint(lParam));
		res = TRUE;
		break;
	}
	return res;
}

void CAdjustButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
}
void CAdjustButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
}
void CAdjustButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		static CPoint pointPrev(point.x, point.y);
		int dx = point.x - pointPrev.x;
		int dy = pointPrev.y - point.y;
		m_pProperty->AdjustValue(dx, dy);
	}
}
// CMFCButtonPropertyGridProperty

CMFCButtonPropertyGridProperty::CMFCButtonPropertyGridProperty(const CString& strName, const COleVariant& varValue, CMFCOnUpdateValueCallback::OnUpdateValueCallback& onUpdateCallback, LPCTSTR lpszDescr, DWORD_PTR dwData,
															   LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
		CMFCOnUpdateValueCallbackProperty(strName, varValue, onUpdateCallback, lpszDescr, dwData,lpszEditMask, lpszEditTemplate, lpszValidChars),
		_bSelectAll(TRUE)
{
	hCursor  = ::LoadCursor(::AfxGetInstanceHandle(), IDC_SIZENS); 
}

CMFCButtonPropertyGridProperty::~CMFCButtonPropertyGridProperty()
{
}
void CMFCButtonPropertyGridProperty::AdjustButtonRect()
{
	CMFCPropertyGridProperty::AdjustButtonRect();
	m_rectButton.left -= m_rectButton.Width();
	//if (!m_button.m_hWnd)
	//{
	//	m_button.m_pProperty = this;
	//	m_button.Create(NULL, BS_FLAT | BS_BITMAP | WS_VISIBLE/* | WS_CHILD*/ | BS_CENTER, m_rectButton, m_pWndList, 0);
	//	m_button.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ADJUSTBTN)));
	//	return;
	//}
	//m_button.SetWindowPos(m_pWndList, m_rectButton.left, m_rectButton.top, m_rectButton.Width(), m_rectButton.Height(), 0);
}

void CMFCButtonPropertyGridProperty::OnClickButton(CPoint point)
{
	//m_button.SetFocus();
	::OutputDebugString(_T("Click"));
	//_CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, "Click");
	::SetCursor(hCursor);
}

void CMFCButtonPropertyGridProperty::OnDrawButton(CDC* pDC, CRect rectButton)
{
	CMFCPropertyGridProperty::OnDrawButton(pDC, rectButton);
	//m_button.SetWindowPos(m_pWndList, rectButton.left, rectButton.top, rectButton.Width(), rectButton.Height(), 0);
}
void CMFCButtonPropertyGridProperty::AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin)
{
	CMFCPropertyGridProperty::AdjustInPlaceEditRect(rectEdit, rectSpin);
	/*if (m_button.m_hWnd && !HasButton())
	{
		CRect rectBtn(m_Rect);
		rectBtn.right = rectEdit.right;
		rectBtn.left = rectBtn.right - rectBtn.Height();
		rectBtn.top ++;
		rectEdit.right = rectBtn.left;
		m_button.SetWindowPos(m_pWndList, rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height(), SWP_SHOWWINDOW);
	}*/
}
BOOL CMFCButtonPropertyGridProperty::OnEdit(LPPOINT lptClick)
{
	//if (!m_button.m_hWnd && !HasButton())
	//{
	//	m_button.m_pProperty = this;
	//	CRect rectBtn(m_Rect);
	//	rectBtn.right = m_Rect.right;
	//	rectBtn.left = rectBtn.right - rectBtn.Height();
	//	rectBtn.top ++;
	//	m_button.Create(NULL, BS_FLAT | BS_BITMAP | WS_VISIBLE/* | WS_CHILD*/ | BS_CENTER, rectBtn, m_pWndList, 0);
	//	m_button.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ADJUSTBTN)));
	//	return TRUE;
	//}
	return CMFCPropertyGridProperty::OnEdit(lptClick);
}
BOOL CMFCButtonPropertyGridProperty::OnClickValue(UINT uiMsg, CPoint point)
{
	BOOL res = TRUE; //CMFCPropertyGridProperty::OnClickValue(uiMsg, point);
	// Select the value on activate...
	if (m_pWndInPlace && _bSelectAll)
		((CEdit*)m_pWndInPlace)->SetSel(0, -1);
	return res;
}
void CMFCButtonPropertyGridProperty::OnDestroyWindow()
{
	CMFCPropertyGridProperty::OnDestroyWindow();
	::SetCursor(NULL);
	::DeleteObject(hCursor);
	/*if (m_button.m_hWnd)
	{
		m_button.DestroyWindow();
	}*/
}

void CMFCButtonPropertyGridProperty::AdjustValue(int dx, int dy)
{
	COleVariant var = GetValue();
	var.fltVal += 0.01f * (float)dx;
	SetValue(var);
	m_pWndList->OnPropertyChanged(this);
	//OnUpdateValue();
}
CWnd* CMFCButtonPropertyGridProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	switch (m_varValue.vt)
	{
	case VT_BSTR:
	case VT_R4:
	case VT_R8:
	case VT_UI1:
	case VT_I2:
	case VT_INT:
	case VT_UINT:
	case VT_I4:
	case VT_UI2:
	case VT_UI4:
	case VT_BOOL:
		break;

	default:
		if (!m_bIsValueList)
		{
			return NULL;
		}
	}

	CEdit* pWndEdit = NULL;

	if (!m_strEditMask.IsEmpty() || !m_strEditTempl.IsEmpty() || !m_strValidChars.IsEmpty())
	{
		CMFCMaskedEdit* pWndEditMask = new CMFCMaskedEdit;
		pWndEditMask->EnableSetMaskedCharsOnly(FALSE);
		pWndEditMask->EnableGetMaskedCharsOnly(FALSE);

		if (!m_strEditMask.IsEmpty() && !m_strEditTempl.IsEmpty())
		{
			pWndEditMask->EnableMask(m_strEditMask, m_strEditTempl, _T(' '));
		}

		if (!m_strValidChars.IsEmpty())
		{
			pWndEditMask->SetValidChars(m_strValidChars);
		}

		pWndEdit = pWndEditMask;
	}
	else
	{
		pWndEdit = new CAdjustEdit;
		((CAdjustEdit*)pWndEdit)->m_pProperty = this;
	}

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL;

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create(dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);

	bDefaultFormat = TRUE;
	return pWndEdit;
}
// CMFCButtonPropertyGridProperty member functions


