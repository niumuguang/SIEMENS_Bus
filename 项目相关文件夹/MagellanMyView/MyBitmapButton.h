/*   THIS SOURCE IS PROPRIETARY TO MERCURY COMPUTER SYSTEMS S.A.,  ***
***  (MCS S.A.), AND IS DISTRIBUTED  UNDER A LICENSE AGREEMENT.    ***
***  REPRODUCTION, DISCLOSURE, OR USE, IN WHOLE OR IN PART, OTHER  ***
***  THAN  AS  SPECIFIED  IN THE LICENSE ARE NOT TO BE UNDERTAKEN  ***
***  EXCEPT WITH PRIOR WRITTEN AUTHORIZATION OF MCS S.A.           ***
***                    RESTRICTED RIGHTS LEGEND                    ***
***  USE,  DUPLICATION,  OR  DISCLOSURE  BY  THE  GOVERNMENT   OF  ***
***  SOFTWARE OR RELATED DOCUMENTATION IS SUBJECT TO RESTRICTIONS  ***
***  AS SET  FORTH  IN  SUBPARAGRAPH  (C)(1)  OF  THE  COMMERCIAL  ***
***  COMPUTER  SOFTWARE  RESTRICTED RIGHT CLAUSE AT FAR 52.227-19  ***
***  OR SUBPARAGRAPH (C)(1)(II) OF THE RIGHTS IN  TECHNICAL  DATA  ***
***  AND COMPUTER SOFTWARE CLAUSE AT DFARS 52.227-7013.            ***
***      COPYRIGHT (C) 2004 BY MERCURY COMPUTER SYSTEMS S.A.,      ***
***                      MERIGNAC, FRANCE                          ***
***                     ALL RIGHTS RESERVED                        **/

#ifndef _SO_WIN_DEF_
#define _SO_WIN_DEF_

#include <Inventor/SbBasic.h>

#ifdef _WIN32
#  include <Inventor/Win/SoWinBeginStrict.h>
#  include <windows.h>
#endif
// Redefine X data types in _WIN32 terms
#define ArgList  PLONG

#define Boolean  BOOLEAN

#define WBOOL   int

#define Colormap UINT

#define Cursor   Hcursor

#define WindowPtr HWND

// Stuff for SoWinMouse/Keyboard -- NOTYET: make constants match X
#define EventMask UINT
#define   ButtonPressMask   0x01
#define   ButtonReleaseMask 0x02
#define   PointerMotionMask 0x04
#define   ButtonMotionMask  0x08
#define   KeyPressMask      0x10
#define   KeyReleaseMask    0x20

#define GLXContext HGLRC

#define KeySym UINT

#ifndef True
#define True  TRUE
#endif

#ifndef False 
#define False FALSE
#endif

//#if defined(_AFX)
//#define Widget  PVOID
//#define Window  PVOID
//#define Hinst   PVOID
//#define Hwnd    PVOID
//#define Hdc     PVOID
//#define Hcursor PVOID
//#define Hmenu   PVOID
//#define Hrsrc   PVOID
//#else
#ifndef Widget
#  define Widget  HWND
#endif
#define Window  HWND
#define Hinst   HINSTANCE
#define Hwnd    HWND
#define Hdc     HDC
#define Hcursor HCURSOR
#define Hmenu   HMENU
#define Hrsrc   HRSRC
//#endif

#define Region HRGN
#define Pixmap HBITMAP

#define Atom ULONG
#define Time time_t

#define XColor UINT

// Event types
#define XEvent       MSG
#ifndef XAnyEvent
#  define XAnyEvent    MSG
#endif
#define XButtonEvent MSG
#define XKeyEvent    MSG
#define XMotionEvent MSG
#define XMouseWheelEvent MSG

// Key codes (basically the same but with different names)
#define XK_ESCAPE   VK_ESCAPE
#define XK_HOME     VK_HOME
#define XK_LEFT     VK_LEFT
#define XK_UP       VK_UP
#define XK_RIGHT    VK_RIGHT
#define XK_DOWN     VK_DOWN
#define XK_s        83

// Redefine Xt and Motif data types in _WIN32 terms
#define XmString char*

#define XtAppContext UINT

#define XtEventHandler FARPROC

#define XtIntervalId UINT

#define XtPointer PVOID

#define XmAnyCallbackStruct void 

#define XtWorkProcId UINT

// XVisualInfo corresponds to PIXELFORMATDESCRIPTOR rather than
// "pixel format" because under Windows, pixel formats are per
// drawable rather than per display (like visuals under X).  In
// other words, under Windows we can't get the description of a
// pixel format unless we have a valid device context and that
// is often inconvenient.
//
// Note that PIXELFORMATDESCRIPTOR also takes the place of GLX's
// "attribList" (which it is conceptually closer to).   --mmh

#define XVisualInfo PIXELFORMATDESCRIPTOR


////////////////////////////////////////////////////////////////////////////
// Functions to implement SoWin on Win32 
//
// Supplying these dummy functions is less ugly than ifdef'ing
// the SoWin code in dozens of places.

INVENTORW_API extern XtAppContext XtWidgetToApplicationContext( Widget w );

INVENTORW_API extern UINT *XtDisplay( Widget w );

INVENTORW_API extern UINT XtScreen( Widget w ) ;

INVENTORW_API extern Window RootWindowOfScreen(UINT s) ;

INVENTORW_API extern Boolean XtIsShell( Widget w );

INVENTORW_API extern Boolean XtIsRealized( Widget w );

INVENTORW_API extern Widget XtParent( Widget w );

INVENTORW_API extern Window XtWindow( Widget w );

INVENTORW_API extern Widget XtWindowToWidget( UINT *d, Window w );


////////////////////////////////////////////////////////////////////////////
// Miscellaneous SGI functions not found in _WIN32

#include "tchar.h"

#undef strdup
#define strdup _tcsdup

#ifdef _WIN32
#include <Inventor/Win/SoWinEndStrict.h>
#endif //_WIN32
#endif //_SO_WIN_DEF_
//////////////////////////////////////////////////////////////////////////
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

//#include <X11/Intrinsic.h>
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
