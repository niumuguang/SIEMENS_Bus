// AutoParameterDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "AutoParameterDLG.h"
#include "afxdialogex.h"
#include "StraightBar.h"


// CAutoParameterDLG 对话框

IMPLEMENT_DYNAMIC(CAutoParameterDLG, CDialogEx)

CAutoParameterDLG::CAutoParameterDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoParameterDLG::IDD, pParent)
{

}

CAutoParameterDLG::~CAutoParameterDLG()
{
}

void CAutoParameterDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoParameterDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAutoParameterDLG::OnBnClickedOk)
END_MESSAGE_MAP()


// CAutoParameterDLG 消息处理程序


void CAutoParameterDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	SbVec3d temp1Vec; temp1Vec.setValue(5,20,-1);
	CString testName; testName = "ssd";
	CLStyleBar test(testName, temp1Vec, 24, 24, 1, 4, 0, 0, 1.57);
	theApp.m_MainRoot->addChild(test.CreateBar());

	testName = "sbb";
	temp1Vec.setValue(0,0,-1);
	CStraightBar testIBar(testName, temp1Vec, 0, 0, 0);
	
	theApp.m_MainRoot->addChild(testIBar.CreateBar());
}
