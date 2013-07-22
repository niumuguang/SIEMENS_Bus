
// LargeModelViewerView.h : CLargeModelViewerView ��Ľӿ�
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
protected: // �������л�����
	CLargeModelViewerView();
	DECLARE_DYNCREATE(CLargeModelViewerView)

// ����
public:
	CLargeModelViewerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CLargeModelViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	 //SoWinViewer * m_pViewer;
	// ���ڵ�
	SoSeparator * m_Root;
	 LmvEViewer		whichViewer;	// current viewer ID
	 SoWinFullViewer	*viewerList[5];	// list of viewers

// ���ɵ���Ϣӳ�亯��
protected:
	//afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LargeModelViewerView.cpp �еĵ��԰汾
inline CLargeModelViewerDoc* CLargeModelViewerView::GetDocument() const
   { return reinterpret_cast<CLargeModelViewerDoc*>(m_pDocument); }
#endif

