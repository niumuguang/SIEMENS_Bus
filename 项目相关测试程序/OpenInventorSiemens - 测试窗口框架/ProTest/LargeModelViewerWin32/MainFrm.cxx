// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "LargeModelViewerDoc.h"

#include "MainFrm.h"

//
// BEGIN OIV
//
#include <Inventor/Win/SoWin.h>
#include <Inventor/SoDB.h>

//
// END OIV
//

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDM_TRAV_OCTREE_ORD, OnTravOctreeOrd)
	ON_COMMAND(IDM_TRAV_SEPARATOR, OnTravSeparator)
	ON_COMMAND(IDM_TRAV_VALUE_ORD, OnTravValueOrd)
	ON_WM_INITMENU()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()


/*
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
 */ 


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,			// Frames Per Second pane
	ID_SEPARATOR,			// Decimation pane
	ID_SEPARATOR,			// Traversal pane

};

#define PANE_FPS 1
#define PANE_DEC 2
#define PANE_TRAV 3


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Pane for frames per second
	m_wndStatusBar.SetPaneInfo(PANE_FPS, ID_SEPARATOR, SBPS_NORMAL, 64);
	m_wndStatusBar.SetPaneText(PANE_FPS, "fps");   

	// Pane for decimation percentage
	m_wndStatusBar.SetPaneInfo(PANE_DEC, ID_SEPARATOR, SBPS_NORMAL, 48);
	m_wndStatusBar.SetPaneText(PANE_DEC, "100%");

	// Pane for traversal ordering
	m_wndStatusBar.SetPaneInfo(PANE_TRAV, ID_SEPARATOR, SBPS_NORMAL, 64);
	m_wndStatusBar.SetPaneText(PANE_TRAV, "Separator");
	

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

//
// BEGIN OIV
//
	SoWin::init(GetSafeHwnd()) ;
        CWinApp *pApp = AfxGetApp();

        SoWin::setInstance(pApp->m_hInstance );
#if _MSC_VER < 1300
        SoWin::setPrevInstance( pApp->m_hPrevInstance );
#endif

	// Allow dropping of Inventor files on our window: 
	if (GetSafeHwnd() != 0)
		DragAcceptFiles();


	


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
        cs.cx = 640;
        cs.cy = 480;

	return CFrameWnd::PreCreateWindow(cs);
}


void CMainFrame::SetFpsPane(CString str)
{
	m_wndStatusBar.SetPaneText(PANE_FPS, str);   

}
  
void CMainFrame::SetDecimationPane(CString str)
{
	m_wndStatusBar.SetPaneText(PANE_DEC, str);   

}


void CMainFrame::SetPrimitivePane(CString str)
{
	m_wndStatusBar.SetPaneText(0, str);   

}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#if defined(_DEBUG)
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnTravSeparator() 
{
	((CLargeModelViewerDoc*)GetActiveDocument())->switchOrderingNode(
		CLargeModelViewerDoc::LMV_ORD_SEPARATOR);

	CMenu* pMenu = GetMenu();
	pMenu->CheckMenuItem( IDM_TRAV_VALUE_ORD, MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_TRAV_SEPARATOR, MF_CHECKED);	
	pMenu->CheckMenuItem( IDM_TRAV_OCTREE_ORD, MF_UNCHECKED);	

	// Set traversal type in Status Bar
	m_wndStatusBar.SetPaneText(PANE_TRAV, "Separator");


}

void CMainFrame::OnTravValueOrd() 
{
	((CLargeModelViewerDoc*)GetActiveDocument())->switchOrderingNode(
		CLargeModelViewerDoc::LMV_ORD_VALUE_ORDERING);

	CMenu* pMenu = GetMenu();
	pMenu->CheckMenuItem( IDM_TRAV_VALUE_ORD, MF_CHECKED);
	pMenu->CheckMenuItem( IDM_TRAV_SEPARATOR, MF_UNCHECKED);	
	pMenu->CheckMenuItem( IDM_TRAV_OCTREE_ORD, MF_UNCHECKED);	

	// Set traversal type in Status Bar:
	m_wndStatusBar.SetPaneText(PANE_TRAV, "Value");

}

void CMainFrame::OnTravOctreeOrd() 
{
	((CLargeModelViewerDoc*)GetActiveDocument())->switchOrderingNode(
		CLargeModelViewerDoc::LMV_ORD_OCTREE_ORDERING);

	 CMenu* pMenu = GetMenu();
	pMenu->CheckMenuItem( IDM_TRAV_VALUE_ORD, MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_TRAV_SEPARATOR, MF_UNCHECKED);	
	pMenu->CheckMenuItem( IDM_TRAV_OCTREE_ORD, MF_CHECKED);
	
	// Set traversal type in Status Bar:
	m_wndStatusBar.SetPaneText(PANE_TRAV, "Octree");

}



void CMainFrame::OnInitMenu(CMenu* pMenu) 
{

     CFrameWnd::OnInitMenu(pMenu);

	
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	
	
}
