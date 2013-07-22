
// OpenInventorMFCView.h : COpenInventorMFCView ��Ľӿ�
//

#pragma once


class COpenInventorMFCView : public CView
{
protected: // �������л�����
	COpenInventorMFCView();
	DECLARE_DYNCREATE(COpenInventorMFCView)

// ����
public:
	COpenInventorMFCDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COpenInventorMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // OpenInventorMFCView.cpp �еĵ��԰汾
inline COpenInventorMFCDoc* COpenInventorMFCView::GetDocument() const
   { return reinterpret_cast<COpenInventorMFCDoc*>(m_pDocument); }
#endif

