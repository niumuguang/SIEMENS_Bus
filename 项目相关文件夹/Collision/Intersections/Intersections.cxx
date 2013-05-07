/*==============================================================================
 * File: Intersections.cxx
 * Author(s) : Fabien ARNAUD
 * Date: 15/04/97
 * Revision: 1.0
 *==============================================================================
 * (c) Copyright 1997 G5G, all rights reserved
 *==============================================================================
 *
 * Purpose : This program shows how to use the SoIntersectionDetectionAction
 *           class. It allows the user to manipulate a model in a scene, and by
 *           using the space bar, to know which shapes intersect together.
 *
 *============================================================================*/

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/collision/SoIntersectionDetectionAction.h>

#define MAC_CONSOLE
#include <Inventor/SoMacApp.h>

static void 
Output(const char *string)
{
  printf("%s", string);
#if defined(_WIN32)
  OutputDebugString(string);
#endif // _WIN32
}

static void
usage() {
  Output("usage: Intersections\n");
  Output("commands: in PICKING mode, the following commands apply\n");
  Output("\t<SPACE> show objects intersections in the scene root\n\n");
}

//----------------------------------------------------------------------------
SoSelection* root;
SoPath *xfOfSelectionPath;
SoTransform *xfOfSelection;
SoHandleBoxManip *myHandleBoxManip;
int intersectionCount;

//----------------------------------------------------------------------------
void
setHandleBoxManip(void)
{
  if (myHandleBoxManip==NULL) {
    myHandleBoxManip=new SoHandleBoxManip;
    myHandleBoxManip->ref();	
  }

  if (xfOfSelectionPath!=NULL)
    myHandleBoxManip->replaceNode(xfOfSelectionPath);
}

//----------------------------------------------------------------------------
void
removeHandleBoxManip(void)
{
  if (xfOfSelectionPath!=NULL && xfOfSelection!=NULL) 
    myHandleBoxManip->replaceManip(xfOfSelectionPath, xfOfSelection);
}

//----------------------------------------------------------------------------
SbBool
isTransformable(SoNode *myNode)
{
  if (myNode->isOfType(SoGroup::getClassTypeId()) ||
      myNode->isOfType(SoShape::getClassTypeId()) ||
      myNode->isOfType(SoCamera::getClassTypeId()) ||
      myNode->isOfType(SoLight::getClassTypeId()))
    return TRUE;
  else 
    return FALSE;
}

//----------------------------------------------------------------------------
SoPath *
createTransformPath(SoPath *inputPath)
{
  int pathLength = inputPath->getLength();
  if (pathLength < 2)
    return NULL;

  SoNode *tail = inputPath->getTail();

  // CASE 1: The tail is a node kit.
  if (tail->isOfType(SoBaseKit::getClassTypeId())) {
    SoBaseKit *kit = (SoBaseKit *) tail;
    return kit->createPathToPart("transform",TRUE,inputPath);
  }

  SoTransform *editXf = NULL;
  SoGroup *parent;

  // CASE 2: The tail is not a group.
  SbBool isTailGroup;
  isTailGroup = tail->isOfType(SoGroup::getClassTypeId());
  if (!isTailGroup) {
    parent = (SoGroup *) inputPath->getNode(pathLength - 2);
    int tailIndx = parent->findChild(tail);

    for (int i = tailIndx; (i >= 0) && (editXf == NULL);i--) {
      SoNode *myNode = parent->getChild(i);
      if (myNode->isOfType(SoTransform::getClassTypeId()))
	editXf = (SoTransform *) myNode;
      else if (i != tailIndx && (isTransformable(myNode)))
	break;
    }

    if (editXf == NULL) {
      editXf = new SoTransform;
      parent->insertChild(editXf, tailIndx);
    }
  }

  // CASE 3: The tail is a group.
  else {
    parent = (SoGroup *) tail;
    int i;
    for (i = 0; (i < parent->getNumChildren()) && (editXf == NULL); i++) {
      SoNode *myNode = parent->getChild(i);
      if (myNode->isOfType(SoTransform::getClassTypeId()))
	editXf = (SoTransform *) myNode;
      else if (isTransformable(myNode))
	break;
    }
    if (editXf == NULL) {
      editXf = new SoTransform;
      parent->insertChild(editXf, i);
    } 
  }

  SoPath *pathToXform = NULL;
  pathToXform = inputPath->copy();
  pathToXform->ref();
  if (!isTailGroup) // pop off the last entry.
    pathToXform->pop();

  int xfIndex   = parent->findChild(editXf);
  pathToXform->append(xfIndex);
  pathToXform->unrefNoDelete();

  return(pathToXform);
}

//----------------------------------------------------------------------------
SoIntersectionDetectionAction::Resp
onIntersection(void *,
               const SoIntersectingPrimitive *primitive1,
               const SoIntersectingPrimitive *primitive2)
{
  printf("%d   %s (%s) *** %s (%s)\n",
	 intersectionCount,
	 (char *)primitive1->path->getTail()->getName().getString(),
	 (char *)primitive1->path->getTail()->getTypeId().getName().getString(),
	 (char *)primitive2->path->getTail()->getName().getString(),
	 (char *)primitive2->path->getTail()->getTypeId().getName().getString());
  intersectionCount++;
  return SoIntersectionDetectionAction::NEXT_SHAPE;
}

//----------------------------------------------------------------------------
void
onKeyPress (void *, SoEventCallback *eventCB)
{
  if (SO_KEY_PRESS_EVENT(eventCB->getEvent(), SPACE)) {
    printf ("List of collisions :\n");
    removeHandleBoxManip();
    intersectionCount=0;
    SoIntersectionDetectionAction action;
#ifdef __osf__
    action.addIntersectionCallback((SoIntersectionDetectionAction::SoIntersectionCB *)onIntersection, NULL);
#else
    action.addIntersectionCallback(onIntersection, NULL);
#endif
    action.apply(root);
    setHandleBoxManip();
  }
}

//----------------------------------------------------------------------------
void
onSelection(void *, SoPath *path)
{
  if (!(path->getTail()->getTypeId().isDerivedFrom
	(SoShape::getClassTypeId())))
    return;

  xfOfSelectionPath=createTransformPath(path);
  xfOfSelectionPath->ref();

  xfOfSelection=(SoTransform *)xfOfSelectionPath->getTail();
  xfOfSelection->ref();

  setHandleBoxManip();
}

//----------------------------------------------------------------------------
void
onDeselection (void *, SoPath *path)
{
  if (!(path->getTail()->getTypeId().isDerivedFrom(SoShape::getClassTypeId())))
    return;

  removeHandleBoxManip();

  if (xfOfSelectionPath!=NULL)
    xfOfSelectionPath->unref();

  if (xfOfSelection!=NULL)
    xfOfSelection->unref();

  xfOfSelectionPath=NULL;
  xfOfSelection=NULL;
}

//----------------------------------------------------------------------------
int
main(int argc, char ** argv)
{
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL) {
    fprintf(stderr, "Cannot initialize the window\n");
    exit(-1);
  }

  char filename [1024];
  if (argc != 2) {
    strcpy(filename, "jumpyMan.iv");
  } 
  else {
    strcpy(filename, argv[1]);
  }

  root = new SoSelection;
  root->ref();
  root->policy = SoSelection::SINGLE;
  root->addSelectionCallback (onSelection, NULL);
  root->addDeselectionCallback(onDeselection, NULL);

  SoInput myInput;
  if (!myInput.openFile(filename))
    exit(-1);
  SoSeparator *fileContent = SoDB::readAll(&myInput);
  if (fileContent == NULL)
    exit(-1);
			
  SoEventCallback *keyboardEvent = new SoEventCallback;
  keyboardEvent->addEventCallback(SoKeyboardEvent::getClassTypeId(), onKeyPress);

  root->addChild(keyboardEvent);
  root->addChild(fileContent);

  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Intersections");
  myViewer->viewAll();

  usage();

  myViewer->show();
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}

