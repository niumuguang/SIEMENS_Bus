#pragma once
#include "afxwin.h"
#include <Inventor\nodes\SoFaceSet.h>

// CMoveDlg 对话框

class CMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveDlg)

public:
	CMoveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMoveDlg();

// 对话框数据
	enum { IDD = IDD_MOVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	SoPath * m_path;

public:
	afx_msg void OnBnClickedOk();
	// 移动X距离编辑框
	CEdit m_Model_Move_X;
	// 移动X距离编辑框
	CEdit m_Model_Move_Y;
	// 移动X距离编辑框
	CEdit m_Model_Move_Z;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
