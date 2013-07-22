// AutoWizardDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "AutoWizardDLG.h"
#include "afxdialogex.h"
#include "AutoParameterDLG.h"


// CAutoWizardDLG �Ի���

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


// CAutoWizardDLG ��Ϣ�������


BOOL CAutoWizardDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HBITMAP hbitmap;
	//hbitmap = LoadBitmap(AfxGetInstanceHandle(), 
	//	MAKEINTRESOURCE(IDB_BITMAP1));
	//m_Connect1.SetBitmap(hbitmap);
	hbitmap = (HBITMAP)::LoadImageA(
		NULL, "d://aaa.bmp", IMAGE_BITMAP,
		0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_Connect1.SetBitmap(hbitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAutoWizardDLG::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	CAutoParameterDLG autoPara;
	autoPara.DoModal();
}


void CAutoWizardDLG::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
