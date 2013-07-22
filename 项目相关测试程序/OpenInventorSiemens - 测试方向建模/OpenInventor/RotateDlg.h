#pragma once
#include "afxwin.h"


// CRotateDlg �Ի���

class CRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRotateDlg();

// �Ի�������
	enum { IDD = IDD_ROTATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	SoPath * m_path;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_RotateX;
	CComboBox m_RotateY;
	CComboBox m_RotateZ;
};
