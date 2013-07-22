// LargeModelViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LargeModelViewer.h"

#include "MainFrm.h"
#include "LargeModelViewerDoc.h"
#include "LargeModelViewerView.h"

//
// BEGIN OIV
//
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodekit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/errors/SoDebugError.h>

// Licensing:
//#include <Inventor/lock/demoUnlock.h>
#include <Inventor/lock/SoLockMgr.h>


//
// END OIV
//


#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerApp

BEGIN_MESSAGE_MAP(CLargeModelViewerApp, CWinApp)
	//{{AFX_MSG_MAP(CLargeModelViewerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDM_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(IDM_HELP_LARGEMODELVIEWER, OnHelpLargemodelviewer)
	ON_COMMAND(IDM_HELP_LMV_OVERVIEW, OnHelpLmvOverview)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerApp construction

CLargeModelViewerApp::CLargeModelViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLargeModelViewerApp object

CLargeModelViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerApp initialization


void	
myDebugErrorCB(const SoError *error, void *)//data)
{
  SoDebugError *debugError = (SoDebugError*) error;

  if (debugError->getSeverity() != SoDebugError::WARNING) {
    AfxMessageBox(error->getDebugString().getString());
  }
}


BOOL CLargeModelViewerApp::InitInstance()
{

//
// BEGIN OIV
//

#ifdef DEMO_UNLOCK_STRING
	SoLockManager::SetUnlockString(DEMO_UNLOCK_STRING);
#endif

SoDB::init();
SoNodeKit::init();
SoInteraction::init();
			//  initialize inventor systems.
   
//
// END OIV
//

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLargeModelViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLargeModelViewerView));
	AddDocTemplate(pDocTemplate);


    // Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

       SoDebugError::setHandlerCallback(myDebugErrorCB, NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CLargeModelViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerApp commands


BOOL CLargeModelViewerApp::OnIdle(LONG lCount) 
{
//
// BEGIN OIV
//
        SoWin::doIdleTasks();
//
// END OIV
//

	return CWinApp::OnIdle(lCount);
}

// Help Content Ids from Inventor.hlp file:
#define OpenInventor_Contents	131073
#define LMV_Overview	        131080
#define LargeModelViewer	131081


void CLargeModelViewerApp::OnHelpContents() 
{
    ::WinHelp(
      AfxGetMainWnd()->GetSafeHwnd(), // handle of window requesting Help 
      "Inventor.hlp",	// address of directory-path string 
      HELP_CONTEXT,	// type of Help 
      OpenInventor_Contents 	// help context id
   );
}

void CLargeModelViewerApp::OnHelpLargemodelviewer() 
{
   ::WinHelp(
      AfxGetMainWnd()->GetSafeHwnd(), // handle of window requesting Help 
      "Inventor.hlp",	// address of directory-path string 
      HELP_CONTEXT,	// type of Help 
      LargeModelViewer 	// help context id
   );
	
}

void CLargeModelViewerApp::OnHelpLmvOverview() 
{
   ::WinHelp(
      AfxGetMainWnd()->GetSafeHwnd(), // handle of window requesting Help 
      "Inventor.hlp",	// address of directory-path string 
      HELP_CONTEXT,	// type of Help 
      LMV_Overview 	// help context id
   );
	
}

