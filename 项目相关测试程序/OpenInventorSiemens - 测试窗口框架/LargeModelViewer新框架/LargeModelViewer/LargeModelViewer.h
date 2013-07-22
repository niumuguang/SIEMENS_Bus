
// LargeModelViewer.h : LargeModelViewer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include <Inventor\Win\viewers\SoWinViewer.h>
#include <Inventor\Win\viewers\SoWinExaminerViewer.h>


// CLargeModelViewer:
// �йش����ʵ�֣������ LargeModelViewer.cpp
//

class CLargeModelViewerApp : public CWinAppEx // diff
{
public:
	CLargeModelViewerApp();


// ��д
public:
	virtual BOOL InitInstance();
	//virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// ʵ��
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
