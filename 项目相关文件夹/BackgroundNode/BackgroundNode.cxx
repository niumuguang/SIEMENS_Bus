// Example - Background nodes
//
// Copyright 2004 TGS
//
// Usage: backgroundNode <filename.iv>
//
// Notes:
//   Requires Open Inventor version 5.0 or higher.

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/SoXtColorEditor.h> 

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/SoInput.h>

#include <DialogViz/SoDialogVizAll.h>

// Headers for the background nodes
#include <Inventor/nodes/SoGradientBackground.h>
#include <Inventor/nodes/SoImageBackground.h>

#include <Inventor/SoMacApp.h>

// Global variables
static SoSwitch *pGradientSwitch = NULL;
static SoSwitch *pImgBackgSwitch = NULL;
static SoGradientBackground *pGradient = NULL;
static SoImageBackground *pImgBackg = NULL;
static SoXtColorEditor *pColEdit  = NULL;
static SoTopLevelDialog *myTopLevelDialog = NULL;

// Constants
const char *ivFilename = "./test.iv";             // model to display
const char *uiFilename = "./BackgroundNodeUI.iv"; // user interface

// This image demonstrates that partially transparent images are
// correctly rendered (other backgrounds show through)
const char *imgFilename = "./cypress.png";

/////////////////////////////////////////////////////////////
//
// Auditor class to handle events in our user interface

class testAuditor : public SoDialogAuditor
{
public:
  testAuditor();
  
private:
  void dialogCheckBox(SoDialogCheckBox* cpt);
  void dialogPushButton(SoDialogPushButton* cpt);
  void dialogComboBox(SoDialogComboBox* cpt);
  void dialogEditText(SoDialogEditText* cpt);
};

Widget buildInterface(Widget window);

/////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
  char *filename = (char *)ivFilename;

  if (argc > 1) {
    filename = argv[1];
  }

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Unable to open model file '%s'\n", filename);
    filename = NULL;
  }
  else {
    fclose(fp);
  }
  
  // Initialize Inventor
  Widget myWindow = SoXt::init(argv[0]);
  
  // Initialize DialogViz for user interface
  SoDialogViz::init();
  
  // Try to read the .iv file (if any)
  SoSeparator *pRoot = NULL;
  if (filename != NULL) {
    SoInput in;
    in.openFile(filename);
    pRoot = SoDB::readAll(&in);
    in.closeFile();
  }

  // If that failed, create our favorite geometry
  if (pRoot == NULL) {
    SoMaterial *pMatl = new SoMaterial;
    SoCone *pCone = new SoCone;

    pMatl->diffuseColor.setValue(1,0,0);

    pRoot = new SoSeparator;
    pRoot->addChild(pMatl);
    pRoot->addChild(pCone);
  }
  pRoot->ref();

  // Create background nodes
  //
  // Image background node will be traversed after gradient background
  // node to show how we can put an image on top of a gradient...
  // Initially the image will be displayed in the lower left corner.
  pGradient = new SoGradientBackground;
  pImgBackg = new SoImageBackground;
  pImgBackg->filename = imgFilename;
  pImgBackg->style = SoImageBackground::LOWER_LEFT;

  // Create some switch nodes so we can turn the backgrounds on and off
  pGradientSwitch = new SoSwitch;
  pImgBackgSwitch = new SoSwitch;
  pGradientSwitch->whichChild = 0;
  pImgBackgSwitch->whichChild = 0;
  pGradientSwitch->addChild(pGradient);
  pImgBackgSwitch->addChild(pImgBackg);
  SoSeparator *pBkgSep = new SoSeparator;
  pBkgSep->addChild(pGradientSwitch);
  pBkgSep->addChild(pImgBackgSwitch);

  // Insert the background nodes at the top of the scene graph
  pRoot->insertChild(pBkgSep, 0);

  // Create a viewer
  Widget parent = buildInterface(myWindow);
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(parent);

  // attach and show viewer
  myViewer->setBackgroundColor(SbColor(0,.65f,.65f));
  myViewer->setSceneGraph(pRoot);
  myViewer->setTitle("Background Test");
  myViewer->show();

  // Loop forever
  SoXt::show(myWindow);
  SoXt::mainLoop();

  pRoot->unref();
  delete myViewer;

  return 0;
}

/////////////////////////////////////////////////////////////
//
// Load simple interface to experiment with background options

Widget
buildInterface(Widget window)
{
  SoInput myInput;
  if (! myInput.openFile(uiFilename)) {
    printf("Unable to open user interface file '%s'\n", uiFilename);
    return window;
  }
  
  SoGroup *myDialogGroup = SoDB::readAll(&myInput);
  if (! myDialogGroup) {
    return window;
  }
  myTopLevelDialog = (SoTopLevelDialog *)myDialogGroup->getChild(0);
  
  testAuditor *myAuditor = new testAuditor;
  myTopLevelDialog->addAuditor(myAuditor);
  
  SoDialogCustom *customNode = (SoDialogCustom *)
    myTopLevelDialog->searchForAuditorId(SbString("Viewer"));
  
  SoDialogCheckBox *checkbox = (SoDialogCheckBox *)
    myTopLevelDialog->searchForAuditorId(SbString("swapColors"));
  if (checkbox) {
    pGradient->swapColors.connectFrom(&(checkbox->state));
  }
  
  SoDialogEditText *editText = (SoDialogEditText *)
    myTopLevelDialog->searchForAuditorId(SbString("filename"));
  if (editText) {
    editText->editText = imgFilename;
  }
  
  myTopLevelDialog->buildDialog(window, customNode != NULL);
  myTopLevelDialog->show();
  
  return customNode ? customNode->getWidget() : window;
}

///////////////////////////////////////////////////////////////////////

testAuditor::testAuditor()
{
}

/*----------------------------- dialogCheckBox -----------------------------------------*/
void
testAuditor::dialogCheckBox(SoDialogCheckBox* cpt)
{
  SbBool state = cpt->state.getValue();

  if (cpt->auditorID.getValue() == "gradientVisible") {
    pGradientSwitch->whichChild = state ? 0 : -1;
  }

  if (cpt->auditorID.getValue() == "imageVisible") {
    pImgBackgSwitch->whichChild = state ? 0 : -1;
  }
}

/*----------------------------- fileSelection ---------------------------------------*/

#include <Inventor/Xt/SoXtFileSelectionDialog.h> 

static SoXtFileSelectionDialog *pFileDialog = NULL;

void
fileSelectionDialogCB(void *data, SoXtFileSelectionDialog *dialog)
{
  char *filename = dialog->getFileName();
  pImgBackg->filename = filename;

  SoDialogEditText *editText =
    (SoDialogEditText *)myTopLevelDialog->searchForAuditorId(SbString("filename"));
  if (editText) {
    editText->editText = filename;
  }
}

/*----------------------------- dialogPushButton ---------------------------------------*/

void
testAuditor::dialogPushButton(SoDialogPushButton *cpt)
{
  if (cpt->auditorID.getValue() == "color0") {
    if (pColEdit == NULL) {
      pColEdit = new SoXtColorEditor;
      pColEdit->setCurrentSliders(SoXtColorEditor::RGB_V);
    }
    pColEdit->attach(&(pGradient->color0), pGradient);
    pColEdit->show();
  }
  if (cpt->auditorID.getValue() == "color1") {
    if (pColEdit == NULL) {
      pColEdit = new SoXtColorEditor;
      pColEdit->setCurrentSliders(SoXtColorEditor::RGB_V);
    }
    pColEdit->attach(&(pGradient->color1), pGradient);
    pColEdit->show();
  }
  if (cpt->auditorID.getValue() == "browse") {
    if (pFileDialog == NULL) {
      pFileDialog = new SoXtFileSelectionDialog;
      pFileDialog->setTitle((char*)"Image file");
    }
    pFileDialog->setAcceptCallback(fileSelectionDialogCB, NULL);
    pFileDialog->show();
  }
}

/*----------------------------- dialogComboBox ---------------------------------------*/

void
testAuditor::dialogComboBox(SoDialogComboBox *cpt)
{
  if (cpt->auditorID.getValue() == "style") {
    int choice = cpt->selectedItem.getValue();
    switch (choice) {
    case 0:
      pImgBackg->style = SoImageBackground::NONE;
      break;
    case 1:
      pImgBackg->style = SoImageBackground::CENTER;
      break;
    case 2:
      pImgBackg->style = SoImageBackground::LOWER_LEFT;
      break;
    case 3:
      pImgBackg->style = SoImageBackground::UPPER_LEFT;
      break;
    case 4:
      pImgBackg->style = SoImageBackground::UPPER_RIGHT;
      break;
    case 5:
      pImgBackg->style = SoImageBackground::LOWER_RIGHT;
      break;
    case 6:
      pImgBackg->style = SoImageBackground::STRETCH;
      break;
    case 7:
      pImgBackg->style = SoImageBackground::TILE;
      break;
    }
  }
}

/*----------------------------- dialogEditText ---------------------------------------*/

void
testAuditor::dialogEditText(SoDialogEditText *cpt)
{
  if (cpt->auditorID.getValue() == "filename") {
    SbString filename = cpt->editText.getValue();
    pImgBackg->filename = filename;
  }
}
