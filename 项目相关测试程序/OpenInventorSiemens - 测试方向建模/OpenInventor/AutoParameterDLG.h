#pragma once


// CAutoParameterDLG �Ի���

class CAutoParameterDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoParameterDLG)

public:
	CAutoParameterDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAutoParameterDLG();

// �Ի�������
	enum { IDD = IDD_AUTO_PARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
