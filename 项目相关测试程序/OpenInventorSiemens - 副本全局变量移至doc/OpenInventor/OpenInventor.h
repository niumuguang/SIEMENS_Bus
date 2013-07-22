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
#include <Inventor/nodes/SoShapeHints.h>
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<float, float, float> AXIS;

#define PI 3.141592653;	// 园周率




// OpenInventor.h : OpenInventor 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "IStyleBarDlg.h"
#include <Inventor/nodes/SoShapeHints.h>


// COpenInventorApp:
// 有关此类的实现，请参阅 OpenInventor.cpp
//

class COpenInventorApp : public CWinAppEx
{
public:
	COpenInventorApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
//		程序的所有全局变量都要放在这里
//////////////////////////////////////////////////////////////////////////
public:
	// viewing 判断
	bool ViewingToF;
	// 模型渲染空间
	SoWinExaminerViewer *m_pViewer;
	// 模型空间主相机
	SoCamera * m_mainCam;
	// 所有事件节点
	SoEventCallback * m_EventNode;
	// 根节点
	SoSeparator * m_Root;
	// 主节点
	SoSeparator *m_MainRoot;
	//SoSelection * m_MainRoot;
	// 主移动节点
	SoTranslation * m_translation;
	// 主旋转节点
	SoRotation * m_rotation;
	// 主光照节点
	SoDirectionalLight * m_DirectionalLight;
	// 主选择节点
	SoSelection * m_Selection;
	// 当前鼠标位置
	CPoint m_MouseCurPos;
	// 鼠标按下位置
	CPoint m_MousePressPos;
	// 鼠标释放位置
	CPoint m_MouseReleasePos;
	// 视图旋转半径
	double m_RadiusView;
	// 旋转开关
	bool m_rotationSwitch;
	// 平移开关
	bool m_translationSwitch;
	// 主窗口句柄;
	Hwnd  m_ViewHwnd;
	// 窗口对象
	CWnd  * m_ViewCWnd;
	// 设置窗口对象函数
	//void setCWnd(CWnd * cwnd);
	// 旋转角度
	double theta;
	// 深度开关
	SoDepthBuffer *blueDepth;
	// 我也不知道
	SoShapeHints *sh;
	// 属性页模型位置
	CMFCPropertyGridProperty* pModelPos;
	CMFCPropertyGridProperty* pPropXcoord;
	
	//
	int testX;
	int testINT;
//////////////////////////////////////////////////////////////////////////
//	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern COpenInventorApp theApp;
