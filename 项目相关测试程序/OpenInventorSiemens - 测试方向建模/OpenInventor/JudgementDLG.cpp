// JudgementDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "JudgementDLG.h"
#include "afxdialogex.h"
#include "AutoWizardDLG.h"


// JudgementDLG �Ի���

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


// JudgementDLG ��Ϣ�������


void JudgementDLG::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	SbString temp = "1 0 0";
	theApp.xx->set(temp);
	theApp.yy->set(temp);
	theApp.m_Selection->deselectAll();
}
