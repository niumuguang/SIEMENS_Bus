/*==============================================================================
 *   File        : MagellanMfcDoc.h
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : Interface of the CMagellanMfcDoc class.
 *
 *============================================================================*/

#if !defined(AFX_MAGELLANMFCDOC_H__5EA85F1C_357C_4843_A919_9FBA14AE9AFD__INCLUDED_)
#define AFX_MAGELLANMFCDOC_H__5EA85F1C_357C_4843_A919_9FBA14AE9AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMagellanMfcDoc : public CDocument
{
protected: // create from serialization only
	CMagellanMfcDoc();
	DECLARE_DYNCREATE(CMagellanMfcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagellanMfcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMagellanMfcDoc();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMagellanMfcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGELLANMFCDOC_H__5EA85F1C_357C_4843_A919_9FBA14AE9AFD__INCLUDED_)
