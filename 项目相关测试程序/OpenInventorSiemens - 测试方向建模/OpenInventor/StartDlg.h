#include "stdafx.h"
#include "resource.h"
#pragma once


// CStartDlg �Ի���

class CStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartDlg)

public:
	CStartDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartDlg();

// �Ի�������
	enum { IDD = IDD_START_FACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
