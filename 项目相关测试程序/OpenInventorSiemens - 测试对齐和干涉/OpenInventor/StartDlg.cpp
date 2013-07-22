// StartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "StartDlg.h"
#include "afxdialogex.h"


// CStartDlg 对话框

IMPLEMENT_DYNAMIC(CStartDlg, CDialogEx)

CStartDlg::CStartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartDlg::IDD, pParent)
{

}

CStartDlg::~CStartDlg()
{
}

void CStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDlg, CDialogEx)
END_MESSAGE_MAP()


// CStartDlg 消息处理程序
