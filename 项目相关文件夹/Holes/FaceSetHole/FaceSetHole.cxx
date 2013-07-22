///////////////////////////////////////////////////////////////////////
// 
// TGS Open Inventor example for hole in an FaceSet
//
// Copyright (C) 2003, TGS
// All Rights Reserved.
//
// Permission is granted for licensed customers of the Mercury
// Open Inventor library to use this source for any reasonable
// purpose that involves the Mercury Open Inventor library.
//
///////////////////////////////////////////////////////////////////////

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture2.h> 
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoEventCallback.h>

#define MAC_CONSOLE
#include <Inventor/SoMacApp.h>
//////////////////////////////////////////////////////////////

static const float 
vertices[12][3] = {
  {   0,     0, 0},   
  {   1,    -1, 0},  
  {   1,     1, 0},   

  {0.8f,     0, 0},  
  {0.3f, -0.2f, 0}, 
  {0.3f,  0.2f, 0}, 

  {   0,     0, 0},   
  {   0,    -1, 0},  
  {   1,    -1, 0},  

  {0.6f,     0, 0},   
  {0.4f, -0.9f, 0}, 
  {0.1f, -0.5f, 0}  
};

// Number of vertices in each polygon:
static int32_t numvertices[4] = { 3, 3, 3, 3 };

SoShapeHints *m_hints;

void
myKeyPressCB(void *userData, SoEventCallback *eventCB)
{
  const SoEvent *event = eventCB->getEvent();
  
  // check for the Up and Down arrow keys being pressed
  if (SO_KEY_PRESS_EVENT(event, W)) {
    if (m_hints->windingType.getValue() == SoShapeHints::ODD_TYPE) {
      m_hints->windingType=SoShapeHints::NON_ZERO_TYPE;
      printf("SoShapeHints::NON_ZERO_TYPE\n");
    } else if (m_hints->windingType.getValue() == SoShapeHints::NON_ZERO_TYPE) {
      m_hints->windingType = SoShapeHints::POSITIVE_TYPE;
      printf("SoShapeHints::POSITIVE_TYPE\n");
    } else if (m_hints->windingType.getValue() == SoShapeHints::POSITIVE_TYPE) {
      m_hints->windingType = SoShapeHints::NEGATIVE_TYPE;
      printf("SoShapeHints::NEGATIVE_TYPE;\n");
    } else if (m_hints->windingType.getValue() == SoShapeHints::NEGATIVE_TYPE) {
      m_hints->windingType = SoShapeHints::NO_WINDING_TYPE;
      printf("SoShapeHints::NO_WINDING_TYPE\n");
    }else if (m_hints->windingType.getValue() == SoShapeHints::NO_WINDING_TYPE) {
      m_hints->windingType = SoShapeHints::ABS_GEQ_TWO_TYPE;
      printf("SoShapeHints::ABS_GEQ_TWO_TYPE\n");
    } else if (m_hints->windingType.getValue() == SoShapeHints::ABS_GEQ_TWO_TYPE) {
      m_hints->windingType = SoShapeHints::ODD_TYPE;
      printf("SoShapeHints::ODD_TYPE\n");
    }
  }
}

SoSeparator *
makeObjectFaceSet()
{
  SoSeparator *result = new SoSeparator();
  result->ref();
  m_hints = new SoShapeHints;
  result->addChild(m_hints);
  m_hints->windingType=SoShapeHints::ODD_TYPE;
  m_hints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;

  printf("Press 'W' (in selection mode) to change winding rule\n");
  printf("SoShapeHints::ODD_TYPE\n");

  SoVertexProperty *myVertexProperty = new SoVertexProperty;

  // Define material
  myVertexProperty->orderedRGBA.setValue(SbColor(0.4f, 0, 0).getPackedValue());
  myVertexProperty->normalBinding = SoVertexProperty::OVERALL;
  myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
  // Define colors for the faces
  int i;
  for (i=0; i<12; i++)
    myVertexProperty->orderedRGBA.set1Value(i, SbColor(1, 0, 0).getPackedValue());

  // Define coordinates for vertices
  myVertexProperty->vertex.setValues(0, 12, vertices);
  
  // Define the FaceSet
  SoFaceSet *myFaceSet = new SoFaceSet;
  myFaceSet->numVertices.setValues(0, 4, numvertices);
  
  myFaceSet->vertexProperty.setValue(myVertexProperty);
  result->addChild(myFaceSet);

  result->unrefNoDelete();
  return result;
}

int
main(int, char **argv)
{
  // Initialize Inventor and Xt
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL) exit(1);
  
  SoSeparator *root = new SoSeparator;
  root->ref();
  // Callback to get keyboard event.
  SoEventCallback *myEventCB = new SoEventCallback;
  myEventCB->addEventCallback(SoKeyboardEvent::getClassTypeId(), myKeyPressCB, root);
  root->addChild(myEventCB);
  root->addChild(makeObjectFaceSet());
  
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setBackgroundColor(SbColor(0.5f, 0.5f, 0.5f));
  myViewer->setSceneGraph(root);

  myViewer->setTitle("Face Set with hole");
  myViewer->show();
  myViewer->viewAll();
  
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}

