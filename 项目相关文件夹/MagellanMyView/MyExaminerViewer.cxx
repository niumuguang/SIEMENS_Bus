/*
 * Copyright 1991-1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 */
/*
 * Copyright (C) 1990-95   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |
 |   Classes	: MyExaminerViewer
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <inttypes.h>
#include <Windows.h>
#include <WindowsX.h>
//#include <X11/Intrinsic.h>
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//
//#include <Xm/LabelG.h>
//#include <Xm/Text.h>
//#include <Xm/ToggleB.h>
//#include <Xm/ToggleBG.h>
//#include <Xm/Form.h>
#include <Sgm/ThumbWheel.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtCursors.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/SoXtIcons.h>
#include "MyExaminerViewer.h"
#include "MyBitmapButton.h"
#include <Inventor/sys/SoGL.h>

#if defined(_DEBUG)
#  include <Inventor/errors/SoDebugError.h>
#  include <SoDebug.h>
#endif

/*
 *  Defines
 */

enum ViewerModes {
    PICK_MODE, 
    VIEW_MODE, 
    SPIN_MODE_ACTIVE, 
    PAN_MODE, 
    PAN_MODE_ACTIVE, 
    DOLLY_MODE_ACTIVE, 
    SEEK_MODE
};

// list of custom push buttons
enum {
    CAM_PUSH = 0,
    PUSH_NUM
};


// size of the rotation buffer, which is used to animate the spinning ball.
#define ROT_BUFF_SIZE 3


//
// The point of interest geometry description
//
char *MyExaminerViewer::geometryBuffer = (char *)"\
#Inventor V2.0 ascii\n\
\
Separator { \
    PickStyle { style UNPICKABLE } \
    LightModel { model BASE_COLOR } \
    MaterialBinding { value PER_PART } \
    DrawStyle { lineWidth 2 } \
    Coordinate3 { point [0 0 0, 1 0 0, 0 1 0, 0 0 1] } \
    BaseColor { rgb [1 0 0, 0 1 0, 0 0 1] } \
    IndexedLineSet { coordIndex [1, 0, 2, -1, 0, 3] } \
     \
    LightModel { model PHONG } \
    MaterialBinding { value OVERALL } \
    Complexity { value .1 } \
    Separator { \
    	Material { \
	    diffuseColor    [ 0.5 0 0 ] \
	    emissiveColor   [ 0.5 0 0 ] \
	} \
	Translation { translation 1 0 0 } \
    	RotationXYZ { axis Z angle -1.570796327 } \
    	Cone { bottomRadius .2 height .3 } \
    } \
    Separator { \
    	Material { \
	    diffuseColor    [ 0 0.5 0 ] \
	    emissiveColor   [ 0 0.5 0 ] \
	} \
	Translation { translation 0 1 0 } \
    	Cone { bottomRadius .2 height .3 } \
    } \
    Material { \
	diffuseColor    [ 0 0 0.5 ] \
	emissiveColor   [ 0 0 0.5 ] \
    } \
    Translation { translation 0 0 1 } \
    RotationXYZ { axis X angle 1.570796327 } \
    Cone { bottomRadius .2 height .3 } \
} ";


static char *thisClassName = (char *)"MyExaminerViewer";

////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
MyExaminerViewer::MyExaminerViewer(
    Widget parent,
    const char *name, 
    SbBool buildInsideParent, 
    MyFullViewer::BuildFlag b, 
    MyViewer::Type t)
	: MyFullViewer(
	    parent,
	    name, 
	    buildInsideParent, 
	    b, 
	    t, 
	    FALSE) // tell GLWidget not to build just yet  
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area is what the app wants, so buildNow = TRUE
    constructorCommon(TRUE);
}

////////////////////////////////////////////////////////////////////////
//
// SoEXTENDER constructor - the subclass tells us whether to build or not
//
MyExaminerViewer::MyExaminerViewer(
    Widget parent,
    const char *name, 
    SbBool buildInsideParent, 
    MyFullViewer::BuildFlag b, 
    MyViewer::Type t, 
    SbBool buildNow)
	: MyFullViewer(
	    parent,
	    name, 
	    buildInsideParent,
	    b,  
	    t, 
	    FALSE) // tell GLWidget not to build just yet  
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area may be what the app wants, 
    // or it may want a subclass of render area. Pass along buildNow
    // as it was passed to us.
    constructorCommon(buildNow);
}

////////////////////////////////////////////////////////////////////////
//
// Called by the constructors
//
// private
//
void
MyExaminerViewer::constructorCommon(SbBool buildNow)
//
////////////////////////////////////////////////////////////////////////
{
    // init local vars
    addVisibilityChangeCallback(visibilityChangeCB, this);
    mode = isViewing() ? VIEW_MODE : PICK_MODE;
    createdCursors = FALSE;
    spinCursor = panCursor = dollyCursor = seekCursor = 0;
    firstBuild = TRUE;
    setSize( SbVec2s(500, 390) );  // default size
    setClassName(thisClassName);
    
    // axis of rotation feedback vars
    feedbackFlag = FALSE;
    feedbackRoot = NULL;
    feedbackSwitch = NULL;
    feedbackSize = 20.0;
    feedbackSizeWheel = NULL;
    
    // init animation variables
    animationEnabled = TRUE;
    animatingFlag = FALSE;
    rotBuffer = new SbRotation[ROT_BUFF_SIZE];
    lastMotionTime = 0;
    animationSensor = new
	SoFieldSensor(MyExaminerViewer::animationSensorCB, this);
#if defined(_DEBUG)
    if (SoDebug::GetEnv("IV_DEBUG_SENSORS")) {
	SoDebug::NamePtr("examinerSpinSensor", animationSensor);
    }
#endif
    
    // init the projector class
    SbViewVolume vv;
    vv.ortho(-1, 1, -1, 1, -10, 10);
    sphereSheet = new SbSphereSheetProjector;
    sphereSheet->setViewVolume( vv );
    sphereSheet->setSphere( SbSphere( SbVec3f(0, 0, 0), 0.7F) );
    
    // assign decoration names
    setPopupMenuString("Examiner Viewer");
    setBottomWheelString("Roty");
    setLeftWheelString("Rotx");
    setPrefSheetString("Examiner Viewer Preference Sheet");
    
    for (int i=0; i<PUSH_NUM; i++)
	buttonList[i] = NULL;
    
    // Build the widget tree, and let SoXtComponent know about our base widget.
    if (buildNow) {
	Widget w = buildWidget(getParentWidget());
	setBaseWidget(w);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

MyExaminerViewer::~MyExaminerViewer()
//
////////////////////////////////////////////////////////////////////////
{
    delete animationSensor;
    
    for (int i=0; i<PUSH_NUM; i++)
	delete buttonList[i];
    
    delete sphereSheet;
    if (feedbackRoot)
	feedbackRoot->unref();
    
    // free the viewer cursors
    /*if (getDisplay()) {
	Display *display = getDisplay();
	if (spinCursor) XFreeCursor(display, spinCursor);
	if (panCursor) XFreeCursor(display, panCursor);
	if (dollyCursor) XFreeCursor(display, dollyCursor);
	if (seekCursor) XFreeCursor(display, seekCursor);
    }*/
    
    delete [] rotBuffer;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Shows/hide the point of rotation feedback geometry
//
// Use: public
void
MyExaminerViewer::setFeedbackVisibility(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == feedbackFlag || camera == NULL) {
	feedbackFlag = flag;
	return;
    }
    
    //
    // find the camera parent to insert/remove the feedback root
    //
    SoSearchAction sa;
    sa.setNode(flag ? (SoNode *)camera : (SoNode *)feedbackRoot);
    sa.apply(sceneRoot);
    SoFullPath *path = (SoFullPath *) sa.getPath();
    if (!path) {
#if defined(_DEBUG)
	SoDebugError::post("MyExaminerViewer::setFeedbackVisibility",
			    flag ? "ERROR: cannot find camera in graph" :
			    "ERROR: cannot find axis feedback in graph");
#endif
	return;
    }
    SoGroup *parent = (SoGroup *)path->getNode(path->getLength() - 2);
    
    feedbackFlag = flag;
    
    // make sure the feedback has been built
    if (!feedbackRoot)
	createFeedbackNodes();
    
    //
    // inserts/remove the feedback axis group
    //
    if (feedbackFlag) {
	
	// return if geometry is already there (this should be an error !)
	if (parent->findChild(feedbackRoot) >= 0)
	    return;
	
	// place the feedback right after the headlight if the headlight
	// is turned on. Otherwise place it right after the camera.
	if ( isHeadlight() )
	    parent->insertChild(feedbackRoot, parent->findChild(camera) + 2);
	else
	    parent->insertChild(feedbackRoot, parent->findChild(camera) + 1);
	
	// make sure the feedback switch is turned to the correct state now
	// that the feedback root has been inserted in the scene
	feedbackSwitch->whichChild.setValue(viewingFlag ? SO_SWITCH_ALL : SO_SWITCH_NONE);
    }
    else
	parent->removeChild(feedbackRoot);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the feedback size.
//
// Use: public
void
MyExaminerViewer::setFeedbackSize(int newSize)
//
////////////////////////////////////////////////////////////////////////
{
    if (feedbackSize == newSize)
	return;
    
    // assign new value and redraw (since it is not a field in the scene)
    feedbackSize = newSize;
    if (isFeedbackVisible() && isViewing())
	scheduleRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Remove the extra geometry when doing a viewAll.
//
// Use: virtual public
void
MyExaminerViewer::viewAll()
//
////////////////////////////////////////////////////////////////////////
{
    // stop spinning
    if ( isAnimating() )
    	stopAnimating();
    
    // temporarily remove the feedback geometry
    if (feedbackFlag && isViewing() && feedbackSwitch)
	feedbackSwitch->whichChild.setValue( SO_SWITCH_NONE );
    
    // call the base class
    MyFullViewer::viewAll();
    
    // now add the geometry back in
    if (feedbackFlag && isViewing() && feedbackSwitch)
	feedbackSwitch->whichChild.setValue( SO_SWITCH_ALL );
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the parent class and stop animation if any
//
// Use: virtual public
void
MyExaminerViewer::resetToHomePosition()
//
////////////////////////////////////////////////////////////////////////
{
    // stop spinning
    if ( isAnimating() )
    	stopAnimating();
    
    // call the base class
    MyFullViewer::resetToHomePosition();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the parent class and insert/remove the feedback root
//
// Use: virtual public
void
MyExaminerViewer::setCamera(SoCamera *newCamera)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == newCamera)
	return;
    
    // set the right thumbwheel label and toggle button image based on 
    // the camera type
    if (newCamera != NULL && (camera == NULL || 
	newCamera->getTypeId() != camera->getTypeId())) {
	if (newCamera->isOfType(SoOrthographicCamera::getClassTypeId())) {
	    if (buttonList[CAM_PUSH])
		buttonList[CAM_PUSH]->setIcon(so_xt_ortho_bits, 
		    so_xt_icon_width, so_xt_icon_height);
	    setRightWheelString("Zoom");
	}
	else {
	    if (buttonList[CAM_PUSH])
		buttonList[CAM_PUSH]->setIcon(so_xt_persp_bits, 
		    so_xt_icon_width, so_xt_icon_height);
	    setRightWheelString("Dolly");
	}
    }
    
    // detach feedback which depends on camera
    if ( feedbackFlag ) {
	setFeedbackVisibility(FALSE);
	feedbackFlag = TRUE;  // can later be turned on
    }
    
    // call parent class
    MyFullViewer::setCamera(newCamera);
    
    // attach feedback back on
    if ( feedbackFlag ) {
	feedbackFlag = FALSE; // enables routine to be called
	setFeedbackVisibility(TRUE);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursors on the window,
//  plus insert/remove the feedback geometry.
//
// Use: virtual public
void
MyExaminerViewer::setViewing(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == viewingFlag)
	return;
    
    // call the parent class
    MyFullViewer::setViewing(flag);
    
    switchMode(isViewing() ? VIEW_MODE : PICK_MODE);
    
    // show/hide the feedback geometry based on the viewing state
    if (feedbackFlag && feedbackSwitch)
	feedbackSwitch->whichChild.setValue(viewingFlag ? SO_SWITCH_ALL : SO_SWITCH_NONE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Enables/Disable the viewer cursor on the window.
//
// Use: virtual public
void
MyExaminerViewer::setCursorEnabled(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == cursorEnabledFlag)
	return;
    
    cursorEnabledFlag = flag;
    
    if (! isViewing())
	return;
    
    updateCursor();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Process the given event to do viewing stuff
//
// Use: virtual protected
void
MyExaminerViewer::processEvent(XAnyEvent *xe)
//
////////////////////////////////////////////////////////////////////////
{
    if ( processCommonEvents(xe) )
	return;
    
    if (!createdCursors)
	updateCursor();
    
    XButtonEvent    *be;
    XMotionEvent    *me;
    XKeyEvent	    *ke;
   // XCrossingEvent  *ce;
    KeySym	    keysym;
    
    SbVec2s raSize = getGlxSize();
    
 //   switch(xe->type) {
 //   case ButtonPress:
 //   case ButtonRelease:
	//be = (XButtonEvent *)xe;
	//if (be->button != Button1 && be->button != Button2)
	//    break;
	//
	//locator[0] = be->x;
	//locator[1] = raSize[1] - be->y;
	//
	//if (mode == SEEK_MODE) {
	//    if (xe->type == ButtonPress)
	//	seekToPoint(locator);
	//}
	//else {
	//    if (xe->type == ButtonPress)
	//	interactiveCountInc();
	//    else { //... ButtonRelease
	//	
	//	// check if we need to start spinning
	//	if (mode == SPIN_MODE_ACTIVE && animationEnabled 
	//	    && lastMotionTime == be->time) {
	//	    animatingFlag = TRUE;
	//	    computeAverage = TRUE;
	//	    animationSensor->attach(viewerRealTime);
	//	    interactiveCountInc();
	//	}
	//	
	//	interactiveCountDec();
	//    }
	//    updateViewerMode(be->state);
	//}
	//if (xe->type == ButtonPress)
	//    stopAnimating();
	//break;
	//
 //   case KeyPress:
 //   case KeyRelease:
	//ke = (XKeyEvent *)xe;
	//keysym = XLookupKeysym(ke, 0);
	//
	//locator[0] = ke->x;
	//locator[1] = raSize[1] - ke->y;
	//if (keysym == XK_Control_L || keysym == XK_Control_R)
	//    updateViewerMode(ke->state);
	//break;
	//
 //   case MotionNotify:
	//me = (XMotionEvent *)xe;
	//switch (mode) {
	//    case SPIN_MODE_ACTIVE:
	//	lastMotionTime = me->time;
	//	spinCamera(SbVec2f(me->x/float(raSize[0]), (raSize[1] - me->y)/float(raSize[1])));
	//	break;
	//    case PAN_MODE_ACTIVE:
	//	panCamera(SbVec2f(me->x/float(raSize[0]), (raSize[1] - me->y)/float(raSize[1])));
	//	break;
	//    case DOLLY_MODE_ACTIVE:
	//	dollyCamera( SbVec2s(me->x, raSize[1] - me->y) );
	//	break;
	//}
	//break;
	//
 //   case LeaveNotify:
 //   case EnterNotify:
	//ce = (XCrossingEvent *)xe;
	////
	//// because the application might use Ctrl-key for motif menu
	//// accelerators we might not receive a key-up event, so make sure
	//// to reset any Ctrl mode if we loose focus, but don't do anything
	//// if Ctrl-key is not down (nothing to do) or if a mouse button 
	//// is down (we will get another leaveNotify).
	////
	//if (! (ce->state & ControlMask))
	//    break;
	//if (ce->state & Button1Mask || ce->state & Button2Mask)
	//    break;
	//if (xe->type == LeaveNotify)
	//    switchMode(VIEW_MODE);
	//else
	//    updateViewerMode(ce->state);
	//break;
 //   }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    sets the viewer mode based on what keys and buttons are being pressed
//
// Use: private
void
MyExaminerViewer::updateViewerMode(unsigned int state)
//
////////////////////////////////////////////////////////////////////////
{
    // ??? WARNING - this routine ONLY works because of 
    // ??? MyViewer::updateEventState() which is called for us
    // ??? by MyViewer::processCommonEvents(). 
    // ??? (XEvent state for button and modifier keys is not updated
    // ??? until after the event is received. WEIRD)
    
    // LEFT+MIDDLE down
 //   if (state & Button1Mask && state & Button2Mask) {
	//switchMode(DOLLY_MODE_ACTIVE);
 //   }
 //   
 //   // LEFT down
 //   else if (state & Button1Mask) {
	//if (state & ControlMask)
	//    switchMode(PAN_MODE_ACTIVE);
	//else
	//    switchMode(SPIN_MODE_ACTIVE);
 //   }
 //   
 //   // MIDDLE DOWN
 //   else if (state & Button2Mask) {
	//if (state & ControlMask)
	//    switchMode(DOLLY_MODE_ACTIVE);
	//else
	//    switchMode(PAN_MODE_ACTIVE);
 //   }
 //   
 //   // no buttons down...
 //   else {
	//if (state & ControlMask)
	//    switchMode(PAN_MODE);
	//else
	//    switchMode(VIEW_MODE);
 //   }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    switches to the specified viewer mode
//
// Use: private
void
MyExaminerViewer::switchMode(int newMode)
//
////////////////////////////////////////////////////////////////////////
{
 //   Widget renderAreaWidget = getRenderAreaWidget();
 //   SbVec2s raSize = getGlxSize();
 //   
 //   // assing new mode
 //   int prevMode = mode;
 //   mode = newMode;
 //   
 //   // update the cursor
 //   updateCursor();
 //   
 //   // switch to new viewer mode
 //   switch (newMode) {
	//case PICK_MODE:
	//    if (renderAreaWidget && XtWindow(renderAreaWidget)) {
	//	// ???? is if are going into PICK mode and some of our
	//	// mouse buttons are still down, make sure to decrement
	//	// interactive count correctly (correct draw style). One
	//	// for the LEFT and one for MIDDLE mouse.
	//	Window root_return, child_return;
	//	int root_x_return, root_y_return;
	//	int win_x_return, win_y_return;
	//	unsigned int mask_return;
	//	XQueryPointer(XtDisplay(renderAreaWidget), XtWindow(renderAreaWidget), 
	//	    &root_return, &child_return,
	//	    &root_x_return, &root_y_return, &win_x_return, 
	//	    &win_y_return, &mask_return);
	//	if (mask_return & Button1Mask && prevMode != SEEK_MODE)
	//	    interactiveCountDec();
	//	if (mask_return & Button2Mask && prevMode != SEEK_MODE)
	//	    interactiveCountDec();
	//    }
	//    stopAnimating();
	//    break;
	//    
	//case SPIN_MODE_ACTIVE:
	//    // set the sphere sheet starting point
	//    sphereSheet->project(
	//	SbVec2f(locator[0]/float(raSize[0]), locator[1]/float(raSize[1])) );
	//    
	//    // reset the animation queue
	//    firstIndex = 0;
	//    lastIndex = -1;
	//    break;
	//    
	//case PAN_MODE_ACTIVE:
	//    {
	//    // Figure out the focal plane
	//    SbMatrix mx;
	//    mx = camera->orientation.getValue();
	//    SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	//    SbVec3f fp = camera->position.getValue() + 
	//	forward * camera->focalDistance.getValue();
	//    focalplane = SbPlane(forward, fp);
	//    
	//    // map mouse starting position onto the panning plane
	//    SbViewVolume    cameraVolume;
	//    SbLine	    line;
	//    cameraVolume = camera->getViewVolume(raSize[0]/float(raSize[1]));
	//    cameraVolume.projectPointToLine(
	//	SbVec2f(locator[0]/float(raSize[0]), locator[1]/float(raSize[1])), line);
	//    focalplane.intersect(line, locator3D);
	//    }
	//    break;
 //   }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    updates the viewer cursor
//
// Use: private
void
MyExaminerViewer::updateCursor()
//
////////////////////////////////////////////////////////////////////////
{
 //   Widget w = getRenderAreaWidget();
 //   Display *display = w ? XtDisplay(w) : NULL;
 //   Window window = w ? XtWindow(w) : 0;
 //   
 //   if (! window)
	//return;
 //   
 //   if (! createdCursors)
	//defineCursors();
 //   
 //   // the viewer cursor are not enabled, then we don't set a new cursor.
 //   // Instead erase the old viewer cursor.
 //   if (! cursorEnabledFlag) {
	//XUndefineCursor(display, window);
	//return;
 //   }
 //   
 //   // ...else set the right cursor for the viewer mode....
 //   switch(mode) {
	//case PICK_MODE:
	//    XUndefineCursor(display, window);
	//    break;
	//    
	//case VIEW_MODE:
	//case SPIN_MODE_ACTIVE:
	//    XDefineCursor(display, window, spinCursor);
	//    break;
	//    
	//case DOLLY_MODE_ACTIVE:
	//    XDefineCursor(display, window, dollyCursor);
	//    break;
	//    
	//case PAN_MODE:
	//case PAN_MODE_ACTIVE:
	//    XDefineCursor(display, window, panCursor);
	//    break;
	//    
	//case SEEK_MODE:
	//    XDefineCursor(display, window, seekCursor);
	//    break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    draws viewer feedback during a render area redraw of the scene.
//
// Use: virtual protected
void
MyExaminerViewer::actualRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    // place the feedback at the focal point
    // ??? we really only need to do this when the camera changes
    if (isViewing() && feedbackFlag && camera != NULL && feedbackRoot) {
	
	// adjust the position to be at the focal point
	SbMatrix mx;
	mx = camera->orientation.getValue();
	SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	feedbackTransNode->translation = camera->position.getValue() + 
	    camera->focalDistance.getValue() * forward;
	
	// adjust the size to be constant on the screen
	float myHeight;
	if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
	    float angle = ((SoPerspectiveCamera *)camera)->heightAngle.getValue();
	    myHeight = (float)(camera->focalDistance.getValue() * tan(angle/2));
	}
	else if (camera->isOfType(SoOrthographicCamera::getClassTypeId()))
	    myHeight = ((SoOrthographicCamera *)camera)->height.getValue() / 2;
	
	// ??? getGlxSize[1] == 0 the very first time, so return in that case
	// ??? else the redraws are 3 times slower from now on !! (alain)
	if (getGlxSize()[1] != 0) {
	    float mySize = 2.0 * myHeight * feedbackSize / float (getGlxSize()[1]);
	    feedbackScaleNode->scaleFactor.setValue(mySize, mySize, mySize);
	}
    }
    
    // have the base class draw the scene
    MyFullViewer::actualRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Enable/disable the animation feature of the Examiner
//
// Use: public
void
MyExaminerViewer::setAnimationEnabled(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (animationEnabled == flag)
	return;
    
    animationEnabled = flag;
    if ( !animationEnabled && isAnimating())
        stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Stops ongoing animation (if any)
//
// Use: public
void
MyExaminerViewer::stopAnimating()
//
////////////////////////////////////////////////////////////////////////
{
    if (animatingFlag) {
	animatingFlag = FALSE;
	animationSensor->detach();
	animationSensor->unschedule();
	interactiveCountDec();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursors on the window
//
// Use: virtual protected
void
MyExaminerViewer::setSeekMode(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if ( !isViewing() )
	return;
    
    // stop spinning
    if (isAnimating())
    	stopAnimating();
    
    // call the base class
    MyFullViewer::setSeekMode(flag);
    
    mode = isSeekMode() ? SEEK_MODE : VIEW_MODE;
    
    updateCursor();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Redefine this routine to add some viewer specific stuff.
//
// Use: virtual protected
void
MyExaminerViewer::createPrefSheet()
//
////////////////////////////////////////////////////////////////////////
{
    // create the preference sheet shell and form widget
    Widget shell, form;
    createPrefSheetShellAndForm(shell, form);
    
    // create all of the parts
    Widget widgetList[20];
    int num = 0;
    createDefaultPrefSheetParts(widgetList, num, form);
    widgetList[num++] = createExamPrefSheetGuts(form);
    
    layoutPartsAndMapPrefSheet(widgetList, num, form, shell);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Creates the viewer extra pref sheet stuff
//
// Use: private
Widget
MyExaminerViewer::createExamPrefSheetGuts(Widget parent)
//
////////////////////////////////////////////////////////////////////////
{
 //   Widget toggles[2], labels[2];
 //   Arg args[12];
 //   int n;
 //   
 //   // create a form to hold everything together
 //   Widget form = XtCreateWidget("", xmFormWidgetClass, parent, NULL, 0);
 //   
 //   // create all the parts
 //   n = 0;
 //   XtSetArg(args[n], XmNset, animationEnabled); n++;
 //   XtSetArg(args[n], XmNspacing, 0); n++;
 //   XtSetArg(args[n], XmNhighlightThickness, 0); n++;
 //   toggles[0] = XtCreateWidget("", xmToggleButtonGadgetClass, form, args, n);
 //   labels[0] = XtCreateWidget("Enable spin animation", 
	//xmLabelGadgetClass, form, NULL, 0);
 //   XtAddCallback(toggles[0], XmNvalueChangedCallback, 
	//(XtCallbackProc) MyExaminerViewer::animPrefSheetToggleCB, 
	//(XtPointer) this);
 //   
 //   n = 0;
 //   XtSetArg(args[n], XmNsensitive, camera != NULL); n++;
 //   XtSetArg(args[n], XmNset, feedbackFlag); n++;
 //   XtSetArg(args[n], XmNspacing, 0); n++;
 //   XtSetArg(args[n], XmNhighlightThickness, 0); n++;
 //   toggles[1] = XtCreateWidget("", xmToggleButtonGadgetClass, form, args, n);
 //   n = 0;
 //   XtSetArg(args[n], XmNsensitive, camera != NULL); n++;
 //   labels[1] = XtCreateWidget("Show point of rotation axes", 
	//xmLabelGadgetClass, form, args, n);
 //   XtAddCallback(toggles[1], XmNvalueChangedCallback, 
	//(XtCallbackProc) MyExaminerViewer::feedbackPrefSheetToggleCB, 
	//(XtPointer) this);
 //   
 //   // layout
 //   n = 0;
 //   XtSetArg(args[n], XmNleftAttachment,    XmATTACH_WIDGET); n++;
 //   XtSetArg(args[n], XmNleftWidget,	    toggles[0]); n++;
 //   XtSetArg(args[n], XmNtopAttachment,	    XmATTACH_OPPOSITE_WIDGET); n++;
 //   XtSetArg(args[n], XmNtopWidget,	    toggles[0]); n++;
 //   XtSetArg(args[n], XmNbottomAttachment,  XmATTACH_OPPOSITE_WIDGET); n++;
 //   XtSetArg(args[n], XmNbottomWidget,	    toggles[0]); n++;
 //   XtSetValues(labels[0], args, n);
 //   
 //   n = 0;
 //   XtSetArg(args[n], XmNtopAttachment,	    XmATTACH_WIDGET); n++;
 //   XtSetArg(args[n], XmNtopWidget,	    toggles[0]); n++;
 //   XtSetArg(args[n], XmNtopOffset,	    10); n++;
 //   XtSetValues(toggles[1], args, n);
 //   
 //   n = 0;
 //   XtSetArg(args[n], XmNleftAttachment,    XmATTACH_WIDGET); n++;
 //   XtSetArg(args[n], XmNleftWidget,	    toggles[1]); n++;
 //   XtSetArg(args[n], XmNtopAttachment,	    XmATTACH_OPPOSITE_WIDGET); n++;
 //   XtSetArg(args[n], XmNtopWidget,	    toggles[1]); n++;
 //   XtSetArg(args[n], XmNbottomAttachment,  XmATTACH_OPPOSITE_WIDGET); n++;
 //   XtSetArg(args[n], XmNbottomWidget,	    toggles[1]); n++;
 //   XtSetValues(labels[1], args, n);
 //   
 //   // manage children
 //   XtManageChildren(toggles, 2);
 //   XtManageChildren(labels, 2);
 //   
 //   if (feedbackFlag && camera)
	//toggleFeedbackWheelSize(toggles[1]);
 //   
 //   return form;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Brings the viewer help card (called by "?" push button)
//
// Use: virtual protected
void
MyExaminerViewer::openViewerHelpCard()
//
////////////////////////////////////////////////////////////////////////
{
    // tell the base class to open the file for us
    openHelpCard("MyExaminerViewer.help");
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Call the base class and stop animating
//
// Use: virtual protected

void
MyExaminerViewer::bottomWheelStart()
//
////////////////////////////////////////////////////////////////////////
{
    MyFullViewer::bottomWheelStart();
    stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Call the base class and stop animating
//
// Use: virtual protected

void
MyExaminerViewer::leftWheelStart()
//
////////////////////////////////////////////////////////////////////////
{
    MyFullViewer::bottomWheelStart();
    stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the object around the screen x axis (called by thumb wheel).
//
// Use: virtual protected

void
MyExaminerViewer::bottomWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    // get rotation and apply to camera
    SbVec3f axis(0, 1, 0);
    SbRotation rot(axis, bottomWheelVal - newVal);
    rotateCamera(rot);
    
    bottomWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the object around the screen y axis (called by thumb wheel).
//
// Use: virtual protected

void
MyExaminerViewer::leftWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    // get rotation and apply to camera
    SbVec3f axis(1, 0, 0);
    SbRotation rot(axis, newVal - leftWheelVal);
    rotateCamera(rot);
    
    leftWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   	Moves the camera closer/further away from the plane of interest
//  (perspective camera case), else change the camera height (orthographic
//  camera case).
//
// Use: virtual protected

void
MyExaminerViewer::rightWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
	// change the ortho camera height
	SoOrthographicCamera *cam = (SoOrthographicCamera *) camera;
	cam->height = (float)(cam->height.getValue() * pow(2.0F, newVal - rightWheelVal));
    }
    else {
	// shorter/grow the focal distance given the wheel rotation
	float focalDistance = camera->focalDistance.getValue();;
	float newFocalDist = focalDistance;
	newFocalDist *= (float)pow(2.0F, newVal - rightWheelVal);
	
	// finally reposition the camera
	SbMatrix mx;
	mx = camera->orientation.getValue();
	SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	camera->position = camera->position.getValue() + 
			   (focalDistance - newFocalDist) * forward;
	camera->focalDistance = newFocalDist;
    }
    
    rightWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This routine is used to define cursors (can only be called after
//  window has been realized).
//
// Use: private

void
MyExaminerViewer::defineCursors()
//
////////////////////////////////////////////////////////////////////////
{
 //   XColor foreground;
 //   Pixmap source;
 //   Display *display = getDisplay();
 //   Drawable d = DefaultRootWindow(display);
 //   
 //   // set color
 //   foreground.red = 65535;
 //   foreground.green = foreground.blue = 0;
 //   
 //   // spin cursor
 //   source = XCreateBitmapFromData(display, d, 
	//so_xt_curved_hand_bits, so_xt_curved_hand_width, so_xt_curved_hand_height);
 //   spinCursor = XCreatePixmapCursor(display, source, source, 
	//&foreground, &foreground, so_xt_curved_hand_x_hot, so_xt_curved_hand_y_hot);
 //   XFreePixmap(display, source);
 //   
 //   // panning cursor
 //   source = XCreateBitmapFromData(display, d, 
	//so_xt_flat_hand_bits, so_xt_flat_hand_width, so_xt_flat_hand_height);
 //   panCursor = XCreatePixmapCursor(display, source, source, 
	//&foreground, &foreground, so_xt_flat_hand_x_hot, so_xt_flat_hand_y_hot);
 //   XFreePixmap(display, source);
 //   
 //   // dolly cursor
 //   source = XCreateBitmapFromData(display, d, 
	//so_xt_pointing_hand_bits, so_xt_pointing_hand_width, so_xt_pointing_hand_height);
 //   dollyCursor = XCreatePixmapCursor(display, source, source, 
	//&foreground, &foreground, so_xt_pointing_hand_x_hot, so_xt_pointing_hand_y_hot);
 //   XFreePixmap(display, source);
 //   
 //   // seek cursor
 //   source = XCreateBitmapFromData(display, d, 
	//(char *)so_xt_target_bits, so_xt_target_width, so_xt_target_height);
 //   seekCursor = XCreatePixmapCursor(display, source, source, 
	//&foreground, &foreground, so_xt_target_x_hot, so_xt_target_y_hot);
 //   XFreePixmap(display, source);
 //   
 //   createdCursors = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the camera around pt of interest by given rotation
//
// Use: private

void
MyExaminerViewer::rotateCamera(const SbRotation &rot)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // get center of rotation
    SbRotation camRot = camera->orientation.getValue();
    float radius = camera->focalDistance.getValue();
    SbMatrix mx;
    mx = camRot;
    SbVec3f forward( -mx[2][0], -mx[2][1], -mx[2][2]);
    SbVec3f center = camera->position.getValue()
	+ radius * forward;
    
    // apply new rotation to the camera
    camRot = rot * camRot;
    camera->orientation = camRot;
    
    // reposition camera to look at pt of interest
    mx = camRot;
    forward.setValue( -mx[2][0], -mx[2][1], -mx[2][2]);
    camera->position = center - radius * forward;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera into the plane defined by the camera forward vector
//  and the focal point to follow the new mouse location.
//
// Use: private

void
MyExaminerViewer::panCamera(const SbVec2f &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // map new mouse location into the camera focal plane
    SbViewVolume    cameraVolume;
    SbLine	    line;
    SbVec3f	    newLocator3D;
    SbVec2s	    raSize = getGlxSize();
    cameraVolume = camera->getViewVolume(raSize[0]/float(raSize[1]));
    cameraVolume.projectPointToLine(newLocator, line);
    focalplane.intersect(line, newLocator3D);
    
    // move the camera by the delta 3D position amount
    camera->position = camera->position.getValue() + 
	(locator3D - newLocator3D);
    
    // You would think we would have to set locator3D to
    // newLocator3D here.  But we don't, because moving the camera
    // essentially makes locator3D equal to newLocator3D in the
    // transformed space, and we will project the next newLocator3D in
    // this transformed space.
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    rotates the camera using the sheet sphere projector, given the new
//  mouse location.
//
// Use: private

void
MyExaminerViewer::spinCamera(const SbVec2f &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    // find rotation and rotate camera
    SbRotation rot;
    sphereSheet->projectAndGetRotation(newLocator, rot);
    rot.invert();

    rotateCamera(rot);
    
    // save rotation for animation
    lastIndex = ((lastIndex+1) % ROT_BUFF_SIZE);
    rotBuffer[lastIndex] = rot;
    
    // check if queue is full
    if (((lastIndex+1) % ROT_BUFF_SIZE) == firstIndex)
	firstIndex = ((firstIndex+1) % ROT_BUFF_SIZE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera forward/backward based on the new mouse position.
//  (perspective camera), else change the camera height (orthographic
//  camera case).
//
// Use: private

void
MyExaminerViewer::dollyCamera(const SbVec2s &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
  if (camera == NULL)
    return;
  
  // moving the mouse up/down will move the camera futher/closer.
  // moving the camera sideway will not move the camera at all
  float d = (newLocator[1] - locator[1]) / 40.0;
  
  if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    // change the ortho camera height
    SoOrthographicCamera *cam = (SoOrthographicCamera *) camera;
    cam->height = (float)(cam->height.getValue() * pow(2.0F, d));
  }
  else {
    // shorter/grow the focal distance given the mouse move
    float focalDistance = camera->focalDistance.getValue();;
    float newFocalDist = (float)(focalDistance * pow(2.0F, d));
    
    // finally reposition the camera
    SbMatrix mx;
    mx = camera->orientation.getValue();
    SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
    camera->position = camera->position.getValue() + 
                       (focalDistance - newFocalDist) * forward;
    camera->focalDistance = newFocalDist;
  }
  
  locator = newLocator;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Routine which animates the ball spinning (called by sensor).
//
// Use: private

void
MyExaminerViewer::doSpinAnimation()
//
////////////////////////////////////////////////////////////////////////
{
    //
    // check if average rotation needs to be computed
    //
    
    if (computeAverage) {
	float averageAngle, angle;
	SbVec3f averageAxis, axis;
	
	// get number of samples
	int num = (((lastIndex - firstIndex) + 1 + 
	    ROT_BUFF_SIZE) % ROT_BUFF_SIZE);
	
	// check for not enough samples
	if (num < 2) {
	    stopAnimating();
	    return;
	}
	
	// get average axis of rotation
	// ??? right now only take one sample
	rotBuffer[firstIndex].getValue(averageAxis, angle);
	
	// get average angle of rotation
	averageAngle = 0;
	for (int i=0; i<num; i++) {
	    int n = (firstIndex + i) % ROT_BUFF_SIZE;
	    rotBuffer[n].getValue(axis, angle);
	    averageAngle += angle;
	}
	averageAngle /= float(num);
	
	averageRotation.setValue(averageAxis, averageAngle);
	computeAverage = FALSE;
    }
    
    //
    // rotate camera by average rotation
    //
    rotateCamera(averageRotation);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Show/hide the pref sheet feedback size wheel and label.
//
// Use: private

void
MyExaminerViewer::toggleFeedbackWheelSize(Widget toggle)
//
////////////////////////////////////////////////////////////////////////
{
 //   if ( feedbackFlag ) {
	//Widget parent = XtParent(toggle);
	//Arg args[12];
	//int n;
	//
	//// create the label/thumb/text/label in the toggle parent
	//feedbackLabel[0] = XtCreateWidget("axes size:", 
	//    xmLabelGadgetClass, parent, NULL, 0);
	//
	//n = 0;
	//XtSetArg(args[n], XmNvalue, 0); n++;
	//XtSetArg(args[n], (char*)SgNangleRange, 0); n++;
	//XtSetArg(args[n], (char*)SgNunitsPerRotation, 360); n++;
	//XtSetArg(args[n], (char*)SgNshowHomeButton, FALSE); n++;
	//XtSetArg(args[n], XmNhighlightThickness, 0); n++;
	//XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
	//feedbackSizeWheel = SgCreateThumbWheel(parent, NULL, args, n);
	//
	//XtAddCallback(feedbackSizeWheel, XmNvalueChangedCallback, 
	//    (XtCallbackProc) MyExaminerViewer::feedbackSizeWheelCB, (XtPointer) this);
	//XtAddCallback(feedbackSizeWheel, XmNdragCallback, 
	//    (XtCallbackProc) MyExaminerViewer::feedbackSizeWheelCB, (XtPointer) this);
	//feedbackSizeWheelVal = 0;
	//
	//n = 0;
	//char str[15];
	//sprintf(str, "%d", int(feedbackSize));
	//XtSetArg(args[n], XmNvalue, str); n++;
	//XtSetArg(args[n], XmNhighlightThickness, 1); n++;
	//XtSetArg(args[n], XmNcolumns, 3); n++;
	//feedbackField = XtCreateWidget("", xmTextWidgetClass, 
	//    parent, args, n);
	//
	//XtAddCallback(feedbackField, XmNactivateCallback, 
	//    (XtCallbackProc) MyExaminerViewer::feedbackSizeFieldCB,
	//    (XtPointer) this);
	//
	//feedbackLabel[1] = XtCreateWidget("pixels", 
	//    xmLabelGadgetClass, parent, NULL, 0);
	//
	//// layout
	//n = 0;
	//XtSetArg(args[n], XmNleftAttachment,	XmATTACH_FORM); n++;
	//XtSetArg(args[n], XmNleftOffset,	20); n++;
	//XtSetArg(args[n], XmNtopAttachment,	XmATTACH_WIDGET); n++;
	//XtSetArg(args[n], XmNtopWidget,		toggle); n++;
	//XtSetArg(args[n], XmNtopOffset,		5); n++;
	//XtSetValues(feedbackLabel[0], args, n);
	//
	//n = 0;
	//XtSetArg(args[n], XmNleftAttachment,	XmATTACH_WIDGET); n++;
	//XtSetArg(args[n], XmNleftWidget,	feedbackLabel[0]); n++;
	//XtSetArg(args[n], XmNleftOffset,	5); n++;
	//XtSetArg(args[n], XmNtopAttachment,	XmATTACH_OPPOSITE_WIDGET); n++;
	//XtSetArg(args[n], XmNtopWidget,		feedbackLabel[0]); n++;
	//XtSetValues(feedbackSizeWheel, args, n);
	//
	//n = 0;
	//XtSetArg(args[n], XmNleftAttachment,	XmATTACH_WIDGET); n++;
	//XtSetArg(args[n], XmNleftWidget,	feedbackSizeWheel); n++;
	//XtSetArg(args[n], XmNleftOffset,	3); n++;
	//XtSetArg(args[n], XmNtopAttachment,	XmATTACH_OPPOSITE_WIDGET); n++;
	//XtSetArg(args[n], XmNtopWidget,		feedbackSizeWheel); n++;
	//XtSetArg(args[n], XmNtopOffset,		-5); n++;
	//XtSetValues(feedbackField, args, n);
	//
	//n = 0;
	//XtSetArg(args[n], XmNleftAttachment,	XmATTACH_WIDGET); n++;
	//XtSetArg(args[n], XmNleftWidget,	feedbackField); n++;
	//XtSetArg(args[n], XmNleftOffset,	5); n++;
	//XtSetArg(args[n], XmNbottomAttachment,	XmATTACH_OPPOSITE_WIDGET); n++;
	//XtSetArg(args[n], XmNbottomWidget,	feedbackLabel[0]); n++;
	//XtSetValues(feedbackLabel[1], args, n);
	//
	//// manage children
	//XtManageChild(feedbackLabel[0]);
	//XtManageChild(feedbackSizeWheel);
	//XtManageChild(feedbackField);
	//XtManageChild(feedbackLabel[1]);
 //   }
 //   else {
	//// destroys the widgets
	//XtDestroyWidget(feedbackLabel[1]);
	//XtDestroyWidget(feedbackField);
	//XtDestroyWidget(feedbackSizeWheel);
	//XtDestroyWidget(feedbackLabel[0]);
 //   }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Build the parent class widget, then register this widget.
//
// Use: protected
Widget
MyExaminerViewer::buildWidget(Widget parent)
//
////////////////////////////////////////////////////////////////////////
{
    // Create the root widget and register it with a class name
    Widget w = MyFullViewer::buildWidget(parent);
    
    // If first build, get resource values
    if (firstBuild) {
	// Full viewer registered the widget for us
	SoXtResource xr(w);
	SbBool flag;
	short val;
	
	if (xr.getResource((char *)"spinAnimation", (char *)"SpinAnimation", flag))
	    setAnimationEnabled(flag);
	if (xr.getResource((char *)"pointOfRotationAxes", (char *)"PointOfRotationAxes", flag))
	    setFeedbackVisibility(flag);
	if (xr.getResource((char *)"axesSize", (char *)"AxesSize", val))
	    feedbackSize = val;
	
	firstBuild = FALSE;
    }
    
    return w;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	add our own button to the existing list
//
// Use: virtual protected
void
MyExaminerViewer::createViewerButtons(Widget parent)
//
////////////////////////////////////////////////////////////////////////
{
 //   // get the default buttons
 //   MyFullViewer::createViewerButtons(parent);
 //   
 //   // allocate our buttons
 //   buttonList[CAM_PUSH] = new MyBitmapButton(parent, FALSE);
 //   buttonList[CAM_PUSH]->setIcon(so_xt_persp_bits, so_xt_icon_width, so_xt_icon_height);
 //   Widget w = buttonList[CAM_PUSH]->getWidget();
 //   XtAddCallback(w, XmNactivateCallback,
	//(XtCallbackProc) MyExaminerViewer::camPushCB, (XtPointer) this);
 //   
 //   // add this button to the list...
 //   viewerButtonWidgets->append(w);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	read the point of interest geometry, which will be placed
//  right after the camera node (or right after the headlight
//  if the headlight is turned on).
//
// Use: private
void
MyExaminerViewer::createFeedbackNodes()
//
////////////////////////////////////////////////////////////////////////
{
    // make sure we havn't built this yet...
    if (feedbackRoot)
	return;
    
    feedbackRoot	= new SoSeparator(1);
    feedbackSwitch	= new SoSwitch(3);
    feedbackTransNode	= new SoTranslation;
    feedbackScaleNode	= new SoScale;
    feedbackRoot->ref();
    feedbackRoot->addChild( feedbackSwitch );
    feedbackSwitch->addChild( feedbackTransNode );
    feedbackSwitch->addChild( feedbackScaleNode );
    SoInput in;
    in.setBuffer((void *)geometryBuffer, (size_t) strlen(geometryBuffer));
    SoNode *node;
    SbBool ok = SoDB::read(&in, node);
    if (ok && node != NULL)
	feedbackSwitch->addChild(node);
#if defined(_DEBUG)
    else
	SoDebugError::post("MyExaminerViewer::createFeedbackNodes",
			    "couldn't read feedback axis geometry");
#endif
}

//
// redefine those generic virtual functions
//
SbString
MyExaminerViewer::getDefaultWidgetName() const
{ return thisClassName; }

SbString
MyExaminerViewer::getDefaultTitle() const
{ return "Examiner Viewer"; }

SbString
MyExaminerViewer::getDefaultIconTitle() const
{ return "Examiner Viewer"; }



//
////////////////////////////////////////////////////////////////////////
// static callbacks stubs
////////////////////////////////////////////////////////////////////////
//

void
MyExaminerViewer::camPushCB(Widget, MyExaminerViewer *v, void *)
{ v->toggleCameraType(); }


void
MyExaminerViewer::animationSensorCB(void *v, SoSensor *)
{ ((MyExaminerViewer *) v)->doSpinAnimation(); }

void
MyExaminerViewer::animPrefSheetToggleCB(Widget toggle, 
    MyExaminerViewer *v, void *)
{
    v->setAnimationEnabled( XmToggleButtonGetState(toggle) );
}

void
MyExaminerViewer::feedbackPrefSheetToggleCB(Widget toggle, 
    MyExaminerViewer *v, void *)
{
    // show/hide the feedback
    v->setFeedbackVisibility( XmToggleButtonGetState(toggle) );
    
    // show/hide the extra size setting
    v->toggleFeedbackWheelSize(toggle);
}

void
MyExaminerViewer::feedbackSizeWheelCB(Widget, MyExaminerViewer *v, XtPointer *d)
{
 //   static SbBool firstDrag = TRUE;
 //   SgThumbWheelCallbackStruct *data = (SgThumbWheelCallbackStruct *) d;
 //   
 //   if (data->reason == XmCR_DRAG) {
	//// for the first move, invoke the start callbacks
	//if (firstDrag) {
	//    v->interactiveCountInc();
	//    firstDrag = FALSE;
	//}
	//
	//// grow/shrink the feedback based on the wheel rotation
	//v->feedbackSize *= (float)pow(12.0, (data->value - v->feedbackSizeWheelVal) / 360.0);
	//v->feedbackSizeWheelVal = data->value;
	//
	//// update the text field
	//char str[15];
	//sprintf(str, "%d", int(v->feedbackSize));
	//XmTextSetString(v->feedbackField, str);
	//
	//// redraw since the wheel size isn't a field in the scene
	//if (v->isViewing())
	//    v->scheduleRedraw();
 //   }
 //   else {
	//// reason = XmCR_VALUE_CHANGED, invoke the finish callbacks
	//v->interactiveCountDec();
	//firstDrag = TRUE;
 //   }
}

void
MyExaminerViewer::feedbackSizeFieldCB(Widget field, MyExaminerViewer *v, void *)
{
 //   // get text value from the field
 //   char *str = XmTextGetString(field);
 //   int val;
 //   if ( sscanf(str, "%d", &val) && val > 0)
	//v->setFeedbackSize(val);
 //   else
	//val = int(v->feedbackSize);
 //   free(str);
 //   
 //   // reformat text field
 //   char valStr[15];
 //   sprintf(valStr, "%d", val);
 //   XmTextSetString(field, valStr);
 //   
 //   // make the text field loose the focus
 //   XmProcessTraversal(SoXt::getShellWidget(field), XmTRAVERSE_CURRENT);
}

// called when the viewer becomes visible/hidden - when hidden, make
// sure to temporary stop any ongoing animation (and restart it as soon
// as we become visible).
//
void
MyExaminerViewer::visibilityChangeCB(void *pt, SbBool visible)
{
    MyExaminerViewer *p = (MyExaminerViewer *)pt;
    
    // only do this if we are/were spinning....
    if (! p->animatingFlag)
	return;
    
    if (visible) {
	// we now are visible again so attach the field sensor
	p->animationSensor->attach(viewerRealTime);
    }
    else {
	// if hidden, detach the field sensor, but don't change the
	// animatingFlag var to let us know we need to turn it back on
	// when we become visible....
	p->animationSensor->detach();
	p->animationSensor->unschedule();
    }
}
