// MoveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "MoveDlg.h"
#include "afxdialogex.h"



// CMoveDlg 对话框

IMPLEMENT_DYNAMIC(CMoveDlg, CDialogEx)

CMoveDlg::CMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoveDlg::IDD, pParent)
{
	m_path = theApp.m_Selection->getPath(0);

	//m_Model_Move_X.SetWindowTextW(_T("10"));
	//m_Model_Move_X.UpdateData(FALSE);
}

CMoveDlg::~CMoveDlg()
{
}

void CMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOVE_X, m_Model_Move_X);
	DDX_Control(pDX, IDC_MOVE_Y, m_Model_Move_Y);
	DDX_Control(pDX, IDC_MOVE_Z, m_Model_Move_Z);
}


BEGIN_MESSAGE_MAP(CMoveDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMoveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMoveDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMoveDlg 消息处理程序


void CMoveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_path->ref();
	SoSeparator * group = (SoSeparator *) m_path->getTail();
	if (group->isOfType(SoFaceSet::getClassTypeId()))
	{
		int testLength = m_path->getLength();
		SoNode * testNode = m_path->getNodeFromTail(2);
		if (testNode->isOfType(SoGroup::getClassTypeId()))
		{
			SbName dd = testNode->getName();
			SbString sbStr1,sbStr2;
			sbStr1.fromWideChar(theApp.SbName2WChar(dd));
			sbStr2.fromWideChar(_T("TRANSLATION"));
			sbStr1 = sbStr1+sbStr2;
			//theApp.myMessageBox(sbStr1);
			SoNode *testTranslation = testNode->getByName(sbStr1);
			if (testTranslation->isOfType(SoTranslation::getClassTypeId()))
			{
				SbString diffuse,temp;
				SbName nameFiled;
				SoField *xx;
				nameFiled = "translation";
				xx = testTranslation->getField(nameFiled);
				xx->get(temp);
				//string dddf = temp.toStdString();
				//theApp.myMessageBox(temp);

				temp = "0 1 0";
				xx->set(temp);
			}

		}

	}
	CDialogEx::OnOK();
}


void CMoveDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Model_Move_X.SetDlgItemInt(CMoveDlg::IDD,2,1);
	//m_Model_Move_X.SetDlgItemText(CMoveDlg::IDD, _T("10"));
	m_Model_Move_X.SetWindowTextW(_T("10"));
	m_Model_Move_X.UpdateData(FALSE);
}


BOOL CMoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Model_Move_X.SetWindowTextW(_T("10"));
	m_Model_Move_Y.SetWindowTextW(_T("0"));
	m_Model_Move_Z.SetWindowTextW(_T("0"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
