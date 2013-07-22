// RotateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "RotateDlg.h"
#include "afxdialogex.h"
#include <Inventor\nodes\SoFaceSet.h>


// CRotateDlg 对话框

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRotateDlg::IDD, pParent)
{
	m_path = theApp.m_Selection->getPath(0);
}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_RotateX);
	DDX_Control(pDX, IDC_COMBO6, m_RotateY);
	DDX_Control(pDX, IDC_COMBO7, m_RotateZ);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRotateDlg 消息处理程序


void CRotateDlg::OnBnClickedOk()
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
			SbString sbStr1,sbStr2,sbStr3, sbStr4, sbStr5, sbStrTemp;
			sbStr1.fromWideChar(theApp.SbName2WChar(dd));
			sbStr2.fromWideChar(_T("ROTATEX"));
			sbStrTemp = sbStr1+sbStr2;
		//	//theApp.myMessageBox(sbStr1);
			SoNode *testRotateX = testNode->getByName(sbStrTemp);
			if (testRotateX->isOfType(SoRotationXYZ::getClassTypeId()))
			{
				//theApp.myMessageBox(sbStr1);
				SbString diffuse,temp;
				SbName nameFiled;
				SoField *xx;
				nameFiled = "angle";
				xx = testRotateX->getField(nameFiled);
		//		xx->get(temp);
		//		//string dddf = temp.toStdString();
				CString XRotate, YRotate, ZRotate, totalMove;
				m_RotateX.GetWindowTextW(XRotate);
		//		m_Model_Move_X.GetWindowTextW(XMove);
		//		m_Model_Move_Y.GetWindowTextW(YMove);
		//		m_Model_Move_Z.GetWindowTextW(ZMove);
		//		totalMove = XMove + " " + YMove + " " + ZMove;
				temp = XRotate.GetBuffer();
				xx->set(temp);
			}
			//
			sbStr3.fromWideChar(_T("ROTATEY"));
			sbStrTemp = sbStr1+sbStr3;
			//	//theApp.myMessageBox(sbStr1);
			SoNode *testRotateY = testNode->getByName(sbStrTemp);
			if (testRotateY->isOfType(SoRotationXYZ::getClassTypeId()))
			{
				//theApp.myMessageBox(sbStr1);
				SbString diffuse,temp;
				SbName nameFiled;
				SoField *yy;
				nameFiled = "angle";
				yy = testRotateY->getField(nameFiled);
				//		xx->get(temp);
				//		//string dddf = temp.toStdString();
				CString XRotate, YRotate, ZRotate, totalMove;
				m_RotateY.GetWindowTextW(YRotate);
				//		m_Model_Move_X.GetWindowTextW(XMove);
				//		m_Model_Move_Y.GetWindowTextW(YMove);
				//		m_Model_Move_Z.GetWindowTextW(ZMove);
				//		totalMove = XMove + " " + YMove + " " + ZMove;
				temp = YRotate.GetBuffer();
				yy->set(temp);
			}
			//
			sbStr4.fromWideChar(_T("ROTATEZ"));
			sbStrTemp = sbStr1+sbStr4;
			//	//theApp.myMessageBox(sbStr1);
			SoNode *testRotateZ = testNode->getByName(sbStrTemp);
			if (testRotateZ->isOfType(SoRotationXYZ::getClassTypeId()))
			{
				//theApp.myMessageBox(sbStr1);
				SbString diffuse,temp;
				SbName nameFiled;
				SoField *zz;
				nameFiled = "angle";
				zz = testRotateZ->getField(nameFiled);
				//		xx->get(temp);
				//		//string dddf = temp.toStdString();
				CString XRotate, YRotate, ZRotate, totalMove;
				m_RotateZ.GetWindowTextW(ZRotate);
				//		m_Model_Move_X.GetWindowTextW(XMove);
				//		m_Model_Move_Y.GetWindowTextW(YMove);
				//		m_Model_Move_Z.GetWindowTextW(ZMove);
				//		totalMove = XMove + " " + YMove + " " + ZMove;
				temp = ZRotate.GetBuffer();
				zz->set(temp);
			}
		}

	}
	CDialogEx::OnOK();
}
