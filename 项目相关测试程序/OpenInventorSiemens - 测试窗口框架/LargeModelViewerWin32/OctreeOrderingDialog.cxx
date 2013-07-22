// OctreeOrderingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "OctreeOrderingDialog.h" 
#include <Inventor/nodes/SoOctreeOrdering.h>


#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COctreeOrderingDialog dialog


COctreeOrderingDialog::COctreeOrderingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COctreeOrderingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COctreeOrderingDialog)
	//}}AFX_DATA_INIT

	m_pOrdering = NULL;

}


void COctreeOrderingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COctreeOrderingDialog)
	DDX_Control(pDX, IDC_CHECK_ODRAW, m_checkDraw);
	DDX_Control(pDX, IDC_CHECK_OMOVING, m_checkMoving);
	DDX_Control(pDX, IDC_CHECK_OADJUST, m_checkAdjust);
	DDX_Control(pDX, IDC_STATIC_ODROP, m_staticDrop);
	DDX_Control(pDX, IDC_SLIDER_ODROP, m_sliderDrop);
	DDX_Control(pDX, IDC_CHECK_ODROP, m_checkDrop);
	DDX_Control(pDX, IDC_CHECK_OBBOX, m_checkBbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COctreeOrderingDialog, CDialog)
	//{{AFX_MSG_MAP(COctreeOrderingDialog)
	ON_BN_CLICKED(IDC_CHECK_OBBOX, OnCheckBbox)
	ON_BN_CLICKED(IDC_CHECK_OCACHING, OnCheckCaching)
	ON_BN_CLICKED(IDC_CHECK_OCULLING, OnCheckCulling)
	ON_BN_CLICKED(IDC_CHECK_ODROP, OnCheckDrop)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_OADJUST, OnCheckAdjust)
	ON_BN_CLICKED(IDC_CHECK_OMOVING, OnCheckMoving)
	ON_BN_CLICKED(IDC_CHECK_ODRAW, OnCheckOdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL 
COctreeOrderingDialog::Create(SoOctreeOrdering *ordering)
{
	m_pOrdering = ordering;

	BOOL res = TRUE;

	if (GetSafeHwnd() == 0) {
		res = CDialog::Create(COctreeOrderingDialog::IDD, NULL);
	}

	if (res) ShowWindow(SW_SHOW);

	return res;

}


/////////////////////////////////////////////////////////////////////////////
// COctreeOrderingDialog message handlers

void COctreeOrderingDialog::OnCheckBbox() 
{
	 m_pOrdering->boundingBoxSubstitutes.setValue(
			 IsDlgButtonChecked( m_checkBbox.GetDlgCtrlID()) );

	
}

void COctreeOrderingDialog::OnCheckCaching() 
{
//	m_pOrdering->renderCaching.setValue(
//			 (IsDlgButtonChecked( m_checkCaching.GetDlgCtrlID()) ?
//			 SoSeparator::ON : SoSeparator::AUTO));


	
}

void COctreeOrderingDialog::OnCheckCulling() 
{
//	m_pOrdering->renderCulling.setValue(
//			 (IsDlgButtonChecked( m_checkCulling.GetDlgCtrlID()) ?
//			 SoSeparator::ON : SoSeparator::AUTO) );


	
}

void COctreeOrderingDialog::OnCheckDrop() 
{
	m_pOrdering->dropCulling.setValue(
			 IsDlgButtonChecked( m_checkDrop.GetDlgCtrlID()) );

	
}

BOOL COctreeOrderingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	CheckDlgButton(	m_checkCaching.GetDlgCtrlID(), 
//		m_pOrdering->renderCaching.getValue() == SoSeparator::ON );
//	CheckDlgButton(	m_checkCulling.GetDlgCtrlID(), 
//		m_pOrdering->renderCulling.getValue() == SoSeparator::ON );
	CheckDlgButton(	m_checkDrop.GetDlgCtrlID(), 
		m_pOrdering->dropCulling.getValue() );
	CheckDlgButton(	m_checkBbox.GetDlgCtrlID(), 
		m_pOrdering->boundingBoxSubstitutes.getValue() );
	CheckDlgButton(	m_checkAdjust.GetDlgCtrlID(), 
		m_pOrdering->adjustDecimation.getValue() );
	CheckDlgButton(	m_checkMoving.GetDlgCtrlID(), 
		m_pOrdering->useMovingQueue.getValue() );
	CheckDlgButton(	m_checkDraw.GetDlgCtrlID(), 
		m_pOrdering->isOctreeDrawing() );
	   
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

void COctreeOrderingDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *slider = (CSliderCtrl*) pScrollBar;

	float value;
	char string[128];
	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_SLIDER_ODROP:		
		value = (float)slider->GetPos();
		m_pOrdering->dropScreenArea.setValue((uint32_t)value);
		
		sprintf(string, "%d", slider->GetPos());
		m_staticDrop.SetWindowText( string );
		break;
	default:
		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COctreeOrderingDialog::OnCheckAdjust() 
{
	m_pOrdering->adjustDecimation.setValue(
		IsDlgButtonChecked( m_checkAdjust.GetDlgCtrlID()));

	
}

void COctreeOrderingDialog::OnCheckMoving() 
{
	m_pOrdering->useMovingQueue.setValue(
		IsDlgButtonChecked( m_checkMoving.GetDlgCtrlID()));

}

void COctreeOrderingDialog::OnCheckOdraw() 
{
	// TODO: Add your control notification handler code here
	m_pOrdering->setOctreeDrawing(
		IsDlgButtonChecked( m_checkDraw.GetDlgCtrlID()));

	m_pOrdering->touch();
	
}
