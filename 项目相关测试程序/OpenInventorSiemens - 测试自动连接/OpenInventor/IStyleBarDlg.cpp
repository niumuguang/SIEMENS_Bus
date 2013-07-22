// IStyleBarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "IStyleBarDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <Inventor\nodes\SoDepthBuffer.h>
#include <Inventor\nodes\SoTranslation.h>
#include "StraightBar.h"
#include "LStyleBar.h"

// CIStyleBarDlg 对话框

IMPLEMENT_DYNAMIC(CIStyleBarDlg, CDialogEx)

CIStyleBarDlg::CIStyleBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIStyleBarDlg::IDD, pParent)
{
	m_StrainghtBarFun.setHwnd(theApp.m_ViewHwnd);
}

CIStyleBarDlg::~CIStyleBarDlg()
{
}

void CIStyleBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IStyleBar_Name, IBar_Name);
}


BEGIN_MESSAGE_MAP(CIStyleBarDlg, CDialogEx)
	ON_BN_CLICKED(ID_I_BAR_OK, &CIStyleBarDlg::OnBnClickedIBarOk)
	ON_BN_CLICKED(IDC_BUTTON3, &CIStyleBarDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CIStyleBarDlg 消息处理程序

void CIStyleBarDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CStraightBar testIBar;
	theApp.m_MainRoot->addChild(testIBar.CreateBar());
}

void CIStyleBarDlg::OnBnClickedIBarOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SbVec3d temp1Vec; temp1Vec.setValue(0,0,0);
	CString testName; testName = "ssd";
	CLStyleBar test(testName, temp1Vec, 24, 24, 1, 4, 1.57,0,0);
	theApp.m_MainRoot->addChild(test.CreateBar());
	//if (theApp.testX == 1)
	//{
	//	// 建立I型连结排
	//	//theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateStraightBar());
	//	//SoGroup * ddd;
	//	SbVec3d temp1Vec; temp1Vec.setValue(0,0,0);
	//	CStraightBar temp1(_T("Co"), temp1Vec);
	//	theApp.m_MainRoot->addChild(temp1.CreateBar());
	//	theApp.testX = theApp.testX + 1;
	//	//theApp.m_pViewer->setSceneGraph(theApp.m_Selection);
	//}
	//else if (theApp.testX == 2)
	//{
	//	//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
	//	//blueDepth->range.setValue(SoDepthBuffer::ALWAYS);
	//	//theApp.m_MainRoot->addChild(blueDepth);
	//	SbVec3d temp2Vec; temp2Vec.setValue(10,0,0);
	//	CStraightBar temp2(_T("CON"),temp2Vec);
	//	theApp.m_MainRoot->addChild(temp2.CreateBar());
	//	//theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateStraightBar());
	//}
	theApp.m_pViewer->viewAll();
	//theApp.blueDepth->test.setValue(TRUE);
	CDialogEx::OnCancel();
	//theApp.I_BarCreateDlg.CloseWindow();
}

bool CIStyleBarDlg::CheckRepeat( CString BarName )
{
	BarName = BarName + _T("GROUP");
	SbName CheckName;
	CheckName = (char *)BarName.GetBuffer();
	SoNode * tempNode = theApp.m_MainRoot->getByName(CheckName);
	if (tempNode->isOfType(SoGroup::getClassTypeId()))
	{
		return true;
	}
	return false;
}

CString CIStyleBarDlg::getIBarName( void )
{
	IBar_Name.GetWindowTextW(m_Name);
	if (m_Name.GetLength() == 0)
	{
		theApp.myMessageBox(_T("输入空值"));
	}
}



