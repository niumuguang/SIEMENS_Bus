// JudgementDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "JudgementDLG.h"
#include "afxdialogex.h"
#include "AutoWizardDLG.h"


// JudgementDLG 对话框

IMPLEMENT_DYNAMIC(JudgementDLG, CDialogEx)

JudgementDLG::JudgementDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(JudgementDLG::IDD, pParent)
{

}

JudgementDLG::~JudgementDLG()
{
}

void JudgementDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(JudgementDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &JudgementDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &JudgementDLG::OnBnClickedCancel)
END_MESSAGE_MAP()


// JudgementDLG 消息处理程序


void JudgementDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	SbString temp = "1 0 0";
	//SoSFEnum dd = theApp.m_Selection->policy;
	//if (dd.getValue() == 1)
	theApp.xx->set(temp);
	theApp.yy->set(temp);
	theApp.m_Selection->deselectAll();
	theApp.m_Selection->policy.setValue(SoSelection::SINGLE);

	CAutoWizardDLG wizard;
	wizard.DoModal();
}


void JudgementDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	SbString temp = "1 0 0";
	theApp.xx->set(temp);
	theApp.yy->set(temp);
	theApp.m_Selection->deselectAll();
}
