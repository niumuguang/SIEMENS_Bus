///////////////////////////////////////////////////////////////////////
// 
// TGS Open Inventor example for hole in an IndexedFaceSet
//
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
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoEventCallback.h>

#define MAC_CONSOLE
#include <Inventor/SoMacApp.h>
//////////////////////////////////////////////////////////////
//
// Positions of all of the vertices:
//
static const float 
vertexPositions[12][3] = {
  {   0,     0, 0},    
  {   1,     1, 0},    
  {   1,     0, 0},    
  {   1,    -1, 0},   

  {0.3f,  0.2f, 0}, 
  {0.8f,     0, 0},  
  {0.3f, -0.2f, 0}, 
  {   0,    -1, 0},   

  {0.1f, -0.5f, 0}, 
  {0.1f, -0.9f, 0}, 
  {0.4f, -0.9f, 0}, 
  
  {0.6f,     0, 0}
};
  
static int32_t 
indices[21] = {
  0,  3,  1,      SO_END_FACE_INDEX,  
  5,  6,  4,      SO_END_POLYGON_INDEX, // To finish the first block of contours.

  0,  7,  3,      SO_END_FACE_INDEX, 
  10, 9,  8,      SO_END_FACE_INDEX, 
  9,  7,  0,  8,  SO_END_POLYGON_INDEX // To finish the second block of contours.
};
 
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
    } else if (m_hints->windingType.getValue() == SoShapeHints::NO_WINDING_TYPE) {
      m_hints->windingType = SoShapeHints::ABS_GEQ_TWO_TYPE;
      printf("SoShapeHints::ABS_GEQ_TWO_TYPE\n");
    } else if (m_hints->windingType.getValue() == SoShapeHints::ABS_GEQ_TWO_TYPE) {
      m_hints->windingType = SoShapeHints::ODD_TYPE;
      printf("SoShapeHints::ODD_TYPE\n");
    }
  }
}

// Routine to create a scene graph representing a polygon with hole
SoSeparator *
makeObjectIndexedFaceSet()
{
  SoSeparator *result = new SoSeparator();
  result->ref();
 
  m_hints = new SoShapeHints;
  result->addChild(m_hints);
  m_hints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
  m_hints->windingType=SoShapeHints::ODD_TYPE;

  printf("Press 'W' (in selection mode) to change winding rule\n");
  printf("SoShapeHints::ODD_TYPE\n");

  SoVertexProperty *myVertexProperty = new SoVertexProperty;
  myVertexProperty->normalBinding = SoVertexProperty::OVERALL;
  myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
  
  // Define colors for the faces
  for (int i = 0; i < 12; i++)
    myVertexProperty->orderedRGBA.set1Value(i, SbColor(1, 0, 0).getPackedValue());

  // Define coordinates for vertices
  myVertexProperty->vertex.setValues(0, 12, vertexPositions);
  
  // Define the IndexedFaceSet, with indices into
  // the vertices:
  SoIndexedFaceSet *myFaceSet = new SoIndexedFaceSet;
  myFaceSet->coordIndex.setValues(0, 21, indices);
  
  myFaceSet->vertexProperty.setValue(myVertexProperty);
  result->addChild(myFaceSet);

  result->unrefNoDelete();
  return result;
}
//////////////////////////////////////////////////////////////

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
  root->addChild(makeObjectIndexedFaceSet());
  
  
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setBackgroundColor(SbColor(.5f, .5f, .5f));
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Indexed Face Set with hole");
  myViewer->show();
  myViewer->viewAll();
  
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}




