
/*
 * Copyright (C) 1992   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |
 |   Class:
 |	MyBitmapButton - bitmap button convenience wrapper for the viewers
 |
 |   Description:
 |	A Motif push button gadget with a bitmap displayed on it.
 |	No border for the button is shown and the button can be selectable.
 |
 |   Author(s): Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_PIXMAP_BUTTON_
#define _SO_PIXMAP_BUTTON_

#include <X11/Intrinsic.h>
#include <Inventor/SbBasic.h>

class MyBitmapButton {
  public:
    MyBitmapButton(Widget parent, SbBool selectable);
    ~MyBitmapButton();
    
    // return the motif push button
    Widget	getWidget()	    { return widget; }
    
    // set the icon to use for the pixmap
    void	setIcon(char *icon, int width, int height);
    
    // Highlight the pixmap to show it it selected (must pass TRUE
    // to the constructor, in which case another pixmap with a highlight
    // color will be created for the button).
    void	select(SbBool onOrOff);
    SbBool	isSelected()	    { return selectFlag; }
    
  private:
    Widget	widget;
    SbBool	selectFlag, selectable;
    Pixmap	normalPixmap, selectPixmap;
};

#endif // _SO_PIXMAP_BUTTON_
