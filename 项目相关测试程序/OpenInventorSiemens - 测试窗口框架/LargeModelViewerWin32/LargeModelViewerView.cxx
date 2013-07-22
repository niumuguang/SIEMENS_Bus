// LargeModelViewerView.cpp : implementation of the CLargeModelViewerView class
//

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "MainFrm.h"

#include "LargeModelViewerDoc.h"
#include "LargeModelViewerView.h"
//#include "DecimationStrategyDialog.h"

//
// BEGIN OIV
//
#include <Inventor/SbColor.h>
#include <Inventor/Win/viewers/SoWinFullViewer.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinWalkViewer.h>
#include <Inventor/Win/viewers/SoWinFlyViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFColor.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>



//
// END OIV
//

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView

IMPLEMENT_DYNCREATE(CLargeModelViewerView, CView)

BEGIN_MESSAGE_MAP(CLargeModelViewerView, CView)
    //{{AFX_MSG_MAP(CLargeModelViewerView)
    ON_WM_DESTROY()
    ON_COMMAND(ID_EDIT_DECIMATION, OnEditDecimation)
	ON_COMMAND(IDM_VIEW_PRIM_STATS, OnViewPrimStats)
	ON_COMMAND(IDM_VIEW_RENDER_STATS, OnViewRenderStats)
	ON_COMMAND(IDM_VIEW_DELAY_TRANSP, OnViewDelayTransp)
	ON_COMMAND(IDM_VIEW_ANTIALIASING, OnViewAntialiasing)
	ON_COMMAND(IDM_VIEW_EXAMINER, OnViewExaminer)
	ON_COMMAND(IDM_VIEW_PLANE, OnViewPlane)
	ON_COMMAND(IDM_VIEW_WALK, OnViewWalk)
	ON_COMMAND(IDM_VIEW_FLY, OnViewFly)
ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView construction/destruction

CLargeModelViewerView::CLargeModelViewerView()
{
  // TODO: add construction code here
 // m_decStrategyDlg = new CDecimationStrategyDialog();
  m_pViewer = NULL;

  renderStats = TRUE;
  primStats = TRUE;
  displayedFPS = -1;
  displayedDecimation = 1.0;
  dispNTris = dispNLines = dispNPoints = dispNText = dispNImage = 0;

  countAction = new SoGetPrimitiveCountAction;
  countIdleSensor = new SoOneShotSensor(countIdleCB, this);

  // Allocate only one viewer at a time. The other viewers will
  // be allocated as needed to increase speed and save memory.
  //
  for (int i=0; i<4; i++) {
    viewerList[i] = NULL;
  }
}

CLargeModelViewerView::~CLargeModelViewerView()
{
  //delete m_decStrategyDlg;
  delete countAction;
  delete countIdleSensor;
}

BOOL 
CLargeModelViewerView::PreCreateWindow(CREATESTRUCT &cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //       the CREATESTRUCT cs

  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView drawing

void 
CLargeModelViewerView::OnDraw(CDC *)//pDC)
{
  CLargeModelViewerDoc *pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView diagnostics

#if defined(_DEBUG)
void 
CLargeModelViewerView::AssertValid() const
{
  CView::AssertValid();
}

void 
CLargeModelViewerView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CLargeModelViewerDoc *
CLargeModelViewerView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLargeModelViewerDoc)));
  return (CLargeModelViewerDoc*)m_pDocument;
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView support functions


////////////////////////////////////////////////////////////////////////
//
// Description:
//	switches from the current viewer to the given viewer. The new
//  viewer will automatically be allocated if needed, and set to have 
//  the same settings the current viewer has (drawing style, buffer 
//  type, etc..).
//
// Use: private
void
CLargeModelViewerView::switchToViewer(LmvEViewer newViewer)
//
////////////////////////////////////////////////////////////////////////
{
  if (whichViewer == newViewer)
    return;

  // remove the sensor + scene graph from the old viewer (to prevent
  // an unecessary redraw of the old viewer)
  m_pViewer->setAutoRedraw(FALSE);
  m_pViewer->setSceneGraph(NULL);

  // remove statistic callbacks from current viewer
  SbBool wasRenderStats = renderStats;
  SbBool wasPrimStats = primStats;
  setRenderStats(FALSE);
  setPrimitiveStats(FALSE);

  // allocate the viewer if needed and set the window title.
  // all viewers share the same highlight render action.
  switch (newViewer) {
  case LMV_VWR_EXAMINER:
    // examiner vwr is already created in build()
    //setTitle("LargeModelViewer (Examiner)");
    break;
  case LMV_VWR_FLY:
    if ( viewerList[newViewer] == NULL ) {
      viewerList[newViewer] = new SoWinFlyViewer(GetSafeHwnd());
      //viewerList[newViewer]->setGLRenderAction(highlightRA);
      //viewerList[newViewer]->redrawOnSelectionChange(selection);
    }
    //setTitle("LargeModelViewer (Fly)");
    break;
  case LMV_VWR_WALK:
    if ( viewerList[newViewer] == NULL ) {
      viewerList[newViewer] = new SoWinWalkViewer(GetSafeHwnd());
      //viewerList[newViewer]->setGLRenderAction(highlightRA);
      //viewerList[newViewer]->redrawOnSelectionChange(selection);
    }
    //setTitle("LargeModelViewer (Walk)");
    break;
  case LMV_VWR_PLANE:
    if ( viewerList[newViewer] == NULL ) {
      viewerList[newViewer] = new SoWinPlaneViewer(GetSafeHwnd());
      //viewerList[newViewer]->setGLRenderAction(highlightRA);
      //viewerList[newViewer]->redrawOnSelectionChange(selection);
    }
    //setTitle("LargeModelViewer (Plane)");
    break;
  }
  SoWinFullViewer *newVwr = viewerList[newViewer];

  // re-init the render action (since it is shared between viewers) now that
  // we are changing windows + set the new scene graph
  newVwr->setBackgroundColor( m_pViewer->getBackgroundColor() );
  newVwr->setAutoRedraw(TRUE);
  SoSeparator *pSceneRoot = GetDocument()->getSceneRoot();
  newVwr->setSceneGraph(pSceneRoot);	        
  newVwr->getGLRenderAction()->invalidateState();

  //
  // make sure the new viewer has all the same settings as 
  // the current viewer. 
  //

  newVwr->setClearBeforeRender(m_pViewer->isClearBeforeRender());
  // XtViewer methods
  newVwr->setHeadlight(m_pViewer->isHeadlight());
  newVwr->setDrawStyle(SoWinViewer::STILL,
                       m_pViewer->getDrawStyle(SoWinViewer::STILL));
  newVwr->setDrawStyle(SoWinViewer::INTERACTIVE,
                       m_pViewer->getDrawStyle(SoWinViewer::INTERACTIVE));
  newVwr->setBufferingType(m_pViewer->getBufferingType());
  newVwr->setTransparencyType(m_pViewer->getTransparencyType());
  newVwr->setViewing(m_pViewer->isViewing());
  newVwr->setAutoClipping(m_pViewer->isAutoClipping());
  newVwr->setSeekTime(m_pViewer->getSeekTime());
  // WinFullViewer methods
  newVwr->setDecoration(((SoWinFullViewer *)m_pViewer)->isDecoration());

  // Transfer Decimation strategy values:
  newVwr->setDecimationStrategy(m_pViewer->getDecimationStrategy());
  newVwr->setGoalFramesPerSecond(m_pViewer->getGoalFramesPerSecond());
  newVwr->setGoalNumberOfTriangles(m_pViewer->getGoalNumberOfTriangles());
  newVwr->setFixedPercentage(m_pViewer->getFixedPercentage());
  newVwr->enableFullRenderingWhenStill(m_pViewer->isFullRenderingWhenStill());
	  
  // build and layout the new viewer
  //buildAndLayoutViewer(newVwr);

  // finally switch to the new viewer by showing the new viewer, 
  // and hiding the old viewer (hide is done last to reduce flicker).
  newVwr->show();
  m_pViewer->hide();
  whichViewer = newViewer;
  m_pViewer = newVwr;

  if (wasRenderStats)
    setRenderStats(TRUE);
  if (wasPrimStats)
    setPrimitiveStats(TRUE);
}

void
CLargeModelViewerView::setRenderStats(SbBool onOff)
{
  renderStats = onOff;
  //if (onOff) {
  //  // Set intitial numSamples to 1 so we get something
  //  m_pViewer->setNumSamples(1);
  //  m_pViewer->setFramesPerSecondCallback(fpsCallback, this);
  //}
  //else {
  //  m_pViewer->setFramesPerSecondCallback(NULL);
  //  ((CMainFrame *)AfxGetMainWnd())->SetFpsPane("");
  //}
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem( IDM_VIEW_RENDER_STATS, 
  renderStats ? MF_CHECKED : MF_UNCHECKED);
}

void
CLargeModelViewerView::setPrimitiveStats(SbBool onOff)
{
  primStats = onOff;
  if (onOff) { 
    m_pViewer->setDecimationPercentageCallback(decimationPercentageCallback, this);
    countIdleSensor->schedule();
  }
  else {         
    m_pViewer->setDecimationPercentageCallback(NULL);
    countIdleSensor->unschedule();
    ((CMainFrame *)AfxGetMainWnd())->SetDecimationPane("");
    displayedDecimation = -1.0; 
    dispNTris = dispNLines = dispNPoints = dispNText = dispNImage = 0;
  }
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem( IDM_VIEW_PRIM_STATS, 
  primStats ? MF_CHECKED : MF_UNCHECKED);
}

void
CLargeModelViewerView::fpsCallback(float fps, void* userData, SoWinViewer *viewer)
{
  CLargeModelViewerView *thisp = (CLargeModelViewerView *) userData;

  if (thisp->displayedFPS != fps) {
    char string[128];
    sprintf(string, "%.1f fps", fps);
    ((CMainFrame*)AfxGetMainWnd())->SetFpsPane(string);
    thisp->displayedFPS = fps;
  }

  // Gradually increase number of samples:
  if (viewer->getNumSamples() < 8) {
    viewer->setNumSamples(2 * viewer->getNumSamples());
  }
}

void
CLargeModelViewerView::decimationPercentageCallback(float decimation, 
                                                    void *userData, 
                                                    SoWinViewer *)
{
  CLargeModelViewerView *thisp = (CLargeModelViewerView *) userData;

  if (thisp->displayedDecimation != decimation) {
    char string[128];

    sprintf(string, "%.1f %%", 100.0 * decimation);
    ((CMainFrame *)AfxGetMainWnd())->SetDecimationPane(string);
    thisp->displayedDecimation = decimation;
  }
  // Get our idle sensor to trigger a primitive count later:
  if (!thisp->countIdleSensor->isScheduled()) {
    thisp->countIdleSensor->schedule();
  }
}

void 
CLargeModelViewerView::countIdleCB(void *data, SoSensor*)
{
	CLargeModelViewerView *thisp = (CLargeModelViewerView *) data;
	thisp->doPrimitiveCount();
}
    
void
CLargeModelViewerView::doPrimitiveCount()
{	
  // Get full scene graph, and current decimation values:
  SoNode *graph = m_pViewer->getSceneManager()->getSceneGraph();
  SoGLRenderAction *glAction = m_pViewer->getGLRenderAction();
  countAction->setDecimationValue(glAction->getDecimationType(), 
                                  glAction->getDecimationPercentage());

  // Apply primitive count action:	
  countAction->apply(graph);

  int32_t numTris   = countAction->getTriangleCount();
  int32_t numLines  = countAction->getLineCount();
  int32_t numPoints = countAction->getPointCount();
  int32_t numText   = countAction->getTextCount();
  int32_t numImage  = countAction->getImageCount();

  if ((dispNTris != numTris) ||
      (dispNLines != numLines) ||
      (dispNPoints != numPoints) ||
      (dispNText != numText) ||
      (dispNImage != numImage)) {
    char string[128];
    SbString countStr;
    if (numTris > 0) {
      sprintf(string, " %d Triangles", numTris);
      countStr += string;
    }
    if (numLines > 0) {
      sprintf(string, " %d Lines", numLines);
      countStr += string;
    }
    if (numPoints > 0) {
      sprintf(string, " %d Points", numPoints);
      countStr += string;
    }
    if (numText > 0) {
      sprintf(string, " %d Text", numText);
      countStr += string;
    }
    if (numImage > 0) {
      sprintf(string, " %d Image", numImage);
      countStr += string;
    }

    ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(countStr.getString());

    dispNTris   = numTris;	
    dispNLines  = numLines;
    dispNPoints = numPoints;
    dispNText   = numText;
    dispNImage  = numImage;	
  }
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerView message handlers

void 
CLargeModelViewerView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  if (!m_pViewer) {
    whichViewer = LMV_VWR_EXAMINER;
    m_pViewer = viewerList[whichViewer] = new SoWinExaminerViewer(GetSafeHwnd());
    m_pViewer->setBackgroundColor(SbColor(0.2f ,0.4f ,0.5f));

    // Set initial settings for Adaptive Viewing
    m_pViewer->setGoalFramesPerSecond(5.0f);
    m_pViewer->setDecimationStrategy(SoWinViewer::FRAMES_PER_SECOND);
    m_pViewer->enableFullRenderingWhenStill(TRUE);

    SoSeparator *pSceneRoot = GetDocument()->getSceneRoot();
	//pSceneRoot->addChild(new SoCube);
    m_pViewer->setSceneGraph(pSceneRoot);	        
    m_pViewer->show();

    // Set up frames per second, etc. callback
    setRenderStats(renderStats);
    setPrimitiveStats(primStats);
  }
  m_pViewer->viewAll();
}

void 
CLargeModelViewerView::OnDestroy() 
{
  // Destroy our viewers:
  delete (SoWinExaminerViewer *)viewerList[LMV_VWR_EXAMINER];
  delete (SoWinFlyViewer *)viewerList[LMV_VWR_FLY];
  delete (SoWinWalkViewer *)viewerList[LMV_VWR_WALK];
  delete (SoWinPlaneViewer *)viewerList[LMV_VWR_PLANE];
  CView::OnDestroy();
}

void 
CLargeModelViewerView::OnUpdate(CView *,//pSender, 
                                LPARAM,// lHint, 
                                CObject *)//pHint) 
{
}

BOOL 
CLargeModelViewerView::Create(LPCTSTR lpszClassName, 
                              LPCTSTR lpszWindowName, 
                              DWORD dwStyle, 
                              const RECT& rect, 
                              CWnd *pParentWnd, 
                              UINT nID, 
                              CCreateContext *pContext) 
{	
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, 
                      rect, pParentWnd, nID, pContext);
}

void 
CLargeModelViewerView::OnEditDecimation() 
{
  //m_decStrategyDlg->Create(this, m_pViewer);
}

void 
CLargeModelViewerView::OnViewPrimStats() 
{
  setPrimitiveStats(!primStats);
}

void 
CLargeModelViewerView::OnViewRenderStats() 
{
  setRenderStats(!renderStats);	
}

void 
CLargeModelViewerView::OnViewDelayTransp() 
{
  SbBool delayed = 
    (m_pViewer->getTransparencyType() == SoGLRenderAction::DELAYED_BLEND);
  // Change mode:
  delayed = !delayed;

  m_pViewer->setTransparencyType((delayed) ? 
                                 SoGLRenderAction::DELAYED_BLEND : 
                                 SoGLRenderAction::SCREEN_DOOR);

  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_DELAY_TRANSP, 
                       delayed ? MF_CHECKED : MF_UNCHECKED);
}

void 
CLargeModelViewerView::OnViewAntialiasing() 
{
  float qual;
  SbBool fsaa = ((SoWinGLWidget*)m_pViewer)->getFullSceneAntialiasing(qual);
  m_pViewer->setFullSceneAntialiasing(!fsaa, 0.75);

  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_ANTIALIASING, !fsaa ? MF_CHECKED : MF_UNCHECKED);

}

void 
CLargeModelViewerView::OnViewExaminer() 
{
  switchToViewer(LMV_VWR_EXAMINER);
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_EXAMINER, MF_CHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_FLY, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_WALK, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_PLANE, MF_UNCHECKED);
}

void 
CLargeModelViewerView::OnViewPlane() 
{
  switchToViewer(LMV_VWR_PLANE); 
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_EXAMINER, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_FLY, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_WALK, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_PLANE, MF_CHECKED);
}

void 
CLargeModelViewerView::OnViewWalk() 
{
  switchToViewer(LMV_VWR_WALK);   
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_EXAMINER, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_FLY, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_WALK, MF_CHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_PLANE, MF_UNCHECKED);
}

void CLargeModelViewerView::OnViewFly() 
{
  switchToViewer(LMV_VWR_FLY);   
  CMenu *pMenu = AfxGetMainWnd()->GetMenu();
  pMenu->CheckMenuItem(IDM_VIEW_EXAMINER, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_FLY, MF_CHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_WALK, MF_UNCHECKED);
  pMenu->CheckMenuItem(IDM_VIEW_PLANE, MF_UNCHECKED);
}

void CLargeModelViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//hemishere(x,y, wndHeight, lastPos);
	CView::OnLButtonDown(nFlags, point);
}