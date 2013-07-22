
// LargeModelViewerView.h : CLargeModelViewerView 类的接口
//

class SoWinViewer;
class SoWinFullViewer;
class SoSeparator;
class SoShape;
class SoGetPrimitiveCountAction;
class SoSensor;
class SoOneShotSensor;
class CDecimationStrategyDialog;
class SbColor;

#include <Inventor/SbBasic.h>

enum LmvEViewer {
	LMV_VWR_EXAMINER = 0, 
	LMV_VWR_FLY, 
	LMV_VWR_WALK, 
	LMV_VWR_PLANE
};

#pragma once
#include <Inventor/nodes/SoSeparator.h>


class CLargeModelViewerView : public CView
{
protected: // 仅从序列化创建
	CLargeModelViewerView();
	DECLARE_DYNCREATE(CLargeModelViewerView)

// 特性
public:
	CLargeModelViewerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CLargeModelViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	 //SoWinViewer * m_pViewer;
	// 根节点
	SoSeparator * m_Root;
	 LmvEViewer		whichViewer;	// current viewer ID
	 SoWinFullViewer	*viewerList[5];	// list of viewers

// 生成的消息映射函数
protected:
	//afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LargeModelViewerView.cpp 中的调试版本
inline CLargeModelViewerDoc* CLargeModelViewerView::GetDocument() const
   { return reinterpret_cast<CLargeModelViewerDoc*>(m_pDocument); }
#endif

