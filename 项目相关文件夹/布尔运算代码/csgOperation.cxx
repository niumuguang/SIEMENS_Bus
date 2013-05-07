///////////////////////////////////////////////////////////////////////
// 
// TGS sample for SoCGSOperation in Open Inventor
//
//
// Copyright (C) 2001, TGS
// All Rights Reserved.
//
// Permission is granted for licensed customers of the TGS
// OpenInventor library to use this source for any reasonable
// purpose that involves the TGS OpenInventor library.
//
// Please note this is a *demo* program, not an example program.
// It is intended to demonstrate some of the features of the TGS
// OpenInventor library.  It is not necessarily an example of
// good programming style. :-)
//
// Original: March-2001     Rodolphe ALBOU.
//
///////////////////////////////////////////////////////////////////////

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <SolidViz/SoSolidViz.h>
#include <SolidViz/SoCSGOperation.h>

SoSeparator *mainRoot;

static void 
eventCB(void *data, SoEventCallback *ev)
{
  SoCSGOperation *CSGOP = (SoCSGOperation *)data;

	SbBool isOp = FALSE;

	if (SO_KEY_PRESS_EVENT(ev->getEvent(), O)) { // OR
		printf("Cube OR Sphere\n");
		isOp = TRUE;
		CSGOP->setOperationType(SoCSGOperation::OR);
	}
	else if (SO_KEY_PRESS_EVENT(ev->getEvent(), A)) { // AND
		printf("Cube AND Sphere\n");
		isOp = TRUE;
		CSGOP->setOperationType(SoCSGOperation::AND);
	}
	else if (SO_KEY_PRESS_EVENT(ev->getEvent(), S)) { // SUB
		printf("Cube SUB Sphere\n");
		isOp = TRUE;
		CSGOP->setOperationType(SoCSGOperation::SUB);
	}
	else if (SO_KEY_PRESS_EVENT(ev->getEvent(), C)) { // CUT
		printf("Cube CUT Sphere\n");
		isOp = TRUE;
		CSGOP->setOperationType(SoCSGOperation::CUT);
	}
	else if (SO_KEY_PRESS_EVENT(ev->getEvent(), M)) { // MERGE
		printf("Cube MERGE Sphere\n");
		isOp = TRUE;
		CSGOP->setOperationType(SoCSGOperation::MERGE);
	}

	if (isOp) {
		SoSeparator *oldResult = (SoSeparator *) SoNode::getByName("CSGResult");
		SoSeparator *result = CSGOP->getSceneGraph();

		if (result) {
			result->setName("CSGResult");
      if (oldResult) {
				mainRoot->replaceChild(oldResult, result);
      }
      else {
				mainRoot->addChild(result);
      }
		}
	}
}

void
usage()
{
	printf("CSG Vector sample\n");
	printf("Press the key :\n");
	printf("\tO for OR operation.\n");
	printf("\tA for AND operation.\n");
	printf("\tS for SUB operation.\n");
	printf("\tC for CUT operation.\n");
	printf("\tM for MERGE operation.\n");
}

int
main(int, char **argv)
{
	// Initialize Inventor, Xt and SolidViz
 	Widget myWindow = SoXt::init(argv[0]);
 	SoSolidViz::init();

   usage();
 
   if (myWindow == NULL) {
     return 1;
   }
 	mainRoot = new SoSeparator;
 	mainRoot->ref();
 	
   // the CSG operation
   SoCSGOperation CSGOP;
 
 	// Event Callback to get expressions
 	SoEventCallback * myEvent = new SoEventCallback();
 	myEvent->addEventCallback(SoKeyboardEvent::getClassTypeId(), eventCB, &CSGOP);
 	mainRoot->addChild(myEvent);
 
 	// Object A
 	SoSeparator *objectA = new SoSeparator;
 	objectA->addChild(new SoCube);
 	
 	// Object B
 	SoSeparator *objectB = new SoSeparator();
 	SoSphere *sphere = new SoSphere;
 	SoTranslation *translation = new SoTranslation;
 	translation->translation.setValue(0.4f, 0.0f, 0.0f);
 	sphere->radius = 1.2f;
 	objectB->addChild(translation);
 	objectB->addChild(sphere);

	 //Sets the 2 object for the CSG operation
 	CSGOP.setSceneGraphs(objectA, objectB);
 	
 	// Computes and retreives the result ObjectA <Op> ObjectB
 	SoSeparator *result = CSGOP.getSceneGraph();
 	if (result) {
 		result->setName("CSGResult");
 		mainRoot->addChild(result);
 	}
	
 	SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
 	myViewer->setSceneGraph(mainRoot);
 	myViewer->setViewing(FALSE);
 
   // Stencil buffer support is highly recommended (needed)
   SoGLGraphicConfigTemplate gTemplate;
   gTemplate.setStencil((SoGraphicConfigTemplate::Preference)8);
   myViewer->setGraphicConfigTemplate(&gTemplate);
	
   myViewer->viewAll();
 	myViewer->setTitle("Constructive Solid Geometry");
 	myViewer->show();
 
 	SoXt::show(myWindow);
 	SoXt::mainLoop();

	return 0;
}
