
// SIEMENS_Bus.h : SIEMENS_Bus Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSIEMENS_BusApp:
// �йش����ʵ�֣������ SIEMENS_Bus.cpp
//

class CSIEMENS_BusApp : public CWinAppEx
{
public:
	CSIEMENS_BusApp();


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
};

extern CSIEMENS_BusApp theApp;
