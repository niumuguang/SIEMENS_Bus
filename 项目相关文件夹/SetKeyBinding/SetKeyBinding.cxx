///////////////////////////////////////////////////////////////////////
// 
// TGS Open Inventor example to set key binding
//
// Copyright (C) 2003, TGS
// All Rights Reserved.
//
// Permission is granted for licensed customers of the Mercury
// Open Inventor library to use this source for any reasonable
// purpose that involves the Mercury Open Inventor library.
//
///////////////////////////////////////////////////////////////////////

#include <Inventor/Win/SoWin.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

#include "SoWinExaminerViewerSetKeyBinding.h"

void
main(int, char **argv)
{
  Widget myWindow = SoWin::init(argv[0]);
  if (myWindow == NULL) exit(1);
  
  SoSeparator *root = new SoSeparator;
  root->ref();
  
  root->addChild(new SoCone);
  
  // Set up viewer:
  SoWinExaminerViewerSetKeyBinding *myViewer = 
    new SoWinExaminerViewerSetKeyBinding(myWindow);
  
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Constrained Examiner Viewer");
  myViewer->setFeedbackVisibility(TRUE);
  myViewer->setFeedbackSize(100);
  myViewer->show();
  
  SoWin::show(myWindow);
  SoWin::mainLoop();

  root->unref();
  delete myViewer;
  SoWin::finish();
}
