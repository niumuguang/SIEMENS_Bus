///*
// * Copyright 1991-1995, Silicon Graphics, Inc.
// * ALL RIGHTS RESERVED
// *
// * UNPUBLISHED -- Rights reserved under the copyright laws of the United
// * States.   Use of a copyright notice is precautionary only and does not
// * imply publication or disclosure.
// *
// * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
// * Use, duplication or disclosure by the Government is subject to restrictions
// * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
// * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
// * in similar or successor clauses in the FAR, or the DOD or NASA FAR
// * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
// * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
// *
// * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
// * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
// * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
// * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
// * GRAPHICS, INC.
// */
////
////    This sample program reads in a inventor data file, and
////  uses the provided source code viewer to see the data.
////
//
//#include <Inventor/SoDB.h>	// for file reading
//#include <Inventor/SoInput.h>	// for file reading
//#include <Inventor/nodes/SoWWWInline.h>
//#include <Inventor/VRMLnodes/SoVRMLInline.h>
//#include <Inventor/Xt/SoXt.h>	// For SoXt::init()
//#include "MyExaminerViewer.h"
//
//int main(int argc, char *argv[])
//{
//    if (argc != 2) {
//	printf("Usage: %s data_file\n", argv[0]);
//	return(1);
//    }
//    
//    // Initialize Inventor and Xt
//    Widget myWindow = SoXt::init(argv[0]);
//    
//    //
//    // Read file in
//    //
//    SoInput in;
//    SoNode *scene;
//    if (! in.openFile(argv[1]))
//	return(1);
//
//    SoWWWInline::setReadAsSoFile(TRUE);
//    SoVRMLInline::setReadAsSoFile(TRUE);
//
//    if(! SoDB::read(&in, scene) || scene == NULL)
//	return(1);
//    
//    // create and show the viewer
//    MyExaminerViewer *myViewer = new MyExaminerViewer(myWindow);
//    myViewer->setSceneGraph(scene);
//    myViewer->show();
//    SoXt::show(myWindow);
//    
//    // Loop forever
//    SoXt::mainLoop();
//}
