// LargeModelViewerView.h : interface of the CLargeModelViewerView class
//
/////////////////////////////////////////////////////////////////////////////

class SoWinViewer;
class SoWinFullViewer;
class SoSeparator;
class SoShape;
class SoGetPrimitiveCountAction;
class SoSensor;
class SoOneShotSensor;
class CDecimationStrategyDialog;
class SbColor;

#include <Inventor/SbBasic.h>

enum LmvEViewer {
    LMV_VWR_EXAMINER = 0, 
    LMV_VWR_FLY, 
    LMV_VWR_WALK, 
    LMV_VWR_PLANE
};


class CLargeModelViewerView : public CView
{
protected: // create from serialization only
	CLargeModelViewerView();
	DECLARE_DYNCREATE(CLargeModelViewerView)


public:
	CLargeModelViewerDoc* GetDocument();


protected:
    SoWinViewer *m_pViewer;
    CDecimationStrategyDialog *m_decStrategyDlg;

	// Render Statistics :
    //void setRenderStats(SbBool onOff);
    //void setPrimitiveStats(SbBool onOff);
    float displayedFPS, displayedDecimation;
    int32_t dispNTris, dispNLines, dispNPoints, dispNText, dispNImage;
    
    //static void fpsCallback(float fps, void *, SoWinViewer *);
//     static void decimationPercentageCallback(float fps, 
//     				void *, SoWinViewer*);
    
    SoGetPrimitiveCountAction *countAction;
    SoOneShotSensor *countIdleSensor;
    //static void countIdleCB(void *data, SoSensor*);
    //void doPrimitiveCount();
    SbBool renderStats, primStats;

    SoWinFullViewer	*viewerList[5];	// list of viewers
    LmvEViewer		whichViewer;	// current viewer ID
    //void    	    	switchToViewer(LmvEViewer newViewer);


protected:
    //void AddItem(SoShape *shape, SbColor &color);


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLargeModelViewerView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLargeModelViewerView();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLargeModelViewerView)
	afx_msg void OnDestroy();
	//afx_msg void OnEditDecimation();
	//afx_msg void OnViewPrimStats();
	//afx_msg void OnViewRenderStats();
	//afx_msg void OnViewDelayTransp();
	//afx_msg void OnViewAntialiasing();
	//afx_msg void OnViewExaminer();
	//afx_msg void OnViewPlane();
	//afx_msg void OnViewWalk();
	//afx_msg void OnViewFly();
	// °´ÏÂÊó±ê×ó¼ü
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if !defined(_DEBUG)  // debug version in LargeModelViewerView.cpp
inline CLargeModelViewerDoc* CLargeModelViewerView::GetDocument()
   { return (CLargeModelViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
