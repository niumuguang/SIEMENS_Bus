// ExportDLL.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDlg.h"
#include "ExportDLL.h"
#include "afxdialogex.h"


// CExportDLL 对话框

IMPLEMENT_DYNAMIC(CExportDLL, CDialogEx)

CExportDLL::CExportDLL(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExportDLL::IDD, pParent)
{

}

CExportDLL::~CExportDLL()
{
}

void CExportDLL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExportDLL, CDialogEx)
END_MESSAGE_MAP()


// CExportDLL 消息处理程序
