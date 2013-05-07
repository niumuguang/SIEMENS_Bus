#ifdef DVD_DEMO
#include <Inventor/lock/SoLockMgr.h>
#endif

#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h> 
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>

#include <DialogViz/SoDialogVizAll.h>

#include <Inventor/nodes/SoExtSelection.h>

SoXtExaminerViewer *myViewer;
SoTopLevelDialog* myTop;
SoDialogLabel *labFPS, *labNOD, *lab, *lab2;
SoDialogComboBox* cb, *cb2;

SoSwitch* switch0;
SoSeparator* separator_Meteorite_Cloud;
SbString fpsStr = "Frame rate = ";
SoComplexity* sceneComplexity;

SoSeparator* g_rulerSceneGraph = NULL;

SoLineSet* g_selectionSgLineSet = NULL;
SoTransform* g_textTransform = NULL;
SoText3* g_text = NULL;

SoDialogComboBox* g_modeCombo;

SoExtSelection* g_fastSelection;

SbVec2s g_startPosition;
SbVec3f g_startScenePosition;

int g_mode = 1;

/*************************************************************************/
SbBool onStartRulerCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  SoSeparator* separator = (SoSeparator*)userData;

  if ( !g_selectionSgLineSet )
  {
    separator->removeAllChildren();

    SoOrthographicCamera *cam = new SoOrthographicCamera;
    {
      cam->viewportMapping.setValue( SoCamera::LEAVE_ALONE );
      separator->addChild( cam );
    }

    SoLightModel *light_model = new SoLightModel;
    {
      light_model->model.setValue(SoLightModel::BASE_COLOR);
      separator->addChild( light_model );
    }

    SoMaterial* material = new SoMaterial;
    {
      material->diffuseColor.setValue( 0.0, 1.0, 0.0 );
      separator->addChild( material );
    }

    g_selectionSgLineSet = new SoLineSet;
    {
      SoVertexProperty* vp = new SoVertexProperty;

      g_selectionSgLineSet->vertexProperty = vp;

      separator->addChild( g_selectionSgLineSet );
    }

    g_textTransform = new SoTransform;
    separator->addChild( g_textTransform );
    
    SoFont* font = new SoFont;
    font->size = 12;

    SoScale* scale = new SoScale;
    scale->scaleFactor.setValue( 0.01f, 0.01f, 0.01f );

    separator->addChild( scale );

    separator->addChild( font );

    g_text = new SoText3;
    separator->addChild( g_text );

    g_text->justification = SoText3::CENTER;
  }

  SoVertexProperty* vp = (SoVertexProperty*)g_selectionSgLineSet->vertexProperty.getValue();

  g_selectionSgLineSet->numVertices.set1Value( 0, 2 );
  g_selectionSgLineSet->numVertices.set1Value( 1, 2 );
  g_selectionSgLineSet->numVertices.set1Value( 2, 2 );
  g_selectionSgLineSet->numVertices.set1Value( 3, 2 );
  g_selectionSgLineSet->numVertices.set1Value( 4, 2 );

  vp->vertex.setNum( 10 );
  vp->vertex.set1Value( 0, scenePosition[0] - 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 1, scenePosition[0] + 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 2, scenePosition[0], scenePosition[1] - 0.02f, 0.f );
  vp->vertex.set1Value( 3, scenePosition[0], scenePosition[1] + 0.02f, 0.f );

  vp->vertex.set1Value( 4, scenePosition[0] - 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 5, scenePosition[0] + 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 6, scenePosition[0], scenePosition[1] - 0.02f, 0.f );
  vp->vertex.set1Value( 7, scenePosition[0], scenePosition[1] + 0.02f, 0.f );

  vp->vertex.set1Value( 8, scenePosition[0], scenePosition[1], 0.f );
  vp->vertex.set1Value( 9, scenePosition[0], scenePosition[1], 0.f );

  g_text->string = "";

  g_startPosition = position;
  g_startScenePosition = scenePosition;

  return TRUE;
}

/*************************************************************************/
SbBool onEndRulerCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  g_text->string = "";
  g_selectionSgLineSet->numVertices = 0;

  return TRUE;
}

/*************************************************************************/
SbBool onMoveRulerCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  SoVertexProperty* vp = (SoVertexProperty*)g_selectionSgLineSet->vertexProperty.getValue();

  vp->vertex.set1Value( 4, scenePosition[0] - 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 5, scenePosition[0] + 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 6, scenePosition[0], scenePosition[1] - 0.02f, 0.f );
  vp->vertex.set1Value( 7, scenePosition[0], scenePosition[1] + 0.02f, 0.f );

  vp->vertex.set1Value( 9, scenePosition[0], scenePosition[1], 0.f );

  SbVec2s vect = position - g_startPosition;
  SbVec3f vectScene = scenePosition - g_startScenePosition;

  SbVec3f origin = g_startScenePosition;

  if ( vectScene[0] < 0.0 )
  {
    origin = scenePosition;
    vectScene = g_startScenePosition - scenePosition;
  }

  float angle = atan2( vectScene[1], vectScene[ 0 ] );

  SbMatrix matRot1;
  SbMatrix matRot2;

  matRot1.setRotate( SbRotation(SbVec3f (0, 1, 0), 0.0 ) );
  matRot2.setRotate( SbRotation( SbVec3f (0, 0, 1), angle ) );
  matRot1.multRight( matRot2 );

  g_textTransform->rotation.setValue( SbRotation( matRot1 ) );
  g_textTransform->translation.setValue( origin[0] + vectScene[0] / 2.f, 
                                         origin[1] + vectScene[1] / 2.f, 0.f );

  char str[ 128 ];

  sprintf( str, "%d px", int( sqrt( double( vect[0]*vect[0] + vect[1]*vect[1] )  ) ) );

  g_text->string = str;

  return TRUE;
}

/*************************************************************************/
SbBool onStartEllipseCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  return onStartRulerCallback( position, scenePosition, selection, userData );
}


/*************************************************************************/
SbBool onMoveEllipseCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  SoVertexProperty* vp = (SoVertexProperty*)g_selectionSgLineSet->vertexProperty.getValue();

  vp->vertex.set1Value( 4, scenePosition[0] - 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 5, scenePosition[0] + 0.02f, scenePosition[1], 0.f );
  vp->vertex.set1Value( 6, scenePosition[0], scenePosition[1] - 0.02f, 0.f );
  vp->vertex.set1Value( 7, scenePosition[0], scenePosition[1] + 0.02f, 0.f );

  SbVec3f ellipseSize = scenePosition - g_startScenePosition;

  double b = ellipseSize[1];
  double a = ellipseSize[0];

  int points = 0;

  for ( double i = -M_PI; i <= M_PI + 0.1; i += 0.1, points++ )
  {
    double x = g_startScenePosition[0] + a * cos( i );
    double y = g_startScenePosition[1] + b * sin( i );

    vp->vertex.set1Value( 8 + points, x, y, 0.f );
  }

  g_selectionSgLineSet->numVertices.set1Value( 4, points );

  return TRUE;
}


/*************************************************************************/
SbBool onEndEllipseCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  return onEndRulerCallback( position, scenePosition, selection, userData );
}


/*************************************************************************/
SbBool onStartSelectionCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  switch( g_mode )
  {
    // Draw the ruler
    case 1:  return onStartRulerCallback( position, scenePosition, selection, userData ); break;
    // Draw the Ellipse
    case 2:  return onStartEllipseCallback( position, scenePosition, selection, userData ); break;
  }

  return FALSE;
}

/*************************************************************************/
SbBool onMoveSelectionCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  switch( g_mode )
  {
    // Draw the ruler
    case 1:  return onMoveRulerCallback( position, scenePosition, selection, userData ); break;
    // Draw the Ellipse
    case 2:  return onMoveEllipseCallback( position, scenePosition, selection, userData ); break;
  }

  return FALSE;
}

/*************************************************************************/
SbBool onEndSelectionCallback( const SbVec2s& position, const SbVec3f& scenePosition, SoExtSelection* selection, void* userData )
{
  switch( g_mode )
  {
    // Draw the ruler
    case 1:  return onEndRulerCallback( position, scenePosition, selection, userData ); break;
    // Draw the Ellipse
    case 2:  return onEndEllipseCallback( position, scenePosition, selection, userData ); break;
  }

  return FALSE;
}

/*************************************************************************/
void
updateCount()
// Description :
//-------------------------------------------------------------------------
{
  SoGetPrimitiveCountAction* countAction = new SoGetPrimitiveCountAction;
  countAction->apply(separator_Meteorite_Cloud);
  int32_t numTris = countAction->getTriangleCount();

  SbString str = "Num Triangles for Meteoroid Cloud: ";
  str += numTris;
  lab->label = str;
}

class MyAuditorPB : public SoDialogChoiceAuditor
{
public:
  void dialogChoice(SoDialogChoice* pb) {
    
    if ( pb == g_modeCombo )
    {
      int mode = pb->selectedItem.getValue();

      if ( mode < 3 ) 
      {
        g_fastSelection->setSelectionSceneGraph( NULL );
        if ( mode == 0 )
          g_fastSelection->lassoType = SoExtSelection::LASSO;
        else
          if ( mode == 1 )
            g_fastSelection->lassoType = SoExtSelection::RECTANGLE;
          else
            if ( mode == 2 )
              g_fastSelection->lassoType = SoExtSelection::ELLIPSE;

        g_mode = 0;
      }
      else
      {
        g_mode = mode - 2;
        g_fastSelection->setSelectionSceneGraph( g_rulerSceneGraph );
      }

      return;
    }
  }
};

class ComplexitySliderAuditor : public SoDialogRealSliderAuditor
{
public:
  void dialogRealSlider(SoDialogRealSlider* slid) {
    sceneComplexity->value = slid->value.getValue();
    updateCount();
  }
};

void
fpsCallback(float fps, void *userData, SoXtViewer *viewer)
{
  SbString str = fpsStr;
  str += (int)fps;
  str += " fps.";
  myTop->label = str;
}

int
main(int, char **argv)
{
#ifdef DVD_DEMO
#include <lock/demoUnlock.cxx>
#endif

  Widget myWindow = SoXt::init(argv[0]); 
  if (myWindow == NULL)
    exit(1);

  SoPreferences::setValue("OIV_BUFFER_REGION_ENABLE", "0");

  int i;
  SoDialogViz::init();

  SoSeparator *root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

  SoPerspectiveCamera* camera = new SoPerspectiveCamera;
  
  sceneComplexity = new SoComplexity;
  sceneComplexity->value = 0.8;

  #define NB 1000.0
  SoSphere* sphere = new SoSphere;

  separator_Meteorite_Cloud = new SoSeparator;
  separator_Meteorite_Cloud->renderCaching = SoSeparator::OFF;
  separator_Meteorite_Cloud->addChild(sceneComplexity);

  for (i = 0; i<NB; i++) {
    SoSeparator* sep = new SoSeparator;
    sep->renderCaching = SoSeparator::OFF;

    float theta = 3.1415926535897932384626433832795 * rand() / RAND_MAX;
    float phi = 6.283185307179586476925286766559 * rand() / RAND_MAX;
    float r = NB / 1000 * 150 * rand() / RAND_MAX;
    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    SoTranslation* trans = new SoTranslation;
    trans->translation.setValue(x, y, z);
    sep->addChild(trans);
    sep->addChild(sphere);
    separator_Meteorite_Cloud->addChild(sep);
  }

  root->addChild(separator_Meteorite_Cloud);

  SoSeparator* separator_Sun = new SoSeparator;
  separator_Sun->renderCaching = SoSeparator::OFF;

  SoBaseColor* color_Sun = new SoBaseColor;
  color_Sun->rgb.setValue(1, 1, 0);
  separator_Sun->addChild(color_Sun);

  SoSphere * my_Sun = new SoSphere;
  my_Sun->radius = 20 * NB / 1000;
  separator_Sun->addChild(my_Sun);

  root->addChild(separator_Sun);


  myTop = new SoTopLevelDialog();
  myTop->label =  "FastSelection Demo" ;
  myTop->position.setValue(0,0);
  
  SoDialogCustom* myCustom = new SoDialogCustom;
  myCustom->width = 600;
  myCustom->height = 400;
  myTop->addChild(myCustom);

  SoColumnDialog* col3 = new SoColumnDialog;
  col3->fixedHeight = TRUE;
  myTop->addChild(col3);

  lab = new SoDialogLabel();
  lab->label = "Num Triangles for Meteoroid Cloud: 0000000000";
  lab->fixedWidth = TRUE;
  col3->addChild(lab);

  SoDialogRealSlider * complexSlid = new SoDialogRealSlider;
  complexSlid->min = 0.0;
  complexSlid->max = 1.0;
  complexSlid->value = 0.7;
  complexSlid->colors.set1Value(0, SbColor(0, 1, 0));
  complexSlid->colors.set1Value(1, SbColor(1, 0, 0));
  complexSlid->toggleVisible = FALSE;
  complexSlid->addAuditor(new ComplexitySliderAuditor());
  complexSlid->fixedLimits = TRUE;
  complexSlid->fixedWidth = TRUE;
  complexSlid->width = 300;
  complexSlid->label = "Complexity of the \\n meteoroid cloud: ";
  col3->addChild(complexSlid);

  SoColumnDialog* col2 = new SoColumnDialog();
  col2->fixedHeight = TRUE;
  myTop->addChild(col2);

  SoColumnDialog* col = new SoColumnDialog();
  col->fixedHeight = TRUE;
  myTop->addChild(col);

  g_modeCombo = new SoDialogComboBox;
  g_modeCombo->label = "Selection mode ";
  g_modeCombo->items.set1Value(0, "Lasso");
  g_modeCombo->items.set1Value(1, "Rectangle");
  g_modeCombo->items.set1Value(2, "Ellipse");
  g_modeCombo->items.set1Value(3, "Ruler");
  g_modeCombo->items.set1Value(4, "Aperture");
  g_modeCombo->selectedItem = 3;
  g_modeCombo->fixedWidth = TRUE;
  col->addChild(g_modeCombo);
  g_modeCombo->addAuditor(new MyAuditorPB);

  myTop->buildDialog(myWindow, TRUE);
  myTop->show();

  SoSeparator* viewerRoot = new SoSeparator;
  viewerRoot->ref();
  viewerRoot->renderCaching = SoSeparator::OFF;

  viewerRoot->addChild( camera );

  g_rulerSceneGraph = new SoSeparator;
  g_rulerSceneGraph->ref();
  g_rulerSceneGraph->renderCaching = SoSeparator::OFF;

  g_fastSelection = new SoExtSelection;
  g_fastSelection->useFastEditing( TRUE );

  g_fastSelection->setLassoColor(SbColor(0, 1, 0));
  g_fastSelection->animateOverlayLasso( TRUE );

  g_fastSelection->policy = SoSelection::SINGLE;
  g_fastSelection->lassoType = SoExtSelection::LASSO;
  g_fastSelection->lassoPolicy = SoExtSelection::PART_BBOX;
  g_fastSelection->lassoMode = SoExtSelection::ALL_SHAPES;

  g_fastSelection->setSelectionSceneGraph( g_rulerSceneGraph );
  g_fastSelection->setStartSelectionCallback( onStartSelectionCallback, g_rulerSceneGraph );
  g_fastSelection->setEndSelectionCallback( onEndSelectionCallback, g_rulerSceneGraph );
  g_fastSelection->setMovingSelectionCallback( onMoveSelectionCallback, g_rulerSceneGraph );

  g_fastSelection->addChild( root );

  viewerRoot->addChild( g_fastSelection );

  myViewer = new SoXtExaminerViewer(myCustom->getWidget());
  myViewer->setGLRenderAction(new SoBoxHighlightRenderAction());

  myViewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
  myViewer->setFastEditSavePolicy(SoGLRenderAction::EACH_FRAME);
  //myViewer->setFramesPerSecondCallback(fpsCallback, NULL);

  myViewer->redrawOnSelectionChange((SoSelection*)g_fastSelection);

  myViewer->setSceneGraph(viewerRoot);
  myViewer->setCamera( camera );
  
  myViewer->setSize(SbVec2s(600,400));
  myViewer->viewAll() ;
  myViewer->show();

  updateCount();

  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoDialogViz::finish();
  SoXt::finish();

  return 0;
}
