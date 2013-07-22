// TestDlg.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TestDlg.h"

#include "ExportDLL.h"
#include <Inventor\nodes\SoSeparator.h>
#include <Inventor\nodes\SoCube.h>
#include "..\..\OpenInventor\OpenInventor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTestDlgApp

BEGIN_MESSAGE_MAP(CTestDlgApp, CWinApp)
END_MESSAGE_MAP()


// CTestDlgApp 构造

CTestDlgApp::CTestDlgApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestDlgApp 对象

CTestDlgApp theAppDD;


// CTestDlgApp 初始化

BOOL CTestDlgApp::InitInstance()
{
	CWinApp::InitInstance();
	
	return TRUE;
}

extern "C" 
	__declspec( dllexport ) void mydlldlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nRet=-1;
	CExportDLL dlg;
	nRet = dlg.DoModal();
}

extern "C" 
	__declspec( dllexport ) LPCTSTR mydlldlgC(int a, int b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int c = a + b;
	LPCTSTR ss = _T("OK");
	return ss;
}

extern "C" 
	__declspec( dllexport ) void mydlldlgS(int a, int b, int c, COpenInventorApp *ttt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//SoSeparator ss;
	
	SoCube * cu = new SoCube;
	cu->width.setValue(a);
	cu->height.setValue(b);
	cu->depth.setValue(c);
	//theAppDD.restest->addChild(cu);
	ttt->m_MainRoot->addChild(cu);
}