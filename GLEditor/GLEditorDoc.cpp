// GLEditorDoc.cpp : implementation of the CGLEditorDoc class
//

#include "stdafx.h"
#include "GLEditor.h"

#include "GLEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLEditorDoc

IMPLEMENT_DYNCREATE(CGLEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGLEditorDoc, CDocument)
END_MESSAGE_MAP()


// CGLEditorDoc construction/destruction

CGLEditorDoc::CGLEditorDoc()
{
	// TODO: add one-time construction code here

}

CGLEditorDoc::~CGLEditorDoc()
{
}

BOOL CGLEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGLEditorDoc serialization

void CGLEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CGLEditorDoc diagnostics

#ifdef _DEBUG
void CGLEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGLEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGLEditorDoc commands
