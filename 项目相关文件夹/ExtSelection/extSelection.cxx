//
// This sample program shows how to use the new SoExtSelection node. This node performs
// lasso or rectangle selection.
//
//------------------------------------------------------------------------------
// Include files

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/VRMLnodes/SoVRMLInline.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>

#include <Inventor/dialog/PoXtDialog.h>
#include <Inventor/dialog/PoXtElementData.h>
#include <Inventor/dialog/PoXtChoiceButtonData.h>
//------------------------------------------------------------------------------
// Dialog master constants
#define D_NUM_CONTROLS 4
#define D_SELECTION_POLICY 0
#define D_LASSO_POLICY     1
#define D_LASSO_TYPE       2
#define D_LASSO_MODE       3
//------------------------------------------------------------------------------
// Prototypes
void onControlChange(void*, PoXtDialog::PoXtDialogDataCB *data);

//------------------------------------------------------------------------------
// Globals
SoXtExaminerViewer *viewer;
SoExtSelection *root;

//******************************************************************************
// Entry point of the application
int
main(int argc, char **argv)
{
  // Initialize Inventor and Xt ------------------------------------------------
  Widget myWindow = SoXt::init(argv[0]);
  if(myWindow == NULL) 
    exit(1);

  // Selection -----------------------------------------------------------------
  root = new SoExtSelection;
  root->ref();
  root->setLassoColor(SbColor(0, 1, 0));
  root->setOverlayLassoPattern(0xE0E0);

  root->animateOverlayLasso(TRUE);
  root->policy = SoSelection::SINGLE;
  root->lassoType = SoExtSelection::RECTANGLE;
  root->lassoPolicy = SoExtSelection::FULL_BBOX;
  root->lassoMode = SoExtSelection::ALL_SHAPES;

  // Scene graph ---------------------------------------------------------------
  if (argc != 2) {
    // build test scene
    SoTransform *t = new SoTransform;
    SoMaterial *m = new SoMaterial;

    t->translation.setValue(1.5, 1.5, 0);
    m->diffuseColor.setValue(1, 0, 0);
    root->addChild(t);
    root->addChild(m);
    root->addChild(new SoSphere);

    t = new SoTransform;
    t->translation.setValue(-3, 0, 0);
    m = new SoMaterial;
    m->diffuseColor.setValue(0, 1, 0);
    root->addChild(t);
    root->addChild(m);
    root->addChild(new SoCone);

    t = new SoTransform;
    t->translation.setValue(0, -3, 0);
    m = new SoMaterial;
    m->diffuseColor.setValue(0, 0, 1);
    root->addChild(t);
    root->addChild(m);
    root->addChild(new SoCylinder);

    t = new SoTransform;
    t->translation.setValue(3, 0, 0);
    m = new SoMaterial;
    m->diffuseColor.setValue(1, 1, 0);
    root->addChild(t);
    root->addChild(m);
    root->addChild(new SoCube);
  }
  else {
    SoInput myInput;
    if (!myInput.openFile(argv[1]))
      exit(0);
    SoWWWInline::setReadAsSoFile(TRUE);
    SoVRMLInline::setReadAsSoFile(TRUE);

    SoSeparator *fileContent = SoDB::readAll(&myInput);
    if (fileContent == NULL)
      exit(0);
    root->addChild(fileContent);
  }

  // Viewer initialisation -----------------------------------------------------
  viewer = new SoXtExaminerViewer(myWindow);
  viewer->setSceneGraph(root);
  SbColor color(0, 1, 0);
  viewer->setOverlayColorMap(1, 1, &color);
  viewer->setGLRenderAction(new SoBoxHighlightRenderAction());
  viewer->redrawOnSelectionChange(root);
  viewer->setTitle("Selection Test");
  viewer->setViewing(FALSE);
  viewer->show();

  // Setup the control dialog --------------------------------------------------
  PoXtElementData *tab[D_NUM_CONTROLS];
  int ind = 0;
 
  const char *selection_policy_items[3] = {"Single", "Toggle", "Shift"};
  tab[ind] = new PoXtChoiceButtonData("Selection policy", 3, selection_policy_items);
  tab[ind]->setID(D_SELECTION_POLICY);
  ((PoXtChoiceButtonData *)tab[ind])->setSelectedItem(root->policy.getValue());
  ind++;
 
  const char *lasso_type_items[4] = {"No Lasso", "Lasso", "Rectangle", "Ellipse"};
  tab[ind] = new PoXtChoiceButtonData("Lasso type", 4, lasso_type_items, 2);
  tab[ind]->setID(D_LASSO_TYPE);
  ((PoXtChoiceButtonData *)tab[ind])->
    setSelectedItem(root->lassoType.getValue());
  ind ++;
 
  const char *lasso_policy_items[4] = {
    "Full Bounding Box", 
    "Part Bounding Box", 
    "Full", 
    "Part"
  };
  tab[ind] = new PoXtChoiceButtonData("Lasso policy", 4, lasso_policy_items);
  tab[ind]->setID(D_LASSO_POLICY);
  ((PoXtChoiceButtonData *)tab[ind])->setSelectedItem(root->lassoPolicy.getValue());
  ind ++;

  const char *lasso_mode_items[2] = {
    "Visible Shapes", 
    "All Shapes"
  };
  tab[ind] = new PoXtChoiceButtonData("Lasso mode", 2, lasso_mode_items, 1);
  tab[ind]->setID(D_LASSO_MODE);
  ((PoXtChoiceButtonData *)tab[ind])->
    setSelectedItem(root->lassoMode.getValue());
  ind ++;

  PoXtDialog *control_dlg = new PoXtDialog(NULL, "Extended Selection Control", 
					   FALSE, D_NUM_CONTROLS, 
					   (const PoXtElementData **)tab);
  control_dlg->addValueChangedCallback((PoXtDialog::PoXtDialogCB *)onControlChange, NULL);
  control_dlg->show();

  // Run the application -------------------------------------------------------
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete control_dlg;
  delete viewer;
  SoXt::finish();

  return 0;
}

//******************************************************************************
// Function called when there is a change in the control box
void
onControlChange(void *, PoXtDialog::PoXtDialogDataCB *data)
{
  const PoXtElementData *element = data->dialog->get1Value(data->widgetNumber);

  switch (element->getID()) {
  case D_SELECTION_POLICY: //---------------------------------------------------
    switch (((PoXtChoiceButtonData *)element)->getSelectedItem()) {
      // Single policy
    default:
    case 0:
      root->policy = SoSelection::SINGLE;
      break;
      // Toggle policy
    case 1:
      root->policy = SoSelection::TOGGLE;
      break;
      // Shift policy
    case 2:
      root->policy = SoSelection::SHIFT;
      break;
    }
    break;
 
  case D_LASSO_TYPE: //---------------------------------------------------------
    switch (((PoXtChoiceButtonData*) element)->getSelectedItem()) {
      // No Lasso
    default:
    case 0:
      root->lassoType = SoExtSelection::NOLASSO;
      break;
      // Lasso
    case 1:
      root->lassoType = SoExtSelection::LASSO;
      break;
      // Rectangle
    case 2:
      root->lassoType = SoExtSelection::RECTANGLE;
      break;
    case 3:
      root->lassoType = SoExtSelection::ELLIPSE;
      break;

    }
    break;
 
  case D_LASSO_POLICY: //-------------------------------------------------------
    switch (((PoXtChoiceButtonData*) element)->getSelectedItem()) {
      // Full Bounding Box
    default:
    case 0:
      root->lassoPolicy = SoExtSelection::FULL_BBOX;
      break;
      // Part bounding box
    case 1:
      root->lassoPolicy = SoExtSelection::PART_BBOX;
      break;
      // Full
    case 2:
      root->lassoPolicy = SoExtSelection::FULL;
      break;
      // Part
    case 3:
      root->lassoPolicy = SoExtSelection::PART;
      break;
    }
    break;
 
  case D_LASSO_MODE: //-------------------------------------------------------
    switch (((PoXtChoiceButtonData*) element)->getSelectedItem()) {

      // Visible shapes
    case 0:
      root->lassoMode = SoExtSelection::VISIBLE_SHAPES;
      break;

      // All shapes
    default:
    case 1:
      root->lassoMode = SoExtSelection::ALL_SHAPES;
      break;
    }
    break;

  default:;
  }
}
