#pragma once


// JudgementDLG 对话框

class JudgementDLG : public CDialogEx
{
	DECLARE_DYNAMIC(JudgementDLG)

public:
	JudgementDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~JudgementDLG();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
