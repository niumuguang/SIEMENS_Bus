#if !defined(AFX_EXPORTDLL_H__3BFAB92C_FDBB_419D_AFAF_E5099EDE27E0__INCLUDED_)
#define AFX_EXPORTDLL_H__3BFAB92C_FDBB_419D_AFAF_E5099EDE27E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDll.h : header file

#pragma once

// CExportDLL �Ի���

class CExportDLL : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDLL)

public:
	CExportDLL(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportDLL();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif