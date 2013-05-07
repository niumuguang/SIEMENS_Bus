// MFCSDIDoc.cpp : implementation of the CMFCSDIDoc class
//

#include "stdafx.h"
#include "MFCSDI.h"

#include "MFCSDIDoc.h"

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIDoc

IMPLEMENT_DYNCREATE(CMFCSDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCSDIDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCSDIDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIDoc construction/destruction

CMFCSDIDoc::CMFCSDIDoc()
{
	// TODO: add one-time construction code here

}

CMFCSDIDoc::~CMFCSDIDoc()
{
}

BOOL CMFCSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIDoc serialization

void CMFCSDIDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIDoc diagnostics

#if defined(_DEBUG)
void CMFCSDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCSDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIDoc commands
