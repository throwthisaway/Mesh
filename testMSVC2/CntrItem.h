
// CntrItem.h : interface of the CtestMSVC2CntrItem class
//

#pragma once

class CtestMSVC2Doc;
class CtestMSVC2View;

class CtestMSVC2CntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CtestMSVC2CntrItem)

// Constructors
public:
	CtestMSVC2CntrItem(REOBJECT* preo = NULL, CtestMSVC2Doc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CtestMSVC2Doc* GetDocument()
		{ return reinterpret_cast<CtestMSVC2Doc*>(CRichEditCntrItem::GetDocument()); }
	CtestMSVC2View* GetActiveView()
		{ return reinterpret_cast<CtestMSVC2View*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CtestMSVC2CntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

