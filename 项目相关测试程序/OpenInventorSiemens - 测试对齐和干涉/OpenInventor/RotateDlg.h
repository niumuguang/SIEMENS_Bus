#pragma once
#include "afxwin.h"


// CRotateDlg 对话框

class CRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRotateDlg();

// 对话框数据
	enum { IDD = IDD_ROTATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	SoPath * m_path;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_RotateX;
	CComboBox m_RotateY;
	CComboBox m_RotateZ;
};
