#pragma once


// JudgementDLG �Ի���

class JudgementDLG : public CDialogEx
{
	DECLARE_DYNAMIC(JudgementDLG)

public:
	JudgementDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~JudgementDLG();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
