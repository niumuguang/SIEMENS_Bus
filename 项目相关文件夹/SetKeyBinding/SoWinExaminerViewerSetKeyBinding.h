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

#ifndef _SO_WIN_EXAMINER_VIEWER_SET_KEY_BINDING_
#define _SO_WIN_EXAMINER_VIEWER_SET_KEY_BINDING_

#include <Inventor/Win/SoWinBeginStrict.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>

class SoWinExaminerViewerSetKeyBinding : public SoWinExaminerViewer {
  
public:
  
  SoWinExaminerViewerSetKeyBinding(Widget parent = NULL,
                                   const char *name = NULL, 
                                   SbBool buildInsideParent = TRUE, 
                                   SoWinFullViewer::BuildFlag flag = BUILD_ALL, 
                                   SoWinViewer::Type type = BROWSER);
  
  // To define new Cursors for this viewer. If this is not done, you will
  // get same cursors as the ExaminerViewer.
  virtual void defineCursors();
};

#endif /* _SO_WIN_EXAMINER_VIEWER_SET_KEY_BINDING_ */
