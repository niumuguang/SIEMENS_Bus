
// LargeModelViewerView.cpp : CLargeModelViewerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LargeModelViewer.h"
#endif

#include "LargeModelViewerDoc.h"
#include "LargeModelViewerView.h"
#include <Inventor\Win\viewers\SoWinExaminerViewer.h>
#include <Inventor\SbColor.h>
#include <Inventor\Win\viewers\SoWinViewer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLargeModelViewerView

IMPLEMENT_DYNCREATE(CLargeModelViewerView, CView)

BEGIN_MESSAGE_MAP(CLargeModelViewerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLargeModelViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLargeModelViewerView 构造/析构

CLargeModelViewerView::CLargeModelViewerView()
{
	// TODO: 在此处添加构造代码

}

CLargeModelViewerView::~CLargeModelViewerView()
{
}

// BOOL CLargeModelViewerView::PreCreateWindow(CREATESTRUCT& cs)
// {
// 	// TODO: 在此处通过修改
// 	//  CREATESTRUCT cs 来修改窗口类或样式
// 
// 	return CView::PreCreateWindow(cs);
// }

// CLargeModelViewerView 绘制

void CLargeModelViewerView::OnDraw(CDC* /*pDC*/)
{
	CLargeModelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CLargeModelViewerView 打印


// void CLargeModelViewerView::OnFilePrintPreview()
// {
// #ifndef SHARED_HANDLERS
// 	AFXPrintPreview(this);
// #endif
// }

// BOOL CLargeModelViewerView::OnPreparePrinting(CPrintInfo* pInfo)
// {
// 	// 默认准备
// 	return DoPreparePrinting(pInfo);
// }

// void CLargeModelViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: 添加额外的打印前进行的初始化过程
// }

// void CLargeModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: 添加打印后进行的清理过程
// }

// void CLargeModelViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
// {
// 	ClientToScreen(&point);
// 	OnContextMenu(this, point);
// }

//void CLargeModelViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// CLargeModelViewerView 诊断

#ifdef _DEBUG
void CLargeModelViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CLargeModelViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLargeModelViewerDoc* CLargeModelViewerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLargeModelViewerDoc)));
	return (CLargeModelViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLargeModelViewerView 消息处理程序


void CLargeModelViewerView::OnDestroy()
{
	//CView::OnDestroy();
	//delete (SoWinExaminerViewer *)viewerList[LMV_VWR_EXAMINER];
	//delete (SoWinFlyViewer *)viewerList[LMV_VWR_FLY];
	//delete (SoWinWalkViewer *)viewerList[LMV_VWR_WALK];
	//delete (SoWinPlaneViewer *)viewerList[LMV_VWR_PLANE];
	CView::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}


void CLargeModelViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	//theApp.m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
	theApp.m_pExViewer = new SoWinExaminerViewer(GetSafeHwnd());

	m_Root = new SoSeparator();

	SoInput Coordinate;
	if (Coordinate.openFile("test.iv") == NULL)
	{
		exit(-1);
	}
	SoSeparator *axis = SoDB::readAll(&Coordinate);

	//theApp.m_pViewer->setDecoration(FALSE);
	//theApp.m_pViewer->setDecimationStrategy(SoWinViewer::FRAMES_PER_SECOND);
	

	//theApp.m_pViewer->setSceneGraph(axis);
	//theApp.m_pViewer->viewAll();
	//theApp.m_pViewer->show();
	//////////////////////////////////////////////////////////////////////////
	theApp.m_pExViewer->setSceneGraph(axis);
	theApp.m_pExViewer->setDecoration(FALSE);
	theApp.m_pExViewer->show();

	//theApp.m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
	//m_pViewer = viewerList[whichViewer] = new SoWinExaminerViewer(GetSafeHwnd());
	//if (!m_pViewer) {
	//	whichViewer = LMV_VWR_EXAMINER;
	//	//m_pViewer = viewerList[whichViewer] = new SoWinExaminerViewer(GetSafeHwnd());
	//	m_pViewer->setBackgroundColor(SbColor(0.2f ,0.4f ,0.5f));

	//	// Set initial settings for Adaptive Viewing
	//	m_pViewer->setGoalFramesPerSecond(5.0f);
	//	m_pViewer->setDecimationStrategy(SoWinViewer::FRAMES_PER_SECOND);
	//	m_pViewer->enableFullRenderingWhenStill(TRUE);

	//	//SoSeparator *pSceneRoot = GetDocument()->getSceneRoot();
	//	//pSceneRoot->addChild(new SoCube);
	//	//m_pViewer->setSceneGraph(pSceneRoot);	        
	//	m_pViewer->show();

	//	// Set up frames per second, etc. callback
	//	//setRenderStats(renderStats);
	//	// setPrimitiveStats(primStats);
	//}
	//m_pViewer->viewAll();
}
