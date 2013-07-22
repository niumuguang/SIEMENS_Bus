// IStyleBarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "IStyleBarDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <Inventor\nodes\SoDepthBuffer.h>

// CIStyleBarDlg 对话框

IMPLEMENT_DYNAMIC(CIStyleBarDlg, CDialogEx)

CIStyleBarDlg::CIStyleBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIStyleBarDlg::IDD, pParent)
{

}

CIStyleBarDlg::~CIStyleBarDlg()
{
}

void CIStyleBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIStyleBarDlg, CDialogEx)
	ON_BN_CLICKED(ID_I_BAR_OK, &CIStyleBarDlg::OnBnClickedIBarOk)
END_MESSAGE_MAP()


// CIStyleBarDlg 消息处理程序


void CIStyleBarDlg::OnBnClickedIBarOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//SoNode *Finded = theApp.m_MainRoot->getByName("ProDepth");
	//theApp.blueDepth =  new SoDepthBuffer;
	//theApp.blueDepth->setName("ProDepth");
	//theApp.blueDepth->test.setValue(FALSE);
	//theApp.m_MainRoot->replaceChild()
	if (theApp.testX == 1)
	{
		//theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateStraightBar());
		theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateBar());
		theApp.testX = theApp.testX + 1;
		//theApp.m_pViewer->setSceneGraph(theApp.m_Selection);
	}
	else if (theApp.testX == 2)
	{
		SoTranslation * tempVec = new SoTranslation;
		tempVec->translation.setValue(0,50,0);
		theApp.m_MainRoot->addChild(tempVec);
		//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
		//blueDepth->range.setValue(SoDepthBuffer::ALWAYS);
		//theApp.m_MainRoot->addChild(blueDepth);
		theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateBar());
		//theApp.m_MainRoot->addChild(m_StrainghtBarFun.CreateStraightBar());
	}
	theApp.m_pViewer->viewAll();
	//theApp.blueDepth->test.setValue(TRUE);
	CDialogEx::OnCancel();
	//theApp.I_BarCreateDlg.CloseWindow();
}
