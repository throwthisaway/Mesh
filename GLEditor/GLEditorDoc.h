// GLEditorDoc.h : interface of the CGLEditorDoc class
//


#pragma once


class CGLEditorDoc : public CDocument
{
protected: // create from serialization only
	CGLEditorDoc();
	DECLARE_DYNCREATE(CGLEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGLEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


