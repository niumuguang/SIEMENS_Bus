#pragma once
#include "afxwin.h"
#include <Inventor\nodes\SoFaceSet.h>

// CMoveDlg �Ի���

class CMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveDlg)

public:
	CMoveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMoveDlg();

// �Ի�������
	enum { IDD = IDD_MOVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	SoPath * m_path;

public:
	afx_msg void OnBnClickedOk();
	// �ƶ�X����༭��
	CEdit m_Model_Move_X;
	// �ƶ�X����༭��
	CEdit m_Model_Move_Y;
	// �ƶ�X����༭��
	CEdit m_Model_Move_Z;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
