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
 * Copyright (C) 1992   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |
 |   Classes:
 |	MyBitmapButton
 |
 |   Author(s)	: Alain Dumesny
 |
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <inttypes.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>

#include "MyBitmapButton.h"

////////////////////////////////////////////////////////////////////////
//
//  Constructor
//
MyBitmapButton::MyBitmapButton(Widget parent, SbBool canSelect)
//
////////////////////////////////////////////////////////////////////////
{
    selectFlag = FALSE;
    selectable = canSelect;
    normalPixmap = selectPixmap = 0;
    
    // Create the push button
    Arg args[8];
    int n = 0;
    XtSetArg(args[n], XmNmarginHeight, 0); n++;
    XtSetArg(args[n], XmNmarginWidth, 0); n++;
    XtSetArg(args[n], XmNshadowThickness, 2); n++;
    XtSetArg(args[n], XmNhighlightThickness, 0); n++;
    widget = XmCreatePushButtonGadget(parent, NULL, args, n);
}

////////////////////////////////////////////////////////////////////////
//
//  Destructor
//
MyBitmapButton::~MyBitmapButton()
//
////////////////////////////////////////////////////////////////////////
{
    //??? destroy the widget?
}

////////////////////////////////////////////////////////////////////////
//
//  Highlight the pixmap button.
//
//  Usage: public
//
void
MyBitmapButton::select(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (selectFlag == flag || ! selectable)
	return;
    
    selectFlag = flag;
    
    XtVaSetValues(widget, XmNlabelPixmap, 
	selectFlag ? selectPixmap : normalPixmap, NULL);
}

////////////////////////////////////////////////////////////////////////
//
//  This routine builds the pixmaps (label pixmap and arm pixmap).
//
//  Usage: public
//
void
MyBitmapButton::setIcon(char *icon, int width, int height)
//
////////////////////////////////////////////////////////////////////////
{
    Display	*display = XtDisplay(XtParent(widget));
    Drawable	d = DefaultRootWindow(display);
    int		depth = XDefaultDepthOfScreen(XtScreen(XtParent(widget)));
    Pixel	fg, bg, hl;
    
    // get the color of the push buttons
    // ??? the foreground and background color have to be
    // ??? taken from the parent widget because we are using
    // ??? Gadget push buttons (not Widget push buttons)
    Arg args[8];
    int n = 0;
    XtSetArg(args[n], XmNforeground, &fg); n++;
    XtSetArg(args[n], XmNbackground, &bg); n++;
    XtSetArg(args[n], XmNtopShadowColor, &hl); n++; //??? highlight color
    XtGetValues(XtParent(widget), args, n);
    
    // free the old pixmaps
    if (normalPixmap)
	XFreePixmap(display, normalPixmap);
    if (selectPixmap)
	XFreePixmap(display, selectPixmap);
    normalPixmap = selectPixmap = 0;
    
    // create the pixmaps from the bitmap data (depth 1).
    normalPixmap = XCreatePixmapFromBitmapData(display, d, 
	icon, width, height, fg, bg, depth);
    if (selectable)
	selectPixmap = XCreatePixmapFromBitmapData(display, d, 
	    icon, width, height, fg, hl, depth);
    
    // now assign the pixmaps
    n = 0;
    XtSetArg(args[n], XmNlabelType, XmPIXMAP); n++;
    XtSetArg(args[n], XmNlabelPixmap, 
	selectFlag ? selectPixmap : normalPixmap); n++;
    XtSetValues(widget, args, n);
}

