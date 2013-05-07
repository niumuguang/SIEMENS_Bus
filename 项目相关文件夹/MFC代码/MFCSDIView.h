// MFCSDIView.h : interface of the CMFCSDIView class
//
/////////////////////////////////////////////////////////////////////////////

class SoWinExaminerViewer;
class SoSeparator;
class SoShape;

class SbColor;

class CMFCSDIView : public CView
{
protected: // create from serialization only
	CMFCSDIView();
	DECLARE_DYNCREATE(CMFCSDIView)

// Attributes
public:
	CMFCSDIDoc* GetDocument();

//
// BEGIN OIV
//
// Data
protected:
	SoWinExaminerViewer *m_pViewer;

	SoSeparator *m_pSceneRoot;
//
// END OIV
//

// Operations
//
// BEGIN OIV
//
protected:
        void AddItem(SoShape *shape, SbColor &color);
//
// END OIV
//

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCSDIView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCSDIView();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFCSDIView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
// 	afx_msg void OnObjectCone();
// 	afx_msg void OnObjectCube();
 	afx_msg void OnObjectSphere();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if !defined(_DEBUG)  // debug version in MFCSDIView.cpp
inline CMFCSDIDoc* CMFCSDIView::GetDocument()
   { return (CMFCSDIDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
