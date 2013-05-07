// MFCSDIView.cpp : implementation of the CMFCSDIView class
//

#include "stdafx.h"
#include "MFCSDI.h"

#include "MFCSDIDoc.h"
#include "MFCSDIView.h"

//
// BEGIN OIV
//
#include <Inventor/SbColor.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFColor.h>
//
// END OIV
//

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIView

IMPLEMENT_DYNCREATE(CMFCSDIView, CView)

BEGIN_MESSAGE_MAP(CMFCSDIView, CView)
	//{{AFX_MSG_MAP(CMFCSDIView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
// 	ON_COMMAND(ID_OBJECT_CONE, OnObjectCone)
// 	ON_COMMAND(ID_OBJECT_CUBE, OnObjectCube)
// 	ON_COMMAND(ID_OBJECT_SPHERE, OnObjectSphere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIView construction/destruction

CMFCSDIView::CMFCSDIView()
{
	// TODO: add construction code here

}

CMFCSDIView::~CMFCSDIView()
{
}

BOOL CMFCSDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIView drawing

void
CMFCSDIView::OnDraw(CDC *)//pDC)
{
	CMFCSDIDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIView diagnostics

#if defined(_DEBUG)
void CMFCSDIView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSDIDoc* CMFCSDIView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSDIDoc)));
	return (CMFCSDIDoc*)m_pDocument;
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCSDIView message handlers

void CMFCSDIView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
//
// BEGIN OIV
//
	//SoWinMouse* ss;
	
	m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
	//m_pViewer->setCursorStyle(SoWinViewer::CLASSIC);
	//m_pViewer->setCursorStyle(SoWinViewer::CursorStyle::CLASSIC);
	m_pViewer->setDecoration(false);
	m_pViewer->setViewing(false);
	//m_pViewer->setViewingMode(SoWinExaminerViewer::ViewingMode::VIEWING_MODE_PAN);
	//m_pViewer->setViewerMode();
	//m_pViewer->removeViewingMouseBinding();
	//m_pViewer->hdc
	m_pSceneRoot = new SoSeparator();
	m_pViewer->setSceneGraph(m_pSceneRoot);
			//  we have one viewer per MDI window and
			//  one scenegraph for each.  Note if
			//  we wanted to share the scenegraph
			//  among several viewers, we'd probably
			//  want to create the separator under
			//  the Doc class and then propogate it
			//  to each viewer as it's created.
	OnObjectSphere();
	m_pSceneRoot->get
	m_pSceneRoot->getByName()
	m_pViewer->show();
//
// END OIV
}

void CMFCSDIView::OnDestroy() 
{
//
// BEGIN OIV
//
	delete m_pViewer;
//
// END OIV
//
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

//
//  BEGIN OIV
//
//  Note:
//  since this is a pretty minimal example, we've just stuck the
//  command handlers for the various objects here.  These command
//  handlers could have just as easily (and appropriately) been
//  put under the document class, as could have the root separator.
//  you need to determine what makes sense for your application.
//
// void CMFCSDIView::OnObjectCone() 
// {
// //   static SbColor color(1., 0., 0.);
// //   SoShape *object = new SoCone();
// //   AddItem(object, color);
// }

// void CMFCSDIView::OnObjectCube() 
// {
//   //static SbColor color(0., 1., 0.);
//   //SoShape *object = new SoCube();
//   //AddItem(object, color);
// }

void CMFCSDIView::OnObjectSphere() 
{
	static SbColor color(0., 0., 1.);
	SoShape *object = new SoSphere();
	AddItem(object, color);
}

void CMFCSDIView::AddItem(SoShape *shape, SbColor &color)
{
	SoMaterial *material = new SoMaterial();
	material->diffuseColor.setValue(color);

	SoTransform *transform = new SoTransform();
	transform->translation.setValue(3., 0., 0.);

	m_pSceneRoot->addChild(material);
	m_pSceneRoot->addChild(transform);
	m_pSceneRoot->addChild(shape);

	m_pViewer->viewAll();
}

// This method will be called on WM_ERASEBKGND messages
// We override the default CView method to avoid unnecessary erasing of
// the background, because the OIV viewer window completely covers the
// background (parent window) anyway.  This can cause "flashing".
// Return true to indicate that we have handled the message.
BOOL CMFCSDIView::OnEraseBkgnd( CDC* pDC )
{
  return TRUE;
}
//
//  END OIV
//
