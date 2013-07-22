
// OpenInventorMFCView.cpp : COpenInventorMFCView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// COpenInventorMFCView ����/����

COpenInventorMFCView::COpenInventorMFCView()
{
	// TODO: �ڴ˴���ӹ������

}

COpenInventorMFCView::~COpenInventorMFCView()
{
}

BOOL COpenInventorMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COpenInventorMFCView ����

void COpenInventorMFCView::OnDraw(CDC* /*pDC*/)
{
	COpenInventorMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// COpenInventorMFCView ���

#ifdef _DEBUG
void COpenInventorMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COpenInventorMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenInventorMFCDoc* COpenInventorMFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenInventorMFCDoc)));
	return (COpenInventorMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenInventorMFCView ��Ϣ�������
