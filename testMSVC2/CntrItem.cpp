
// CntrItem.cpp : implementation of the CtestMSVC2CntrItem class
//

#include "stdafx.h"
#include "testMSVC2.h"

#include "testMSVC2Doc.h"
#include "testMSVC2View.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestMSVC2CntrItem implementation

IMPLEMENT_SERIAL(CtestMSVC2CntrItem, CRichEditCntrItem, 0)

CtestMSVC2CntrItem::CtestMSVC2CntrItem(REOBJECT* preo, CtestMSVC2Doc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CtestMSVC2CntrItem::~CtestMSVC2CntrItem()
{
	// TODO: add cleanup code here
}


// CtestMSVC2CntrItem diagnostics

#ifdef _DEBUG
void CtestMSVC2CntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CtestMSVC2CntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

