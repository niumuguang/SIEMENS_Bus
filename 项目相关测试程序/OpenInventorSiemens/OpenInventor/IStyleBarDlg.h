#pragma once
#include "StraightBar.h"
#include <Inventor/nodes/SoSelection.h>
// CIStyleBarDlg 对话框

class CIStyleBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIStyleBarDlg)

public:
	CIStyleBarDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIStyleBarDlg();

// 对话框数据
	enum { IDD = IDD_ISTYLE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIBarOk();


	//自建类
public:
	CStraightBar m_StrainghtBarFun;
};
