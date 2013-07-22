
// OpenInventorView.h : COpenInventorView 类的接口
//
#ifndef PI
#define  PI 3.1415926535897932384626433832795
#endif

#pragma once
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoColorIndex.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <SolidViz/SoCSGOperation.h>
#include <Inventor/engines/SoBoolOperation.h>
#include <SolidViz/nodes/SoCSGGroup.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/events/SoMouseWheelEvent.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <cmath>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/fields/SoField.h>

#include <tuple>
#include <vector>
//typedef tuple<float, float, float> AXIS;
using namespace std;


class COpenInventorView : public CView
{
public: // 仅从序列化创建
	COpenInventorView();
	DECLARE_DYNCREATE(COpenInventorView);

	// 新增公共数据成员变量 用户自定义
	// 新增函数 用户自定义
public:
	// 测试 SoFaceSet 的代码
	void TestSoFaceSet();
	double get2PointDis(CPoint pt1, CPoint pt2);
	double getAngle(double distance2Pt);
	double getSlope(CPoint pt1, CPoint pt2);
	double getArcSlope(double k);
	SbVec2f getPt4(double dis, double arcK);
	SbVec2f getMidPt(CPoint pt1, CPoint pt2);
	SbVec3f getNormalVec(SbVec2f pt4, SbVec2f midPt);
	//////////////////////////////////////////////////////////////////////////
	//高仿
	COORDINATE3 curPos, lastPos;
	//float curPos[3], lastPos[3];// 鼠标上次和当前坐标（映射到单位半球面）
	COORDINATE3 m_axis;
	SbRotation totalRotation;
	//float axis[3];						// 旋转轴
	int hemishere(int x, int y, int d,COORDINATE3 &v);
	void mouseWheelCB(void *, SoEventCallback *eventCB);
	//////////////////////////////////////////////////////////////////////////
	float ss;
	SoSeparator * addNodeFun(void);
	SoWinExaminerViewer * getViewer(void);
	void testFun(void *userdata, SoEventCallback *node);
	int ViewType;

// 特性
public:
	COpenInventorDoc* GetDocument() const;

// 操作
public:
	//SoCSGGroup *m_csgGroup;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COpenInventorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnCreatmodel();
	afx_msg void OnFile1();
	afx_msg void OnDestroy();
	afx_msg void OnViewStatusBar();
	afx_msg void rotView();
};

extern COpenInventorView theView;

#ifndef _DEBUG  // OpenInventorView.cpp 中的调试版本
inline COpenInventorDoc* COpenInventorView::GetDocument() const
   { return reinterpret_cast<COpenInventorDoc*>(m_pDocument); }
#endif

