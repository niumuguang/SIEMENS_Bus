/*==============================================================================
 *   File        : MagellanMfcDoc.cxx
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : Implementation of the CMagellanMfcDoc class.
 *
 *============================================================================*/

#include "stdafx.h"
#include "MagellanMfc.h"

#include "MagellanMfcDoc.h"

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcDoc

IMPLEMENT_DYNCREATE(CMagellanMfcDoc, CDocument)

BEGIN_MESSAGE_MAP(CMagellanMfcDoc, CDocument)
	//{{AFX_MSG_MAP(CMagellanMfcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcDoc construction/destruction

CMagellanMfcDoc::CMagellanMfcDoc()
{
	// TODO: add one-time construction code here

}

CMagellanMfcDoc::~CMagellanMfcDoc()
{
}

BOOL CMagellanMfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcDoc serialization

void CMagellanMfcDoc::Serialize(CArchive& ar)
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
// CMagellanMfcDoc diagnostics

#if defined(_DEBUG)
void CMagellanMfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMagellanMfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcDoc commands
