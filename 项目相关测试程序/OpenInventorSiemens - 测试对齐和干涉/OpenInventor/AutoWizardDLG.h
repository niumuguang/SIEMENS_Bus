#pragma once
#include "afxwin.h"


// CAutoWizardDLG �Ի���

class CAutoWizardDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoWizardDLG)

public:
	CAutoWizardDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAutoWizardDLG();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_Connect1;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedOk();
};
