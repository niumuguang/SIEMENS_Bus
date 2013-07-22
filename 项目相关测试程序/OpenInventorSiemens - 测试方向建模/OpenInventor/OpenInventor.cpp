
// OpenInventor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OpenInventor.h"
#include "MainFrm.h"

#include "OpenInventorDoc.h"
#include "OpenInventorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>


 //extern "C" {
 //	//extern void mydlldlg1(); 
 //	extern void mydlldlg();
 //	//	extern void dlldlg();
 //}
// COpenInventorApp

BEGIN_MESSAGE_MAP(COpenInventorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COpenInventorApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// COpenInventorApp 构造

COpenInventorApp::COpenInventorApp()
{
	m_HoleStatus = false;
	m_AlignName = "";
	m_AlignStatus = false;
	m_BodySel = false;
	testX = 1;
	testINT = 1;
	//////////////////////////////////////////////////////////////////////////
	// 自定义全局变量初始化
	m_rotationSwitch = false;
	m_translationSwitch = false;
	/*m_MousePos.setValue(0,0);
	m_RadiusView = 200;
	m_rotationSwitch = false;*/
	//m_Root = new SoSeparator;
	//
	m_bHiColorIcons = TRUE;
	ViewingToF = false;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OpenInventor.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 COpenInventorApp 对象

COpenInventorApp theApp;


// COpenInventorApp 初始化

BOOL COpenInventorApp::InitInstance()
{
	SoWin::init("");
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COpenInventorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(COpenInventorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int COpenInventorApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// COpenInventorApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void On32776();
	//afx_msg void OnDllDlg();
	afx_msg void OnCreatHead();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_NCRBUTTONUP()
	ON_COMMAND(ID_32776, &CAboutDlg::On32776)
	//ON_COMMAND(ID_DLL_DLG, &CAboutDlg::OnDllDlg)
	//ON_COMMAND(ID_CREAT_HEAD, &CAboutDlg::OnCreatHead)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void COpenInventorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COpenInventorApp 自定义加载/保存方法

void COpenInventorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COpenInventorApp::LoadCustomState()
{
}

void COpenInventorApp::SaveCustomState()
{
	//MessageBox(m_ViewHwnd,_T("Start"),_T("Start"),0);
}

wchar_t* COpenInventorApp::Cstring2Wchar( CString cstr )
{
	wchar_t* wCharString = cstr.GetBuffer(cstr.GetLength()+1); 
	return wCharString;
}

LPCTSTR COpenInventorApp::Char2LPCTSTR( char * charPtr )
{
	CString tempstr;
	tempstr = charPtr;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)tempstr.GetBuffer();
	return lpstr;
}

wchar_t* COpenInventorApp::Char2WChar(char* charPtr )
{
	size_t len = strlen(charPtr) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, charPtr, _TRUNCATE);
	return WStr;
}

const char * COpenInventorApp::CString2Char( CString cstr )
{
	char *p = (LPSTR)(LPCTSTR)cstr;
	return p;
}

void COpenInventorApp::myMessageBox( char * inputData )
{
	CString go;
	go = inputData;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	MessageBox(theApp.m_ViewHwnd,lpstr,_T("TestOut"),0);
}

void COpenInventorApp::myMessageBox( int inputData )
{
	char a[200];
	itoa(inputData, a, 10);
	CString go;
	go = a;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	MessageBox(theApp.m_ViewHwnd,lpstr,_T("TestOut"),0);
}

void COpenInventorApp::myMessageBox( SbString inputData )
{
	const char *ts = inputData.getString();
	CString go;	go = ts;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	MessageBox(theApp.m_ViewHwnd,lpstr,_T("TestOut"),0);
}

void COpenInventorApp::myMessageBox( SbName inputData )
{
	SbString tempStr;
	tempStr.fromWideChar(SbName2WChar(inputData));

	const char *ts = tempStr.getString();
	CString go;	go = ts;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	MessageBox(theApp.m_ViewHwnd,lpstr,_T("TestOut"),0);
}

wchar_t* COpenInventorApp::SbName2WChar( SbName  inputName )
{
	const char * sstring = inputName.getString();
	size_t len = strlen(sstring) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, sstring, _TRUNCATE);
	return WStr;
}

void COpenInventorApp::GenFiled( SbString tempCoor, float &getX, float &getY, float &getZ )
{
	SbString testCoor;testCoor = tempCoor;
	//testCoor = tempCoor.getSubString(0,tempCoor.getLength()-2);
	//theApp.myMessageBox(testCoor);
	int FirstSpacePos = testCoor.find(" ");
	
	testCoor = tempCoor.getSubString(FirstSpacePos+1, tempCoor.getLength());
	//myMessageBox(testCoor);
	//myMessageBox(FirstSpacePos);
	int SecSpacePos = testCoor.find(" ");
	//myMessageBox(SecSpacePos);
	//int sun = testCoor.getLength();
	//myMessageBox(sun);
	//myMessageBox(sun - SecSpacePos);
	SbString XStr;
	XStr = tempCoor.getSubString(0, FirstSpacePos);
	
	SbString YStr;
	YStr = tempCoor.getSubString(FirstSpacePos+1, SecSpacePos+1);

	SbString ZStr;
	ZStr = tempCoor.getSubString(SecSpacePos+1+1, tempCoor.getLength());
	/*tempCoor = tempCoor.getSubString(SecSpacePos, tempCoor.getLength());
	int ThirSpacePos = tempCoor.find(" ");

	SbString ZStr;
	ZStr = tempCoor.getSubString(0, ThirSpacePos);*/
	//myMessageBox(getX);
	//myMessageBox(getY);
	//myMessageBox(getZ);
	getX = XStr.toFloat();
	getY = YStr.toFloat();
	getZ = ZStr.toFloat();

}



void CAboutDlg::On32776()
{
	// viweing 测试代码
	// TODO: 在此添加命令处理程序代码
	if(theApp.ViewingToF == TRUE)
	{
		theApp.m_pViewer->setViewing(FALSE);
		theApp.ViewingToF = false;
	}
	else
	{
		theApp.m_pViewer->setViewing(TRUE);
		theApp.ViewingToF = true;
	}

}



//CDocument* COpenInventorApp::OpenDocumentFile(LPCTSTR lpszFileName)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CWinAppEx::OpenDocumentFile(lpszFileName);
//}



//void CAboutDlg::OnCreatHead()
//{
//	// TODO: 在此添加命令处理程序代码
//	
//
//}
