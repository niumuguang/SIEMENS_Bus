// TestDlg.h : TestDlg DLL 的主头文件
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
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTestDlgApp
// 有关此类实现的信息，请参阅 TestDlg.cpp
//

class CTestDlgApp : public CWinApp
{
public:
	CTestDlgApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	SoSeparator * restest;
};

#endif