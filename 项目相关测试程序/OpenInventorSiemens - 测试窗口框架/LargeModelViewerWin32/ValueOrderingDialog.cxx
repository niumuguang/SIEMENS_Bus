// ValueOrderingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "ValueOrderingDialog.h"
#include <Inventor/nodes/SoValueOrdering.h>

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValueOrderingDialog dialog


CValueOrderingDialog::CValueOrderingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CValueOrderingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValueOrderingDialog)
	//}}AFX_DATA_INIT

	m_pOrdering = NULL;
}


void CValueOrderingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueOrderingDialog)
	DDX_Control(pDX, IDC_CHECK_VDEC_SUBS, m_checkDecSubs);
	DDX_Control(pDX, IDC_STATIC_VDROP, m_staticDrop);
	DDX_Control(pDX, IDC_SLIDER_VDROP, m_sliderDrop);
	DDX_Control(pDX, IDC_CHECK_VDROP, m_checkDrop);
	DDX_Control(pDX, IDC_CHECK_VBBOX, m_checkBbox);
	DDX_Control(pDX, IDC_CHECK_VADJUST, m_checkAdjust);
	DDX_Control(pDX, IDC_CHECK_LOW_VALUE, m_checkLowValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueOrderingDialog, CDialog)
	//{{AFX_MSG_MAP(CValueOrderingDialog)
	ON_BN_CLICKED(IDC_CHECK_LOW_VALUE, OnCheckLowValue)
	ON_BN_CLICKED(IDC_CHECK_VADJUST, OnCheckAdjust)
	ON_BN_CLICKED(IDC_CHECK_VBBOX, OnCheckBbox)
	ON_BN_CLICKED(IDC_CHECK_VCACHING, OnCheckCaching)
	ON_BN_CLICKED(IDC_CHECK_VCULLING, OnCheckCulling)
	ON_BN_CLICKED(IDC_CHECK_VDROP, OnCheckDrop)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_VDEC_SUBS, OnCheckDecSubs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL 
CValueOrderingDialog::Create(CView *pView, SoValueOrdering *ordering)
{
	m_pOrdering = ordering;

	BOOL res = TRUE;

	if (GetSafeHwnd() == 0) {
		res = CDialog::Create(CValueOrderingDialog::IDD,pView);
	}

	if (res) ShowWindow(SW_SHOW);

	return res;

}

/////////////////////////////////////////////////////////////////////////////
// CValueOrderingDialog message handlers

void CValueOrderingDialog::OnCheckLowValue() 
{
	m_pOrdering->lowValueCulling.setValue(
			 IsDlgButtonChecked( m_checkLowValue.GetDlgCtrlID()) );


	
}

void CValueOrderingDialog::OnCheckAdjust() 
{
	m_pOrdering->adjustDecimation.setValue(
			 IsDlgButtonChecked( m_checkAdjust.GetDlgCtrlID()) );


	
}

void CValueOrderingDialog::OnCheckBbox() 
{
	m_pOrdering->boundingBoxSubstitutes.setValue(
			 IsDlgButtonChecked( m_checkBbox.GetDlgCtrlID()) );


	
}

void CValueOrderingDialog::OnCheckCaching() 
{
//	m_pOrdering->renderCaching.setValue(
//			 (IsDlgButtonChecked( m_checkCaching.GetDlgCtrlID()) ?
//			 SoSeparator::ON : SoSeparator::AUTO) );


	
}

void CValueOrderingDialog::OnCheckCulling() 
{
//	m_pOrdering->renderCulling.setValue(
//			 (IsDlgButtonChecked( m_checkCulling.GetDlgCtrlID()) ?
//			 SoSeparator::ON : SoSeparator::AUTO) );


	
}

void CValueOrderingDialog::OnCheckDrop() 
{
	m_pOrdering->dropCulling.setValue(
			 IsDlgButtonChecked( m_checkDrop.GetDlgCtrlID()) );

	
}

BOOL CValueOrderingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//CheckDlgButton(	m_checkCaching.GetDlgCtrlID(), 
	//	m_pOrdering->renderCaching.getValue() == SoSeparator::ON );
	//CheckDlgButton(	m_checkCulling.GetDlgCtrlID(), 
	//	m_pOrdering->renderCulling.getValue() == SoSeparator::ON );
	CheckDlgButton(	m_checkDrop.GetDlgCtrlID(), 
		m_pOrdering->dropCulling.getValue() );
	CheckDlgButton(	m_checkBbox.GetDlgCtrlID(), 
		m_pOrdering->boundingBoxSubstitutes.getValue() );
	CheckDlgButton(	m_checkDecSubs.GetDlgCtrlID(), 
		m_pOrdering->decimateSubstitutes.getValue() );   

	CheckDlgButton(	m_checkAdjust.GetDlgCtrlID(), 
		m_pOrdering->adjustDecimation.getValue() );
	CheckDlgButton(	m_checkLowValue.GetDlgCtrlID(), 
		m_pOrdering->lowValueCulling.getValue() );

	char str[128];
	int dropArea = m_pOrdering->dropScreenArea.getValue();
	if (dropArea < 1) dropArea = 1;
	else if (dropArea > 40) dropArea = 40;
	m_sliderDrop.SetRange(1, 40);
	m_sliderDrop.SetPos(dropArea);
	sprintf(str, "%d", dropArea);
	m_staticDrop.SetWindowText(str);
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CValueOrderingDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *slider = (CSliderCtrl*) pScrollBar;

	float value;
	char string[128];
	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_SLIDER_VDROP:		
		value =  (float)slider->GetPos();
		m_pOrdering->dropScreenArea.setValue((uint32_t)value);
		
		sprintf(string, "%d", slider->GetPos());
		m_staticDrop.SetWindowText( string );
		break;
	default:
		break;
	}

	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CValueOrderingDialog::OnCheckDecSubs() 
{
	m_pOrdering->decimateSubstitutes.setValue(
		IsDlgButtonChecked( m_checkDecSubs.GetDlgCtrlID()));
	
}
