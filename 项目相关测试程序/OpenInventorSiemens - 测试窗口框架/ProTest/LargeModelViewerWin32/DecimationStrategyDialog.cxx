// DecimationStrategyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "DecimationStrategyDialog.h"

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDecimationStrategyDialog dialog


CDecimationStrategyDialog::CDecimationStrategyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDecimationStrategyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecimationStrategyDialog)
	//}}AFX_DATA_INIT



}

BOOL 
CDecimationStrategyDialog::Create(CView *pView, SoWinViewer *viewer)
{
	m_pAdaptiveViewer = viewer;

	BOOL res = TRUE;

	// Don't do anything if window is up.
	if (GetSafeHwnd() == 0) {
		//BOOL res = CDialog::Create(MAKEINTRESOURCE(IDD_DIALOG1),this);
		res = CDialog::Create(CDecimationStrategyDialog::IDD,pView);
	}

	if (res) ShowWindow(SW_SHOW);

	return res;

}

void CDecimationStrategyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecimationStrategyDialog)
	DDX_Control(pDX, IDC_STATIC_PERCENT, m_staticPercent);
	DDX_Control(pDX, IDC_STATIC_FPS, m_staticFPS);
	DDX_Control(pDX, IDC_SLIDER_PERCENT, m_sliderPercent);
	DDX_Control(pDX, IDC_SLIDER_FPS, m_sliderFPS);
	DDX_Control(pDX, IDC_RADIO_NORMAL, m_radioNormal);
	DDX_Control(pDX, IDC_EDIT_NUM_TRIS, m_editNumTris);
	DDX_Control(pDX, IDC_CHECK_FULL, m_checkFullRender);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDecimationStrategyDialog, CDialog)
	//{{AFX_MSG_MAP(CDecimationStrategyDialog)
	ON_BN_CLICKED(IDC_CHECK_FULL, OnCheckFull)
	ON_EN_CHANGE(IDC_EDIT_NUM_TRIS, OnChangeEditNumTris)
	ON_BN_CLICKED(IDC_RADIO_FPS, OnRadioFps)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, OnRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_PERCENT, OnRadioPercent)
	ON_BN_CLICKED(IDC_RADIO_TRIS, OnRadioTris)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecimationStrategyDialog message handlers

void CDecimationStrategyDialog::OnCheckFull() 
{
	  m_pAdaptiveViewer->enableFullRenderingWhenStill(
			 IsDlgButtonChecked( m_checkFullRender.GetDlgCtrlID()) );
}

void CDecimationStrategyDialog::OnChangeEditNumTris() 
{
	CString valueStr;
	m_editNumTris.GetWindowText( valueStr );

	int32_t numTris = atol( (const char *) valueStr);
	m_pAdaptiveViewer->setGoalNumberOfTriangles( numTris );

	CheckStrategyRadioButton(SoWinViewer::FIXED_NUM_TRIANGLES);

}

void CDecimationStrategyDialog::OnRadioFps() 
{
	// TODO: Add your control notification handler code here
	m_pAdaptiveViewer->setDecimationStrategy(SoWinViewer::FRAMES_PER_SECOND);
	
}

void CDecimationStrategyDialog::OnRadioNormal() 
{
	m_pAdaptiveViewer->setDecimationStrategy(SoWinViewer::NORMAL);
	
}

void CDecimationStrategyDialog::OnRadioPercent() 
{
	m_pAdaptiveViewer->setDecimationStrategy(SoWinViewer::FIXED_PERCENTAGE);
	
}

void CDecimationStrategyDialog::OnRadioTris() 
{
	m_pAdaptiveViewer->setDecimationStrategy(SoWinViewer::FIXED_NUM_TRIANGLES);
	
}

void CDecimationStrategyDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *slider = (CSliderCtrl*) pScrollBar;

	float value;
	char string[128];
	switch (pScrollBar->GetDlgCtrlID()) {
	case IDC_SLIDER_PERCENT:		
		value =  slider->GetPos()	/ 100.0f;
		m_pAdaptiveViewer->setFixedPercentage( value );
		CheckStrategyRadioButton(SoWinViewer::FIXED_PERCENTAGE);
		
		sprintf(string, "%d%%", slider->GetPos() );
		m_staticPercent.SetWindowText( string );
		break;
	case IDC_SLIDER_FPS:
		value = slider->GetPos()	/ 10.0f;
		m_pAdaptiveViewer->setGoalFramesPerSecond( value );
		CheckStrategyRadioButton(SoWinViewer::FRAMES_PER_SECOND); 

		sprintf(string, "%.1f", value );
		m_staticFPS.SetWindowText( string );
		break;
	default:
		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDecimationStrategyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char str[128];
	
	if (m_pAdaptiveViewer != NULL) {
		SoWinViewer::DecimationStrategy strategy =  
				m_pAdaptiveViewer->getDecimationStrategy()	;

		int32_t numTris = m_pAdaptiveViewer->getGoalNumberOfTriangles();
		sprintf(str, "%d", numTris);
		m_editNumTris.SetWindowText(str);
	
		float goalFps = m_pAdaptiveViewer->getGoalFramesPerSecond();
		int fps = (int) (goalFps * 10.0);
		if (fps < 0) fps = 0;
		else if (fps > 300) fps = 300;
		m_sliderFPS.SetRange(0, 300);
		m_sliderFPS.SetPos(fps);
		sprintf(str, "%.1f", goalFps);
		m_staticFPS.SetWindowText(str);
	
		int percent = (int) (m_pAdaptiveViewer->getFixedPercentage() * 100.0);
		m_sliderPercent.SetRange(0, 100);
		m_sliderPercent.SetPos(percent);
		sprintf(str, "%d%%", percent);
		m_staticPercent.SetWindowText(str);

		CheckDlgButton(	m_checkFullRender.GetDlgCtrlID(), 
			m_pAdaptiveViewer->isFullRenderingWhenStill());
	   
		CheckStrategyRadioButton( strategy );

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void 
CDecimationStrategyDialog::CheckStrategyRadioButton(SoWinViewer::DecimationStrategy strategy)
{
    	 CheckRadioButton( IDC_RADIO_NORMAL, IDC_RADIO_PERCENT, 
			IDC_RADIO_NORMAL + 	(int) strategy - SoWinViewer::NORMAL);

		m_pAdaptiveViewer->setDecimationStrategy(strategy);
}
