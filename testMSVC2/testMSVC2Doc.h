
// testMSVC2Doc.h : interface of the CtestMSVC2Doc class
//


#pragma once
#include "GLProgram.h"

class CtestMSVC2Doc : public CRichEditDoc
{
protected: // create from serialization only
	CtestMSVC2Doc();
	DECLARE_DYNCREATE(CtestMSVC2Doc)

// Attributes
public:
	Scene::CGLShader* m_pShader;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CtestMSVC2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBuildCompile();
	afx_msg void OnBuildBuildprogram();
};


