/*==============================================================================
 *   File        : MagellanMfc.h
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : Main header file for the MAGELLANMFC application.
 *
 *============================================================================*/
#if !defined(AFX_MAGELLANMFC_H__E7A8430D_ECAD_48AB_9444_AEB516D5A926__INCLUDED_)
#define AFX_MAGELLANMFC_H__E7A8430D_ECAD_48AB_9444_AEB516D5A926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcApp:
// See MagellanMfc.cpp for the implementation of this class
//

class CMagellanMfcApp : public CWinApp
{
public:
	CMagellanMfcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagellanMfcApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMagellanMfcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGELLANMFC_H__E7A8430D_ECAD_48AB_9444_AEB516D5A926__INCLUDED_)
