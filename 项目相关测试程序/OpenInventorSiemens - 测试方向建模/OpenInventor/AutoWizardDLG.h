#pragma once
#include "afxwin.h"


// CAutoWizardDLG 对话框

class CAutoWizardDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoWizardDLG)

public:
	CAutoWizardDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoWizardDLG();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_Connect1;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedOk();
};
