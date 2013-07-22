
// OpenInventorMFCView.cpp : COpenInventorMFCView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenInventorMFC.h"
#endif

#include "OpenInventorMFCDoc.h"
#include "OpenInventorMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenInventorMFCView

IMPLEMENT_DYNCREATE(COpenInventorMFCView, CView)

BEGIN_MESSAGE_MAP(COpenInventorMFCView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// COpenInventorMFCView 构造/析构

COpenInventorMFCView::COpenInventorMFCView()
{
	// TODO: 在此处添加构造代码

}

COpenInventorMFCView::~COpenInventorMFCView()
{
}

BOOL COpenInventorMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COpenInventorMFCView 绘制

void COpenInventorMFCView::OnDraw(CDC* /*pDC*/)
{
	COpenInventorMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void COpenInventorMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenInventorMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenInventorMFCView 诊断

#ifdef _DEBUG
void COpenInventorMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COpenInventorMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenInventorMFCDoc* COpenInventorMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenInventorMFCDoc)));
	return (COpenInventorMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenInventorMFCView 消息处理程序
