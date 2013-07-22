// DecimationStrategyDialog.h : header file
//

#include <Inventor/Win/viewers/SoWinViewer.h>

/////////////////////////////////////////////////////////////////////////////
// CDecimationStrategyDialog dialog

class CDecimationStrategyDialog : public CDialog
{
// Construction
public:
	CDecimationStrategyDialog(CWnd* pParent = NULL);   // standard constructor
	
    BOOL Create( CView *pView, SoWinViewer *viewer);

// Dialog Data
	//{{AFX_DATA(CDecimationStrategyDialog)
	enum { IDD = IDD_DEC_STRAT_DIALOG };
	CStatic	m_staticPercent;
	CStatic	m_staticFPS;
	CSliderCtrl	m_sliderPercent;
	CSliderCtrl	m_sliderFPS;
	CButton	m_radioNormal;
	CEdit	m_editNumTris;
	CButton	m_checkFullRender;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecimationStrategyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	SoWinViewer *m_pAdaptiveViewer;

	void CheckStrategyRadioButton(SoWinViewer::DecimationStrategy strategy);

	// Generated message map functions
	//{{AFX_MSG(CDecimationStrategyDialog)
	afx_msg void OnCheckFull();
	afx_msg void OnChangeEditNumTris();
	afx_msg void OnRadioFps();
	afx_msg void OnRadioNormal();
	afx_msg void OnRadioPercent();
	afx_msg void OnRadioTris();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
