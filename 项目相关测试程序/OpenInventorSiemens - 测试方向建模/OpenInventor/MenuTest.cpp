// MenuTest.cpp : 实现文件
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



// CMenuTest 消息处理程序




void CMenuTest::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
	//CMenuTest popMenu;

}
