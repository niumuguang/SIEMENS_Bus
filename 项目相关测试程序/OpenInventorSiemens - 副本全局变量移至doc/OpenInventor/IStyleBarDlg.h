#pragma once
#include "StraightBar.h"
#include <Inventor/nodes/SoSelection.h>
// CIStyleBarDlg �Ի���

class CIStyleBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIStyleBarDlg)

public:
	CIStyleBarDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIStyleBarDlg();

// �Ի�������
	enum { IDD = IDD_ISTYLE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIBarOk();


	//�Խ���
public:
	CStraightBar m_StrainghtBarFun;
};
