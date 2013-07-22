// ValueOrderingDialog.h : header file


class SoValueOrdering;

/////////////////////////////////////////////////////////////////////////////
// CValueOrderingDialog dialog

class CValueOrderingDialog : public CDialog
{
// Construction
public:
	CValueOrderingDialog(CWnd* pParent = NULL);   // standard constructor  

	// create and display dialog:
	BOOL Create(CView *pView, SoValueOrdering *ordering);


// Dialog Data
	//{{AFX_DATA(CValueOrderingDialog)
	enum { IDD = IDD_VALUE_ORD_DIALOG };
	CButton	m_checkDecSubs;
	CStatic	m_staticDrop;
	CSliderCtrl	m_sliderDrop;
	CButton	m_checkDrop;
	CButton	m_checkBbox;
	CButton	m_checkAdjust;
	CButton	m_checkLowValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValueOrderingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	SoValueOrdering *m_pOrdering;

	// Generated message map functions
	//{{AFX_MSG(CValueOrderingDialog)
	afx_msg void OnCheckLowValue();
	afx_msg void OnCheckAdjust();
	afx_msg void OnCheckBbox();
	afx_msg void OnCheckCaching();
	afx_msg void OnCheckCulling();
	afx_msg void OnCheckDrop();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckDecSubs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
