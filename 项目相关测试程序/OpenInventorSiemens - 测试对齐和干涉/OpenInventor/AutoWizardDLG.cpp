// AutoWizardDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "AutoWizardDLG.h"
#include "afxdialogex.h"
#include "AutoParameterDLG.h"


// CAutoWizardDLG 对话框

IMPLEMENT_DYNAMIC(CAutoWizardDLG, CDialogEx)

CAutoWizardDLG::CAutoWizardDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoWizardDLG::IDD, pParent)
{

}

CAutoWizardDLG::~CAutoWizardDLG()
{
}

void CAutoWizardDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON8, m_Connect1);
}


BEGIN_MESSAGE_MAP(CAutoWizardDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON8, &CAutoWizardDLG::OnBnClickedButton8)
	ON_BN_CLICKED(IDOK, &CAutoWizardDLG::OnBnClickedOk)
END_MESSAGE_MAP()


// CAutoWizardDLG 消息处理程序


BOOL CAutoWizardDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HBITMAP hbitmap;
	//hbitmap = LoadBitmap(AfxGetInstanceHandle(), 
	//	MAKEINTRESOURCE(IDB_BITMAP1));
	//m_Connect1.SetBitmap(hbitmap);
	hbitmap = (HBITMAP)::LoadImageA(
		NULL, "d://aaa.bmp", IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_Connect1.SetBitmap(hbitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAutoWizardDLG::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	CAutoParameterDLG autoPara;
	autoPara.DoModal();
}


void CAutoWizardDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
