//#ifdef WIN32
#include "StdAfx.h"
//#endif
#include "Editor.h"

namespace Scene
{
std::string emptyString("");
CEditor::CEditor(const std::string& title, const std::string& str):/* m_hWnd(0),*/ m_szData(emptyString)
{
	//HWND parent = NULL;
	//m_hWnd = ::CreateWindowA("Edit",title.c_str(), WS_TILEDWINDOW | WS_VISIBLE |/*WS_SIZEBOX | WS_MAXIMIZEBOX |WS_MINIMIZEBOX |*/ ES_MULTILINE | ES_WANTRETURN , 0, 0, 600, 400, NULL, NULL, NULL, NULL);
	Set(title, str);
}

CEditor::~CEditor(void)
{
	//::DestroyWindow(m_hWnd);
}

void CEditor::Set(const std::string& title, const std::string& str)
{
	m_szData = str;
	//::SetWindowTextA(m_hWnd, m_szData.c_str());
}

}