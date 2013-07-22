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

#define PI 3.141592653;	// ԰����




// OpenInventor.h : OpenInventor Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "IStyleBarDlg.h"
#include <Inventor/nodes/SoShapeHints.h>


// COpenInventorApp:
// �йش����ʵ�֣������ OpenInventor.cpp
//

class COpenInventorApp : public CWinAppEx
{
public:
	COpenInventorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
//		���������ȫ�ֱ�����Ҫ��������
//////////////////////////////////////////////////////////////////////////
public:
	// viewing �ж�
	bool ViewingToF;
	// ģ����Ⱦ�ռ�
	SoWinExaminerViewer *m_pViewer;
	// ģ�Ϳռ������
	SoCamera * m_mainCam;
	// �����¼��ڵ�
	SoEventCallback * m_EventNode;
	// ���ڵ�
	SoSeparator * m_Root;
	// ���ڵ�
	SoSeparator *m_MainRoot;
	//SoSelection * m_MainRoot;
	// ���ƶ��ڵ�
	SoTranslation * m_translation;
	// ����ת�ڵ�
	SoRotation * m_rotation;
	// �����սڵ�
	SoDirectionalLight * m_DirectionalLight;
	// ��ѡ��ڵ�
	SoSelection * m_Selection;
	// ��ǰ���λ��
	CPoint m_MouseCurPos;
	// ��갴��λ��
	CPoint m_MousePressPos;
	// ����ͷ�λ��
	CPoint m_MouseReleasePos;
	// ��ͼ��ת�뾶
	double m_RadiusView;
	// ��ת����
	bool m_rotationSwitch;
	// ƽ�ƿ���
	bool m_translationSwitch;
	// �����ھ��;
	Hwnd  m_ViewHwnd;
	// ���ڶ���
	CWnd  * m_ViewCWnd;
	// ���ô��ڶ�����
	//void setCWnd(CWnd * cwnd);
	// ��ת�Ƕ�
	double theta;
	// ��ȿ���
	SoDepthBuffer *blueDepth;
	// ��Ҳ��֪��
	SoShapeHints *sh;
	// ����ҳģ��λ��
	CMFCPropertyGridProperty* pModelPos;
	CMFCPropertyGridProperty* pPropXcoord;
	
	//
	int testX;
	int testINT;
//////////////////////////////////////////////////////////////////////////
//	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern COpenInventorApp theApp;
