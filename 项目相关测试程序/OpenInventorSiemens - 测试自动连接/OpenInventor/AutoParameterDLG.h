#pragma once


// CAutoParameterDLG 对话框

class CAutoParameterDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoParameterDLG)

public:
	CAutoParameterDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoParameterDLG();

// 对话框数据
	enum { IDD = IDD_AUTO_PARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
