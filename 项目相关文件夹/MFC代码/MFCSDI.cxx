// MFCSDI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCSDI.h"

#include "MainFrm.h"
#include "MFCSDIDoc.h"
#include "MFCSDIView.h"


//
// BEGIN OIV
//
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodekit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/Win/SoWin.h>
#include <ScaleViz/SoScaleViz.h>

//
// END OIV
//


#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIApp

BEGIN_MESSAGE_MAP(CMFCSDIApp, CWinApp)
	//{{AFX_MSG_MAP(CMFCSDIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIApp construction

CMFCSDIApp::CMFCSDIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMFCSDIApp object

CMFCSDIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIApp initialization

BOOL CMFCSDIApp::InitInstance()
{

//
// BEGIN OIV
//
SoDB::init();
SoScaleViz::init();
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
    //
    // NOTE: These methods are deprecated in MFC 5.0 and later
#if _MFC_VER < 0x0500
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

    unsigned int foo = _MFC_VER;

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMFCSDIDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMFCSDIView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

int
CMFCSDIApp::ExitInstance()
{
  SoWin::finish();

  return CWinApp::ExitInstance();
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
void CMFCSDIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIApp commands


BOOL CMFCSDIApp::OnIdle(LONG lCount) 
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
