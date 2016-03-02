#pragma once
#include "GLScene.h"
#include "VTest1.h"
// CGLVTest1Doc document

class CGLVTest1Doc : public CDocument
{
	DECLARE_DYNCREATE(CGLVTest1Doc)
	Scene::CGLScene* m_pScene;
	CVTest1 vtest;
public:
	CGLVTest1Doc();
	virtual ~CGLVTest1Doc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
