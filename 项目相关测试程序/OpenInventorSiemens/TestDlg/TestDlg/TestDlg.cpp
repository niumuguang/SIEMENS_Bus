// TestDlg.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CTestDlgApp

BEGIN_MESSAGE_MAP(CTestDlgApp, CWinApp)
END_MESSAGE_MAP()


// CTestDlgApp ����

CTestDlgApp::CTestDlgApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTestDlgApp ����

CTestDlgApp theAppDD;


// CTestDlgApp ��ʼ��

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