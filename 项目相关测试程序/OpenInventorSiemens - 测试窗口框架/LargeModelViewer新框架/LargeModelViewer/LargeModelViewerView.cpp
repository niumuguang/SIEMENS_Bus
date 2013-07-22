
// LargeModelViewerView.cpp : CLargeModelViewerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLargeModelViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLargeModelViewerView ����/����

CLargeModelViewerView::CLargeModelViewerView()
{
	// TODO: �ڴ˴���ӹ������

}

CLargeModelViewerView::~CLargeModelViewerView()
{
}

// BOOL CLargeModelViewerView::PreCreateWindow(CREATESTRUCT& cs)
// {
// 	// TODO: �ڴ˴�ͨ���޸�
// 	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
// 
// 	return CView::PreCreateWindow(cs);
// }

// CLargeModelViewerView ����

void CLargeModelViewerView::OnDraw(CDC* /*pDC*/)
{
	CLargeModelViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CLargeModelViewerView ��ӡ


// void CLargeModelViewerView::OnFilePrintPreview()
// {
// #ifndef SHARED_HANDLERS
// 	AFXPrintPreview(this);
// #endif
// }

// BOOL CLargeModelViewerView::OnPreparePrinting(CPrintInfo* pInfo)
// {
// 	// Ĭ��׼��
// 	return DoPreparePrinting(pInfo);
// }

// void CLargeModelViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
// }

// void CLargeModelViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: ��Ӵ�ӡ����е��������
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


// CLargeModelViewerView ���

#ifdef _DEBUG
void CLargeModelViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CLargeModelViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLargeModelViewerDoc* CLargeModelViewerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLargeModelViewerDoc)));
	return (CLargeModelViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLargeModelViewerView ��Ϣ�������


void CLargeModelViewerView::OnDestroy()
{
	//CView::OnDestroy();
	//delete (SoWinExaminerViewer *)viewerList[LMV_VWR_EXAMINER];
	//delete (SoWinFlyViewer *)viewerList[LMV_VWR_FLY];
	//delete (SoWinWalkViewer *)viewerList[LMV_VWR_WALK];
	//delete (SoWinPlaneViewer *)viewerList[LMV_VWR_PLANE];
	CView::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
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
