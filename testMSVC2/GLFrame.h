#pragma once


// CGLFrame

class CGLFrame : public CMDIChildWndEx
{
	//DECLARE_DYNAMIC(CGLFrame)
	DECLARE_DYNCREATE(CGLFrame)
public:
	CGLFrame();
	virtual ~CGLFrame();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


