// TestDlg.h : TestDlg DLL ����ͷ�ļ�
//
#if !defined(AFX_DLLDLG_H__07BBEC7A_80B8_44E3_8AA0_DDBD9979DF3D__INCLUDED_)
#define AFX_DLLDLG_H__07BBEC7A_80B8_44E3_8AA0_DDBD9979DF3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Inventor\nodes\SoSeparator.h>
#include <Inventor\nodes\SoCube.h>
#include "..\..\OpenInventor\OpenInventor.h"
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestDlgApp
// �йش���ʵ�ֵ���Ϣ������� TestDlg.cpp
//

class CTestDlgApp : public CWinApp
{
public:
	CTestDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	SoSeparator * restest;
};

#endif