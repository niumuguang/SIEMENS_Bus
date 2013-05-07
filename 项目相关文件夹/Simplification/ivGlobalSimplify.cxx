/*****************************************************************-*-c++-*-*****
 * File : ivglobalsimplify.C
 * Author(s) : Pascal DOUX
 * Date : 02/Jan/98
 * Revision : 1.0
 *==============================================================================
 * (c) Copyright 1998 TGS-Europe, all rights reserved
 *==============================================================================
 *
 * Purpose : This program demonstrates how to use the SoGlobalSimplifyAction
 *           to create simplified versions of a given scene graph.
 *
 ******************************************************************************/

#include <Inventor/Xt/SoXt.h>
#include <Inventor/SoDB.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/actions/SoWriteAction.h>

#include <Inventor/actions/SoGlobalSimplifyAction.h>
#include <Inventor/simplifier/SoDecimator.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/events/SoKeyboardEvent.h>


SoXtExaminerViewer *mySimplifiedViewer;
int numLevels = 4;
static float levels[5] = { 1.0f, 0.6f, 0.3f, 0.1f };

typedef struct cbdata {
  SoXtExaminerViewer *vwr;
  SoNode *root;
  int maxLevels;
} callbackData;

void 
saveToFile(SoNode *root)
{
  SoOutput output;

  printf("storing the simplified scene graph in simplified.iv...\n");
  output.openFile("simplified.iv");
  SoWriteAction wa(&output);
  wa.apply(root);
  output.closeFile();
}

static void
processKeyEvents(void *data, SoEventCallback *cb)
{
  SoKeyboardEvent *keyEvent = (SoKeyboardEvent *) cb->getEvent();
  SoKeyboardEvent::Key key = keyEvent->getKey();

  if (SoKeyboardEvent::isKeyPressEvent(cb->getEvent(), key)) 
    return;

  callbackData *cbData = (callbackData *)data;

  if (key == SoKeyboardEvent::L) { 
    printf("Level Of Simplification used for switching levels\n");
    mySimplifiedViewer->setDecimationStrategy(SoXtViewer::NORMAL);
  } 
  else {
    int whichChild = (int)key - (int) SoKeyboardEvent::NUMBER_1;
    
    if (0 <= whichChild && whichChild < cbData->maxLevels) {
      printf("changing to level %d percentage=%f\n", whichChild+1, levels[whichChild]);
      mySimplifiedViewer->setDecimationStrategy(SoXtViewer::FIXED_PERCENTAGE);
      mySimplifiedViewer->setFixedPercentage(levels[whichChild]);
    } 
  }
  cb->setHandled(); 
}

void
myInlineCallback(const SbString &url, void *, SoWWWInline *node)
{
  // Open the input file
  SoInput mySceneInput;
  
  printf("opening url %s\n", url.getString());
  if (!mySceneInput.openFile(url.getString())) {
    fprintf(stderr, "Cannot open file %s\n", url.getString());
    return;
  }
  
  // Read the whole file into the database
  SoSeparator *myGraph = SoDB::readAll(&mySceneInput);

  if (myGraph == NULL) {
    fprintf(stderr, "Problem reading file\n");
    return;
  }
  myGraph->renderCulling = SoSeparator::ON;
  node->setChildData(myGraph);
  mySceneInput.closeFile();
}

int
main(int argc, char **argv)
{
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL) 
    exit(1);

  if (argc != 2) {
    printf("Usage: %s file_name \n", argv[0]);
    exit(-1);
  }

  SoInput input;
  if (! input.openFile(argv[1])) {
    printf("Cannot open file %s \n", argv[1]);
    exit (-1);
  }
   
  SoWWWInline::setFetchURLCallBack(myInlineCallback, (void *)NULL);
  printf("reading %s...\n", argv[1]);
  SoSeparator *root = SoDB::readAll(&input);
  root->ref();

  // The Simplifier object: uses geometric simplification
  SoDecimator *decimer = new SoDecimator;

  // The simplification action: uses the global simplification.
  SoGlobalSimplifyAction simplify(decimer);

  simplify.setSimplificationLevels(numLevels, levels);
  simplify.setSizeFactor(4.);
  simplify.setUrlName("new");
  simplify.setMinTriangles(4);
  simplify.addShapeHintsNode(TRUE);

  printf("simplification started...\n");
  simplify.apply(root);
  printf("simplification done\n");
   
  root->unref();

  SoGroup *newRoot = new SoSeparator;
  newRoot->ref();
  newRoot->addChild(simplify.getSimplifiedSceneGraph());
  saveToFile(newRoot);

  SoEventCallback *eventCB = new SoEventCallback;
  newRoot->addChild(eventCB);

  printf("displaying the simplified scene graph...\n");
  // Open a viewer for the simplified scene:
  mySimplifiedViewer = new SoXtExaminerViewer(myWindow);
  mySimplifiedViewer->setSceneGraph(newRoot);
  mySimplifiedViewer->setTitle("Simplified Scene");
  mySimplifiedViewer->setViewing(FALSE);

  mySimplifiedViewer->show();

  printf("\n Enter Esc to switch between viewing and picking mode \n");
  printf("\n Enter a number between 0 and %d to see the corresponding \n"
         "level of simplification\n", numLevels);
  printf("\n Enter l to switch to the Level Of Detail switching method \n"
         "based on distance to the viewer\n\n");
  
  // Setup the event callback data and routine for switching levels
  callbackData *data = new callbackData;
   
  data->vwr = mySimplifiedViewer;
  data->root = newRoot;
  data->maxLevels = numLevels;
  eventCB->addEventCallback(SoKeyboardEvent::getClassTypeId(), 
                            processKeyEvents, data);
  SoXt::show(myWindow);
  SoXt::mainLoop();

  newRoot->unref();
  delete mySimplifiedViewer;
  SoXt::finish();

  return 0;
}



