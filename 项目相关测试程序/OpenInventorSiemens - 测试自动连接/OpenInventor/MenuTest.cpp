// MenuTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "MenuTest.h"

// CMenuTest

IMPLEMENT_DYNAMIC(CMenuTest, CMFCMenuButton)

CMenuTest::CMenuTest()
{

}

CMenuTest::~CMenuTest()
{
}


BEGIN_MESSAGE_MAP(CMenuTest, CMFCMenuButton)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMenuTest ��Ϣ�������




void CMenuTest::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
	//CMenuTest popMenu;

}
