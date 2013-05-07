/*==============================================================================
 *   File        : MagelanMFCView.cxx
 *   Author(s)   : P. ESTRADE
 *   Date        : 18 June 2001
 *==============================================================================
 *  Open Inventor (c) Copyright 2001 Mercury SAS, all rights reserved
 *==============================================================================
 *
 * Description : This file is a demonstration for the utilisation of Space Mouse
 *               int a Mfc program.
 *
 *============================================================================*/

#include "stdafx.h"
#include "MagellanMfc.h"

#include "MagellanMfcDoc.h"
#include "MagellanMfcView.h"

// BEGIN OIV
//
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/events/SoSpaceballButtonEvent.h>
#include <Inventor/events/SoMotion3Event.h>

extern "C" {
#include "mgldll.h"
}
//
// END OIV

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView

IMPLEMENT_DYNCREATE(CMagellanMfcView, CView)

BEGIN_MESSAGE_MAP(CMagellanMfcView, CView)
	//{{AFX_MSG_MAP(CMagellanMfcView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView construction/destruction

CMagellanMfcView::CMagellanMfcView()
{
	m_rotationMode    = 1;
	m_translationMode = 0;

}

CMagellanMfcView::~CMagellanMfcView()
{
}

BOOL CMagellanMfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView drawing

void CMagellanMfcView::OnDraw(CDC* pDC)
{
	CMagellanMfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView printing

BOOL CMagellanMfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMagellanMfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMagellanMfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView diagnostics

#if defined(_DEBUG)
void CMagellanMfcView::AssertValid() const
{
	CView::AssertValid();
}

void CMagellanMfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMagellanMfcDoc* CMagellanMfcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMagellanMfcDoc)));
	return (CMagellanMfcDoc*)m_pDocument;
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMagellanMfcView message handlers
void
CMagellanMfcView::spaceballButtonCB(void *userData, SoEventCallback *cb) {
	CMagellanMfcView* localView = (CMagellanMfcView*)userData;
	const SoSpaceballButtonEvent *ev = 
    (const SoSpaceballButtonEvent *) cb->getEvent();
  
  if (ev->getState() == SoButtonEvent::DOWN) {
    int which = ev->getButton();
    printf(" button %d\n",which);
    switch (which) {
    case 1:
      printf("rotation only mode \n");
      localView->m_rotationMode    = 1;
      localView->m_translationMode = 0;
	  //
      break;
    case 2:
      printf("translation only mode \n");
      localView->m_translationMode = 1;
      localView->m_rotationMode    = 0;
      break;
    case 3:
      printf("translation and rotation mode \n");
      localView->m_translationMode = 1;
      localView->m_rotationMode    = 1;
      break;
    case 4:
      localView->m_rotScaleFactor *= 2.;
      printf("Increasing Rotation Scale Factor : %f\n",localView->m_rotScaleFactor);
      localView->m_sb->setRotationScaleFactor(localView->m_rotScaleFactor);
      break;
    case 5:
      localView->m_rotScaleFactor /= 2.;
      printf("Decreasing Rotation Scale Factor : %f\n",localView->m_rotScaleFactor);
      localView->m_sb->setRotationScaleFactor(localView->m_rotScaleFactor);
      break;
    case 6:
      localView->m_transScaleFactor *= 2.;
      printf("Increasing Translation Scale Factor: %f\n",localView->m_transScaleFactor);
      localView->m_sb->setTranslationScaleFactor(localView->m_transScaleFactor);
      break;
    case 7:
      localView->m_transScaleFactor /= 2;
      printf("Decreasing Translation Scale Factor: %f\n",localView->m_transScaleFactor);
      localView->m_sb->setTranslationScaleFactor(localView->m_transScaleFactor);
      break;
    case 8:
      printf("Reset Position\n");
      localView->m_translation->translation.setValue(SbVec3f(0,0,0));
	  localView->m_rotation->rotation.setValue(SbRotation(1,1,0,0));
	  localView->m_translationMode = 0;
      localView->m_rotationMode    = 1;
      break;
    default: 
      break;
    }
  }
}
void
CMagellanMfcView::motion3TranslationCB(void *userData, SoEventCallback *cb) {
	CMagellanMfcView* localView = (CMagellanMfcView*)userData;

	if (!localView->m_translationMode) return;
	
	const SoMotion3Event *ev = (const SoMotion3Event *) cb->getEvent();
	localView->m_translation->translation.setValue( localView->m_translation->translation.getValue() + ev->getTranslation());
  cb->setHandled();
}
void
CMagellanMfcView::motion3RotationCB( void *userData, SoEventCallback *cb ) {
	CMagellanMfcView* localView = (CMagellanMfcView*)userData;

	if (!localView->m_rotationMode) return;
  
	const SoMotion3Event *ev = (const SoMotion3Event *) cb->getEvent();
  localView->m_rotation->rotation.setValue( localView->m_rotation->rotation.getValue() * ev->getRotation());
  cb->setHandled();

}
void CMagellanMfcView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
// BEGIN OIV
//
	m_pSceneRoot = new SoSeparator();
	m_pSceneRoot->ref();

	m_translation = new SoTranslation();
	m_pSceneRoot->addChild(m_translation);

	m_rotation = new SoRotation();
	m_pSceneRoot->addChild(m_rotation);

	SoEventCallback *cb = new SoEventCallback;
	m_pSceneRoot->addChild(cb);

	SoInput input;
	if (input.openFile("axis.iv") == NULL) {
		printf("Cannot open file axis.iv \n");
		exit(-1);
	}
	SoSeparator *axis = SoDB::readAll(&input);
	m_pSceneRoot->addChild(axis);
	
	// Set up event callbacks
	cb->addEventCallback(SoSpaceballButtonEvent::getClassTypeId(), spaceballButtonCB, this);
	cb->addEventCallback(SoMotion3Event::getClassTypeId(),motion3TranslationCB, this);
	cb->addEventCallback(SoMotion3Event::getClassTypeId(), motion3RotationCB, this);
	
	m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
	m_pViewer->setHeadlight(TRUE);
	m_pViewer->setSceneGraph(m_pSceneRoot);
	m_pViewer->setTitle("Demo Space Mouse with Mfc \n");

	if (! SoWinSpaceball::exists()) {
		fprintf(stderr, "Sorry, no Space Ball or Magellan Space Mouse on this display!\n");
	} 
	else {
		m_sb = new SoWinSpaceball();
		m_pViewer->registerDevice(m_sb);
		m_rotScaleFactor   = m_sb->getRotationScaleFactor();
		m_transScaleFactor = m_sb->getTranslationScaleFactor();
		
		printf("Default rotation scale factor %f\n",m_rotScaleFactor);
		printf("Default translation scale factor %f\n",m_transScaleFactor);
	}
	
	
	m_pViewer->show();	

	m_pViewer->setViewing(FALSE);   // come up in pick mode

}

void CMagellanMfcView::OnDestroy() 
{
// BEGIN OIV
	delete m_pViewer;
// END OIV
	CView::OnDestroy();
}
