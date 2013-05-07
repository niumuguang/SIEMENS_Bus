// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFCSDI.h"

#include "MainFrm.h"

//
// BEGIN OIV
//
#include <Inventor/Win/SoWin.h>
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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

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
	
	// 工具条
 	if (!m_wndToolBar.Create(this) ||
 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
 	{
 		TRACE0("Failed to create toolbar\n");
 		return -1;      // fail to create
 	}

	// 状态条
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

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
		#if _MFC_VER < 1300
        SoWin::setPrevInstance( pApp->m_hPrevInstance );
		#endif
//
// END OIV
//

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
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
	//CFrameWnd::Dump(dc);
	CFrameWnd::Dump(dc);
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

//LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp, LPARAM lp )
//{
// 	LRESULT lres = CFrameWnd::OnToolbarCreateNew(wp,lp);
// 
// 	if (lres == 0)
// 	{
// 		return 0;
// 	}
// 
// 	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
// 	ASSERT_VALID(pUserToolbar);
// 
// 	BOOL bNameValid;
// 	CString strCustomize = "错误";
// 	//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
// 	ASSERT(bNameValid);
// 
// 	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
// 	return lres;
//}

// CMainFrame::COutputWnd()
// {
// 
// }
// 
// CMainFrame::~COutputWnd()
// {
// 
// }

#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
