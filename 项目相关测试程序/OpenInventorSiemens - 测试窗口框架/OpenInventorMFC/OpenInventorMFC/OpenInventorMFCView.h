
// OpenInventorMFCView.h : COpenInventorMFCView 类的接口
//

#pragma once


class COpenInventorMFCView : public CView
{
protected: // 仅从序列化创建
	COpenInventorMFCView();
	DECLARE_DYNCREATE(COpenInventorMFCView)

// 特性
public:
	COpenInventorMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~COpenInventorMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // OpenInventorMFCView.cpp 中的调试版本
inline COpenInventorMFCDoc* COpenInventorMFCView::GetDocument() const
   { return reinterpret_cast<COpenInventorMFCDoc*>(m_pDocument); }
#endif

