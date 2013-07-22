// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenInventor.h"
#include "TestDlg.h"
#include "afxdialogex.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEST, OutputText);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
END_MESSAGE_MAP()

void CTestDlg::setTestText( CString testStr )
{
	//CString cStr;
	LPCTSTR lpctStr=(LPCTSTR)testStr;
	//CString pp = _T("123");
	//pp.SetString("testing");
	OutputText.SetWindowTextW(lpctStr);
}



//void MarshalStrings(System::String^ s, string& os)//格式转换
//{
//	using namespace Runtime::InteropServices;
//	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
//	os = chars;
//	Marshal::FreeHGlobal(IntPtr((void*)chars));
//
//}
//void MarshalStrings(System::String ^ s, wstring& os )
//{
//	using namespace Runtime::InteropServices;
//	const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
//	os = chars;
//	Marshal::FreeHGlobal(IntPtr((void*)chars));
//}
// CTestDlg 消息处理程序
