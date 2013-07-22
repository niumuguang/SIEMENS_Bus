#include "stdafx.h"
#include "resource.h"
#pragma once


// CStartDlg 对话框

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartDlg();

// 对话框数据
	enum { IDD = IDD_START_FACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
