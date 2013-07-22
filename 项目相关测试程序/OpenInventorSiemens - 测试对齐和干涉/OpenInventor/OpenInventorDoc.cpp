
// OpenInventorDoc.cpp : COpenInventorDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenInventor.h"
#endif

#include "OpenInventorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenInventorDoc

IMPLEMENT_DYNCREATE(COpenInventorDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenInventorDoc, CDocument)
END_MESSAGE_MAP()


// COpenInventorDoc 构造/析构

COpenInventorDoc::COpenInventorDoc()
{
	// TODO: 在此添加一次性构造代码
	root = NULL;
}

COpenInventorDoc::~COpenInventorDoc()
{
	if (root)
	{
		root->unref();
	}
}





// COpenInventorDoc 序列化

void COpenInventorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void COpenInventorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void COpenInventorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void COpenInventorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenInventorDoc 诊断

#ifdef _DEBUG
void COpenInventorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenInventorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenInventorDoc 命令
BOOL COpenInventorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 文件存储

	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL COpenInventorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL COpenInventorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	theApp.m_Selection->deselectAll();
	int rootNum = theApp.m_MainRoot->getNumChildren();
	if (rootNum > 0)
	{
		//for (int i = theApp.m_MainRoot->getNumChildren(); i>0; i--)
		//{
		theApp.m_MainRoot->removeChild(rootNum-1);
		//}
	}
	//for (int i = theApp.m_Selection->getNumChildren(); i>0; i--)
	//	theApp.m_Selection->removeChild(i-1);
	//theApp.m_MainRoot->unref();
	//////////////////////////////////////////////////////////////////////////
	SoInput mySceneInput;
	//C:\Users\Lio\Documents\GitHub\OpenInventorSiemens\OpenInventor\banana.iv

	SbString dir = "C:\\Users\\Lio\\Documents\\GitHub\\OpenInventorSiemens\\OpenInventor\\banana.iv";
	dir = lpszPathName;
	if (mySceneInput.openFile(dir) == FALSE)
	{
		MessageBox(theApp.m_ViewHwnd, _T("打开文件失败"),_T("打开文件失败"),0);
	}
	SoSeparator *myGraph = SoDB::readAll(&mySceneInput);

	if (myGraph == NULL)
	{
		MessageBox(theApp.m_ViewHwnd, _T("打开文件为空"),_T("打开文件为空"),0);
	}
	//SoSeparator *None = new SoSeparator();
	//theApp.m_MainRoot = new SoSeparator();
	//theApp.m_MainRoot->unref();
	//theApp.m_MainRoot->setRefCount(0);
	//theApp.m_MainRoot->addChild(myGraph);
	//theView.OnInitialUpdate();
	//myGraph->ref();
	//theApp.m_MainRoot = new SoSeparator();
	SoSeparator * test = new SoSeparator();
	test->addChild(myGraph);
	if (theApp.m_MainRoot->getNumChildren() == 0)
	{
		printf("ssss");
	}
	if (theApp.m_MainRoot->getNumChildren() == 1)
	{
		printf("ssss");
	}
	theApp.m_MainRoot->addChild(test);
	if (theApp.m_MainRoot->getNumChildren() == 1)
	{
		printf("ssss");
	}
	if (theApp.m_MainRoot->getNumChildren() == 2)
	{
		printf("ssss");
	}
	//int sstest = theApp.m_Selection->getNumChildren();
//	theApp.m_Selection->addChild(theApp.m_MainRoot);
	//myGraph->unref();
	
	//theApp.m_pViewer->setSceneGraph(theApp.m_MainRoot);
	theApp.m_pViewer->viewAll();
	mySceneInput.closeFile();
	return TRUE;
}

BOOL COpenInventorDoc::SaveModified()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDocument::SaveModified();
}