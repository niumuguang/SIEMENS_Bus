/*==============================================================================
 *   File        : MainFrm.h
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : Interface of the CMainFrame class.
 *
 *============================================================================*/

#if !defined(AFX_MAINFRM_H__29664BD6_3974_4624_A8D1_657CE0998160__INCLUDED_)
#define AFX_MAINFRM_H__29664BD6_3974_4624_A8D1_657CE0998160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__29664BD6_3974_4624_A8D1_657CE0998160__INCLUDED_)
