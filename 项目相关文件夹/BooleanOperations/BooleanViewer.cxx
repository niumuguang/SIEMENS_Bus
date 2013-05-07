// Include files
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/events/SoKeyboardEvent.h>

void onStartBoolean(void *, SoAction *);
void onMiddleBoolean(void *, SoAction *);
void onEndBoolean(void *, SoAction *);
void drawOldTool(void *, SoAction *);
void drawNewTool(void *, SoAction *);
void endDrawTool(void *, SoAction *);
void drawTool(void *, SoAction *);
void onkey(void *, SoEventCallback *);
void root2sensorcb(void *, SoSensor *);

SoXtExaminerViewer *myViewer;
SoSeparator *root;
SoSwitch *sw;
SoPerspectiveCamera *camera;
SoSwitch *toolDrawSwitch;

int
main(int, char **argv)
{
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL) 
    exit(1);
  
  // Root of the scene graph
  root = new SoSeparator;
  root->ref();

  camera = new SoPerspectiveCamera;
  root->addChild(camera);

  // Node to catch key events
  SoEventCallback *cbk = new SoEventCallback;
  cbk->addEventCallback(SoKeyboardEvent::getClassTypeId(), onkey, NULL);
  root->addChild(cbk);

  sw = new SoSwitch;
  sw->whichChild.setValue(SO_SWITCH_ALL);
  root->addChild(sw);

  // Part of the scene graph containing the material to be machined
  // (the material is symbolized by a cube)
  SoSeparator *material_root = new SoSeparator;
  sw->addChild(material_root);

  SoMaterial *material_mat = new SoMaterial;
  material_mat->diffuseColor.setValue(0.0, 1.0, 0.0);
  material_root->addChild(material_mat);
  
  SoCube *material = new SoCube;
  material->width.setValue(6);
  material->height.setValue(2);
  material->depth.setValue(6);
  material_root->addChild(material);

  // Part of the scene graph containing the tool
  // (the tool is symbolized by a cylinder)
  SoSeparator *tool_root = new SoSeparator;
  sw->addChild(tool_root);

  SoMaterial *tool_mat = new SoMaterial;
  tool_mat->diffuseColor.setValue(1.0, 0.0, 0.0);
  tool_root->addChild(tool_mat);

  SoTranslation *tool_curve = new SoTranslation;
  tool_root->addChild(tool_curve);

  // Calculator and time couter engines to 
  // release the motion of the tool.
  SoCalculator *calcXZ  = new SoCalculator; 
  SoTimeCounter *thetaCounter = new SoTimeCounter;

  thetaCounter->max = 3600;
  thetaCounter->step = 1;
  thetaCounter->frequency = 0.02f;

  calcXZ->a.connectFrom(&thetaCounter->output);    
  calcXZ->expression.set1Value(0, "ta=a/1800*M_PI");
  calcXZ->expression.set1Value(1, "oA[0]=2*cos(2*ta)"); // x
  calcXZ->expression.set1Value(2, "oA[1]=1");           // y
  calcXZ->expression.set1Value(3, "oA[2]=2*sin(5*ta)"); // z
  tool_curve->translation.connectFrom(&calcXZ->oA);

  SoSeparator *tool = new SoSeparator;
  SoSeparator *toolDraw = new SoSeparator;
  tool_root->addChild(toolDraw);

  SoDrawStyle *style2 = new SoDrawStyle;
  style2->style = SoDrawStyle::LINES;
  style2->lineWidth = 1;
  toolDraw->addChild(style2);

  SoLightModel *lModel = new SoLightModel;
  lModel->model = SoLightModel::BASE_COLOR;
  toolDraw->addChild(lModel);

  SoCallback *cbk_node4 = new SoCallback;
  cbk_node4->setCallback(drawTool, NULL);
  toolDraw->addChild(cbk_node4);

  toolDrawSwitch = new SoSwitch;
  toolDrawSwitch->whichChild = SO_SWITCH_NONE;
  toolDraw->addChild(toolDrawSwitch);
  toolDrawSwitch->addChild(tool);

  SoCallback *cbk_node6 = new SoCallback;
  cbk_node6->setCallback(endDrawTool, NULL);
  toolDraw->addChild(cbk_node6);

  SoCallback *cbk_node1 = new SoCallback;
  cbk_node1->setCallback(onStartBoolean, NULL);
  tool_root->addChild(cbk_node1);

  tool_root->addChild(tool);

  SoCylinder *shape = new SoCylinder;
  shape->radius = 0.25;
  tool->addChild(shape);

  SoCallback *cbk_node2 = new SoCallback;
  cbk_node2->setCallback(onMiddleBoolean, NULL);
  tool_root->addChild(cbk_node2);

  tool_root->addChild(tool);

  SoCallback *cbk_node3 = new SoCallback;
  cbk_node3->setCallback(onEndBoolean, NULL);
  tool_root->addChild(cbk_node3);

  tool_root->addChild(toolDraw);

  // Cube containing the material and the
  // tool. This cube must be absolutly present.
  SoCube *machine_area = new SoCube;
  machine_area->width.setValue(8);
  machine_area->height.setValue(8);
  machine_area->depth.setValue(8);

  SoDrawStyle *style = new SoDrawStyle;
  style->style = SoDrawStyle::INVISIBLE;
  root->addChild(style);
  root->addChild(machine_area);

  // Set up viewer
  myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(root);
  
  myViewer->setTitle("Boolean Viewer");
  myViewer->viewAll();
  myViewer->show();

  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}

void 
drawTool(void *, SoAction *action)
{
  static float modelMatrix[16];
  static SbBool isDrawNewTool = FALSE;

  // No cache used
  SoCacheElement::invalidate(action->getState());
	
  if(sw->whichChild.getValue() == SO_SWITCH_ALL)
    return;

  if(action->getTypeId() != SoGLRenderAction::getClassTypeId())
    return;
	
  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |
               GL_TRANSFORM_BIT | GL_ENABLE_BIT | 
               GL_STENCIL_BUFFER_BIT);
		
  glDepthMask(GL_FALSE);
  glLogicOp(GL_XOR);
  glEnable(GL_COLOR_LOGIC_OP);
  glPushMatrix();
	
  if(!isDrawNewTool) {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelMatrix);
  }
  else {
    toolDrawSwitch->whichChild = SO_SWITCH_ALL;
    glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
  }
	
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilFunc(GL_EQUAL, 0, 1);
	
  isDrawNewTool = (isDrawNewTool ? FALSE : TRUE);
}


void 
endDrawTool(void *, SoAction *action)
{
  // No cache used
  SoCacheElement::invalidate(action->getState());

  if(sw->whichChild.getValue() == SO_SWITCH_ALL) 
    return;
  if (action->getTypeId() != SoGLRenderAction::getClassTypeId()) 
    return;

  glPopAttrib();
  glPopMatrix();
}

void 
onStartBoolean(void *, SoAction *action)
{
  if (action->getTypeId() != SoGLRenderAction::getClassTypeId()) 
    return;

  // Store OpenGL state
  glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | 
	       GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDisable(GL_CULL_FACE);

  // Z Buffer actions
  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LESS);

  // No drawing
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  // Stencil buffer 
  glStencilFunc(GL_ALWAYS, 0, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
  glEnable(GL_STENCIL_TEST);
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);

  // No cache used
  SoCacheElement::invalidate(action->getState());
}

void 
onMiddleBoolean(void *, SoAction *action)
{
  if (action->getTypeId() != SoGLRenderAction::getClassTypeId()) 
    return;

  // Z Buffer actions
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_GREATER);

  // Drawing
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  // Stencil buffer 
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 0, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  // No cache used
  SoCacheElement::invalidate(action->getState());
}

void 
onEndBoolean(void *, SoAction *action)
{
  if (action->getTypeId() != SoGLRenderAction::getClassTypeId()) 
    return;

  // Restore OpenGL state
  glPopAttrib();

  // No cache used
  SoCacheElement::invalidate(action->getState());
}

void
onkey(void *, SoEventCallback *eventcb)
{
  SoKeyboardEvent *event = (SoKeyboardEvent*)eventcb->getEvent();
  if (SO_KEY_RELEASE_EVENT(event, SPACE)) {
    switch (sw->whichChild.getValue()) {
    case SO_SWITCH_ALL:
      // Start the machining
      toolDrawSwitch->whichChild = SO_SWITCH_NONE;
      myViewer->setClearBeforeRender(FALSE, FALSE);
      sw->whichChild.setValue(1);
      break;

    case 1:
      // End the machining
      myViewer->setClearBeforeRender(TRUE, TRUE);
      sw->whichChild.setValue(SO_SWITCH_ALL);
      toolDrawSwitch->whichChild = SO_SWITCH_NONE;
      break;
    }
  }
}

