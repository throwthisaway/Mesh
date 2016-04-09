#include "EditLogger.h"
#include "HelperMacros.h"
#include "commctrl.h"

namespace Log
{

CEditLogger::CEditLogger(HWND parentHWnd) : m_hWnd(NULL), buffer(NULL)
{
	m_hWnd = ::CreateWindow(WC_EDIT,__T("Log"), WS_TILEDWINDOW | WS_VISIBLE | WS_VSCROLL | /*WS_SIZEBOX | WS_MAXIMIZEBOX |WS_MINIMIZEBOX |*/ ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL, 0, 650, 900, 300, parentHWnd, NULL, NULL, NULL);
}

void CEditLogger::Write(LPCTSTR msg)
{
	LPTSTR p;
	int len = ::GetWindowTextLength(m_hWnd);
	size_t newlen;
	if (len)
		p = new TCHAR[newlen = (len + strlen(msg) + 1)];
	else
		p = new TCHAR[newlen = (strlen(msg) + 1)];
	::GetWindowText(m_hWnd, p, (int)newlen);
	strcat(p+len, msg);
	::SetWindowText(m_hWnd, NULL);	
	DEL_ARRAY(buffer);
	buffer = p;
	::SetWindowText(m_hWnd, buffer);
}

void CEditLogger::Close()
{
	if (m_hWnd)
		::DestroyWindow(m_hWnd);
	DEL_ARRAY(buffer);
}

CEditLogger::~CEditLogger(void)
{
	Close();
}

void CEditLogger::Flush(void) const
{
	::SendMessage(m_hWnd, WM_VSCROLL, SB_BOTTOM, 0);
}

}
