
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "OpenInventor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_UPDATEUISTATE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar ��Ϣ�������

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �������:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ���������� \n");
		return -1;      // δ�ܴ���
	}

	//m_wndObjectCombo.AddString(_T("Ӧ�ó���"));
	m_wndObjectCombo.AddString(_T("ģ������"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;      // δ�ܴ���
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
	
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("ģ�Ͳ���"));
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("�Ի������"), _T("����֮һ:���ޡ�����ϸ�������ɵ�����С���򡰶Ի������"));
	//pProp->AddOption(_T("��"));
	//pProp->AddOption(_T("ϸ"));
	//pProp->AddOption(_T("�ɵ�����С"));
	//pProp->AddOption(_T("�Ի������"));
	//pProp->AllowEdit(FALSE);

	//pGroup1->AddSubItem(pProp);
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));

	//m_wndPropList.AddProperty(pGroup1);
	theApp.pModelPos = new CMFCPropertyGridProperty(_T("ģ��λ��"), 0, TRUE);
	theApp.pPropXcoord = new CMFCPropertyGridProperty(_T("X����"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
	theApp.pPropXcoord->EnableSpinControl(TRUE, 50, 300);
	theApp.pModelPos->AddSubItem(theApp.pPropXcoord);
	pProp = new CMFCPropertyGridProperty( _T("Y����"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	theApp.pModelPos->AddSubItem(pProp);
	// �ı����ԶԻ���

	//pProp->SetValue((_variant_t) 333l);
	//m_wndPropList.AddProperty(pModelPos);
	

	CMFCPropertyGridProperty* pModelSize = new CMFCPropertyGridProperty(_T("ģ�Ͳ���"), 0, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Lģ�ͳ�"), (_variant_t) 200l, _T("ָ�����ڵĸ߶�"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	//&pProp.SetData(_T("s"));
	pModelSize->AddSubItem(pProp);
	
	pProp = new CMFCPropertyGridProperty( _T("Wģ�Ϳ�"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pModelSize->AddSubItem(pProp);
	
	pProp = new CMFCPropertyGridProperty( _T("Dģ����"), (_variant_t) 10l, _T("ָ�����ڵĿ��"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pModelSize->AddSubItem(pProp);
	//m_wndPropList.AddProperty(pModelSize);

	pGroup1->AddSubItem(theApp.pModelPos);
	pGroup1->AddSubItem(pModelSize);
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("ģ����Ϣ"));
	//pProp = new CMFCPropertyGridProperty(_T("�Ƿ�����"), (_variant_t)_T("������"), _T("ָ�����ڵĸ߶�"));
	//LOGFONT lf;
	//CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	//font->GetLogFont(&lf);

	//lstrcpy(lf.lfFaceName, _T("����, Arial"));
	CString sstemp  = _T("GG-ss1");
	//pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
	//pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�Ƿ�����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t)sstemp, _T("ģ�͵�����")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t)_T("A��������"), _T("ģ�͵�����: ���ӡ�ĳ����ӳ��ߡ�ĳ�������š�ĳ��ʼ��")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("����������"), (_variant_t)_T("I��"), _T("������ģ�͵�����: I�͡�L�͡�Z��")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t)_T("����ͭ��TMY8X100X350"), _T("�ӹ����ϲ���")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("A������"), (_variant_t)_T("None"), _T("������A������ģ������")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("B������"), (_variant_t)_T("GG-ss2"), _T("������B������ģ������")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��ע"), (_variant_t)theApp.testINT, _T("�û��Զ�����Ϣ����")));
	
	//CMFCPropertyGridProperty* pModelConnect = new CMFCPropertyGridProperty(_T("����״̬"), 0, TRUE);
	//pProp = new CMFCPropertyGridProperty(_T("A������"), (_variant_t)_T("ʼ��"), _T("ָ�����ڵĸ߶�"));
	//pModelConnect->AddSubItem(pProp);
	//pProp = new CMFCPropertyGridProperty(_T("B������"), (_variant_t)_T("GG-ss2"), _T("ָ�����ڵĸ߶�"));
	//pModelConnect->AddSubItem(pProp);
	
	//pGroup2->AddSubItem(pModelConnect);
	//m_wndPropList.AddProperty(pModelConnect);
	m_wndPropList.AddProperty(pGroup2);
	
	/*CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
	pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
	pColorProp->EnableOtherButton(_T("����..."));
	pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);*/
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


void CPropertiesWnd::OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/)
{
	// �ù���Ҫ��ʹ�� Windows 2000 ����߰汾��
	// ���� _WIN32_WINNT �� WINVER ���� >= 0x0500��
	// TODO: �ڴ˴������Ϣ����������

}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	printf("ss");
	return 0;
}
