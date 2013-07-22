
// OpenInventorDoc.cpp : COpenInventorDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// COpenInventorDoc ����/����

COpenInventorDoc::COpenInventorDoc()
{
	// TODO: �ڴ����һ���Թ������
	root = NULL;
}

COpenInventorDoc::~COpenInventorDoc()
{
	if (root)
	{
		root->unref();
	}
}





// COpenInventorDoc ���л�

void COpenInventorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void COpenInventorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void COpenInventorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// COpenInventorDoc ���

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


// COpenInventorDoc ����
BOOL COpenInventorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	// �ļ��洢

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

	// TODO:  �ڴ������ר�õĴ�������
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
		MessageBox(theApp.m_ViewHwnd, _T("���ļ�ʧ��"),_T("���ļ�ʧ��"),0);
	}
	SoSeparator *myGraph = SoDB::readAll(&mySceneInput);

	if (myGraph == NULL)
	{
		MessageBox(theApp.m_ViewHwnd, _T("���ļ�Ϊ��"),_T("���ļ�Ϊ��"),0);
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
	// TODO: �ڴ����ר�ô����/����û���

	return CDocument::SaveModified();
}