/*==============================================================================
 *   File        : MagelanMFCView.h
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : Interface of the CMagellanMfcView class.
 *
 *============================================================================*/

#if !defined(AFX_MAGELLANMFCVIEW_H__3309BEDC_4B83_4221_B380_DD9E579A3F19__INCLUDED_)
#define AFX_MAGELLANMFCVIEW_H__3309BEDC_4B83_4221_B380_DD9E579A3F19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/Xt/devices/SoXtSpaceball.h>

class SoWinExaminerViewer;
class SoSeparator;
class SoShape;
class SoTranslation;
class SoRotation;

class CMagellanMfcView : public CView
{
protected: // create from serialization only
	CMagellanMfcView();
	DECLARE_DYNCREATE(CMagellanMfcView)

// Attributes
public:
	CMagellanMfcDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagellanMfcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMagellanMfcView();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// BEGIN OIV
//
private:
	SoWinExaminerViewer *m_pViewer;
	SoSeparator *m_pSceneRoot;
	SoTranslation* m_translation;
	SoRotation *m_rotation;
	SoXtSpaceball *m_sb;
	float  m_rotScaleFactor;
	float  m_transScaleFactor;
	SbBool m_rotationMode;
	SbBool m_translationMode;

// Callback
	static void spaceballButtonCB( void *, SoEventCallback *cb );
	static void motion3TranslationCB( void *, SoEventCallback *cb );
	static void motion3RotationCB( void *, SoEventCallback *cb );
//
// END OIV

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMagellanMfcView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if !defined(_DEBUG)  // debug version in MagellanMfcView.cpp
inline CMagellanMfcDoc* CMagellanMfcView::GetDocument()
   { return (CMagellanMfcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGELLANMFCVIEW_H__3309BEDC_4B83_4221_B380_DD9E579A3F19__INCLUDED_)
