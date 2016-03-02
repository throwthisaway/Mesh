
// testMSVC2Doc.cpp : implementation of the CtestMSVC2Doc class
//

#include "stdafx.h"
#include "testMSVC2.h"

#include "testMSVC2Doc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestMSVC2Doc

IMPLEMENT_DYNCREATE(CtestMSVC2Doc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CtestMSVC2Doc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_BUILD_COMPILE, &CtestMSVC2Doc::OnBuildCompile)
	ON_COMMAND(ID_BUILD_BUILDPROGRAM, &CtestMSVC2Doc::OnBuildBuildprogram)
END_MESSAGE_MAP()


// CtestMSVC2Doc construction/destruction

CtestMSVC2Doc::CtestMSVC2Doc()
{
	// TODO: add one-time construction code here

}

CtestMSVC2Doc::~CtestMSVC2Doc()
{
}

BOOL CtestMSVC2Doc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CtestMSVC2Doc::CreateClientItem(REOBJECT* preo) const
{
	return new CtestMSVC2CntrItem(preo, const_cast<CtestMSVC2Doc*>(this));
}




// CtestMSVC2Doc serialization

void CtestMSVC2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::m_bRTF = FALSE;
	CRichEditDoc::Serialize(ar);
	CRichEditDoc::SetModifiedFlag(FALSE);
}


// CtestMSVC2Doc diagnostics

#ifdef _DEBUG
void CtestMSVC2Doc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CtestMSVC2Doc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CtestMSVC2Doc commands

void CtestMSVC2Doc::OnBuildCompile()
{
	m_pShader->Compile();
}

void CtestMSVC2Doc::OnBuildBuildprogram()
{
    m_pShader->Load(m_pShader->_fname.c_str());
	m_pShader->m_glProgram.CreateAndLink();
}
