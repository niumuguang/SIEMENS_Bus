// OctreeOrderingDialog.h : header file
//

class SoOctreeOrdering;

/////////////////////////////////////////////////////////////////////////////
// COctreeOrderingDialog dialog

class COctreeOrderingDialog : public CDialog
{
// Construction
public:
	COctreeOrderingDialog(CWnd* pParent = NULL);   // standard constructor

	// create and display dialog:
	BOOL Create(SoOctreeOrdering *ordering);

// Dialog Data
	//{{AFX_DATA(COctreeOrderingDialog)
	enum { IDD = IDD_OCTREE_ORD_DIALOG };
	CButton	m_checkDraw;
	CButton	m_checkMoving;
	CButton	m_checkAdjust;
	CStatic	m_staticDrop;
	CSliderCtrl	m_sliderDrop;
	CButton	m_checkDrop;
	CButton	m_checkBbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COctreeOrderingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	SoOctreeOrdering *m_pOrdering;

	// Generated message map functions
	//{{AFX_MSG(COctreeOrderingDialog)
	afx_msg void OnCheckBbox();
	afx_msg void OnCheckCaching();
	afx_msg void OnCheckCulling();
	afx_msg void OnCheckDrop();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckAdjust();
	afx_msg void OnCheckMoving();
	afx_msg void OnCheckOdraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
