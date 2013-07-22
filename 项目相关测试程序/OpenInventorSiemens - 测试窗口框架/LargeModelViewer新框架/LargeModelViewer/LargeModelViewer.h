
// LargeModelViewer.h : LargeModelViewer 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include <Inventor\Win\viewers\SoWinViewer.h>
#include <Inventor\Win\viewers\SoWinExaminerViewer.h>


// CLargeModelViewer:
// 有关此类的实现，请参阅 LargeModelViewer.cpp
//

class CLargeModelViewerApp : public CWinAppEx // diff
{
public:
	CLargeModelViewerApp();


// 重写
public:
	virtual BOOL InitInstance();
	//virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	SoWinViewer * m_pViewer;
	SoWinExaminerViewer * m_pExViewer;

	afx_msg void OnAppAbout();
	//virtual void PreLoadState();
	//virtual void LoadCustomState();
	//virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	
};

extern CLargeModelViewerApp theApp;
