// LargeModelViewer.h : main header file for the LargeModelViewer application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerApp:
// See LargeModelViewer.cpp for the implementation of this class
//

class CLargeModelViewerApp : public CWinApp
{
public:
	CLargeModelViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLargeModelViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLargeModelViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpLargemodelviewer();
	afx_msg void OnHelpLmvOverview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
