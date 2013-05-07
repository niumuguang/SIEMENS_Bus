#ifdef DVD_DEMO
#include <Inventor/lock/SoLockMgr.h>
#endif

#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h> 
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <DialogViz/SoDialogVizAll.h>
#include <Inventor/SoWinApp.h>
#include <Inventor/SoMacApp.h>

SoXtExaminerViewer *myViewer;
SoTopLevelDialog* myTop;
SoDialogLabel *labFPS, *labNOD, *lab, *lab2;
SoDialogComboBox* cb, *cb2;
SoDialogCheckBox* chkbEarth, *chkbSpaceStation;

SoSwitch* switch0;
SoSeparator* separator_Meteorite_Cloud, *separator_earth_spacestation;
SbString fpsStr = "Frame rate = ";
SoComplexity* sceneComplexity;

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

  countAction->apply(separator_earth_spacestation);
  numTris = countAction->getTriangleCount();
  str = "Num Triangles for FastEditing Scene: ";
  str += numTris;
  lab2->label = str;
}

class CHKB : public SoDialogCheckBoxAuditor
{
public:
  void dialogCheckBox(SoDialogCheckBox* chb) {
    if (chkbSpaceStation->state.getValue())
      switch0->whichChild = 0;
    if (chkbEarth->state.getValue())
      switch0->whichChild = 1;
    if (chkbSpaceStation->state.getValue() && chkbEarth->state.getValue())
      switch0->whichChild = -3;
    if (!chkbSpaceStation->state.getValue() && !chkbEarth->state.getValue())
      switch0->whichChild = -1;
    updateCount();
  }
};

class MyAuditorCB : public SoDialogChoiceAuditor
{
public:
  void dialogChoice(SoDialogChoice* pb) {
    switch(pb->selectedItem.getValue()) {
    case 0:
      myViewer->setFastEditSavePolicy(SoGLRenderAction::DISABLE);
      break;
    case 1:
      myViewer->setFastEditSavePolicy(SoGLRenderAction::EACH_FRAME);
      break;
    case 2:
      myViewer->setFastEditSavePolicy(SoGLRenderAction::WHEN_NEEDED);
      break;
    }
  }
};

class MyAuditorPB : public SoDialogChoiceAuditor
{
public:
  void dialogChoice(SoDialogChoice* pb) {
    switch(pb->selectedItem.getValue()) {
    case 0:
      separator_earth_spacestation->fastEditing = SoSeparator::DISABLE;
      break;
    case 1:
      separator_earth_spacestation->fastEditing = SoSeparator::KEEP_ZBUFFER;
      break;
    case 2:
      separator_earth_spacestation->fastEditing = SoSeparator::CLEAR_ZBUFFER;
      break;
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

SoSeparator *
readFile(const char *filename)
{
  // Open the input file
  SoInput mySceneInput;
  if (!mySceneInput.openFile(filename,TRUE)) {
//    fprintf(stderr, "Cannot open file %s\n", filename);
    return NULL;
  }
  
  // Read the whole file into the database
  SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
  if (myGraph == NULL) {
    fprintf(stderr, "Problem reading file\n");
    return NULL;
  } 
  
  mySceneInput.closeFile();
  return myGraph;
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

  // Uncomment the following line to disable the use of
  // buffer region with fastEdit.
  // See also SoGLRenderAction::setFastEditSavePolicy for complete description.
  // SoPreferences::setValue("OIV_BUFFER_REGION_ENABLE", "0");

  int i;
  SoDialogViz::init();

  SoSeparator *root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

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

  separator_earth_spacestation = new SoSeparator;
  separator_earth_spacestation->fastEditing = SoSeparator::KEEP_ZBUFFER;
  separator_earth_spacestation->addChild(sceneComplexity);

  switch0 = new SoSwitch;
  switch0->whichChild = 1;
  separator_earth_spacestation->addChild(switch0);

  SoSeparator* mySSSep = new SoSeparator;

  SoTransformerManip* manip1 = new SoTransformerManip;
  mySSSep->addChild(manip1);

  SoTranslation* mySSTrans = new SoTranslation;
  mySSTrans->translation.setValue(50 * NB / 1000,0,0);
  mySSSep->addChild(mySSTrans);
  SoBaseColor* ssColor = new SoBaseColor;
  ssColor->rgb.setValue(1,0,0);
  mySSSep->addChild(ssColor);
  SoSeparator* spaceStation;
  const char *oivhome = SoPreferences::getValue( "OIVHOME" );
  char buffer[512];
  strcpy( buffer, oivhome );
  strcat( buffer, "/data/models/vehicles/spacestation.iv" );
  spaceStation = readFile(buffer);
  if (spaceStation == NULL)
    spaceStation = readFile("../../../../../data/models/vehicles/spacestation.iv");
  if (spaceStation == NULL) {
    printf("Cannot find ../../../../../data/models/vehicles/spacestation.iv ; trying in local folder ...\n");
    spaceStation = readFile("./spacestation.iv");
  }
  if (spaceStation == NULL) {
    printf("Cannot locate spacestation.iv Inventor file.\n");
    exit(0);
  }

  mySSSep->addChild(spaceStation);
  switch0->addChild(mySSSep);

  SoSeparator *myEarthSep = new SoSeparator;
  SoTransformerManip* manip2 = new SoTransformerManip;
  myEarthSep->addChild(manip2);

  SoTranslation* myEarthTrans = new SoTranslation;
  myEarthTrans->translation.setValue(30 * NB / 1000,0,0);
  myEarthSep->addChild(myEarthTrans);
  SoTexture2  *myTexture2 = new SoTexture2;
  myTexture2->filename = "../../data/globe.rgb";
  myEarthSep->addChild(myTexture2);
  SoSphere* myEarth = new SoSphere;
  myEarth->radius = 6 * NB / 1000;
  myEarthSep->addChild(myEarth);
  switch0->addChild(myEarthSep);
  root->addChild(separator_earth_spacestation);

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
  myTop->label =  "FastEditing Demo" ;
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

  lab2 = new SoDialogLabel();
  lab2->label = "Num Triangles for FastEditing Scene: 0000000000";
  lab2->fixedWidth = TRUE;
  col2->addChild(lab2);

  chkbEarth = new SoDialogCheckBox();
  chkbEarth->offString = "Earth";
  chkbEarth->onString = "Earth";
  chkbEarth->fixedWidth = TRUE;
  chkbEarth->label = "Fast Editing Scene:";
  chkbEarth->addAuditor(new CHKB());
  chkbEarth->state = TRUE;
  col2->addChild(chkbEarth);

  chkbSpaceStation = new SoDialogCheckBox();
  chkbSpaceStation->offString = "Space Station";
  chkbSpaceStation->onString = "Space Station";
  chkbSpaceStation->fixedWidth = TRUE;
  chkbSpaceStation->addAuditor(new CHKB());
  col2->addChild(chkbSpaceStation);


  SoColumnDialog* col = new SoColumnDialog();
  col->fixedHeight = TRUE;
  myTop->addChild(col);

  cb = new SoDialogComboBox;
  cb->label = "SoGLRenderAction::FastEditSavePolicy: ";
  cb->items.set1Value(0, "DISABLE");
  cb->items.set1Value(1, "EACH_FRAME");
  cb->items.set1Value(2, "WHEN_NEEDED");
  cb->selectedItem = 2;
  cb->fixedWidth = TRUE;
  col->addChild(cb);
  cb->addAuditor(new MyAuditorCB);

  cb2 = new SoDialogComboBox;
  cb2->label = "SoSeparator::FastEditing: ";
  cb2->items.set1Value(0, "DISABLE");
  cb2->items.set1Value(1, "KEEP_ZBUFFER");
  cb2->items.set1Value(2, "CLEAR_ZBUFFER");
  cb2->selectedItem = 1;
  cb2->fixedWidth = TRUE;
  col->addChild(cb2);
  cb2->addAuditor(new MyAuditorPB);

  myTop->buildDialog(myWindow, TRUE);
  myTop->show();

  myViewer = new SoXtExaminerViewer(myCustom->getWidget());
  myViewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
  myViewer->setFastEditSavePolicy(SoGLRenderAction::WHEN_NEEDED);
  //myViewer->setFramesPerSecondCallback(fpsCallback, NULL);
  myViewer->setSceneGraph(root);
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
