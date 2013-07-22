// Activate red/cyan stereo from application program
//
// Copyright (C) 2001 TGS, Inc.
//
// Original: mmh
// Modified: 
//
// Notes:
//   On MS-Windows, build as a console app.

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <Inventor/components/stereo/SoBaseStereo.h>
#include <Inventor/components/stereo/SoAnaglyphStereo.h>

/////////////////////////////////////////////////////////////

void 
toggleRedGreenStereo(SoXtViewer *pViewer)
{
  static SoAnaglyphStereo *pAnaStereo = NULL;

  if (pAnaStereo == NULL) {
    // Create anaglyph stereo implementation
    pAnaStereo = new SoAnaglyphStereo();

    // Tell it to use red/green (actually the default)
    pAnaStereo->setColorFilter(SoAnaglyphStereo::RED_CYAN);

    // Default stereo offset is a little too big...
    pViewer->setStereoOffset(1.0);
  }
  else
    pAnaStereo = (SoAnaglyphStereo*)pViewer->getStereoViewType();


  // Tell the viewer to use this stereo implementation
  if (pViewer->getStereoViewType() != pAnaStereo && pAnaStereo)
    pViewer->setStereoViewType(pAnaStereo);

  // Toggle stereo on/off
  SbBool isActive = pViewer->isStereoActive();
  pViewer->setStereoActive(!isActive);
}

/////////////////////////////////////////////////////////////

void 
EventCallback(void *userData, SoEventCallback *node)
{
  const SoEvent *event = node->getEvent();
  if (SO_KEY_PRESS_EVENT(event, R)) {
    SoXtViewer *pViewer = (SoXtViewer*)userData;
    toggleRedGreenStereo(pViewer);
  }
}

/////////////////////////////////////////////////////////////

#ifdef _WIN32
SbBool EventCB(void *userData, MSG *msg)
{
  if (msg->message == WM_KEYDOWN) {
    int key = (int)(msg->wParam);
    if (key == 'R') {
      SoXtViewer *pViewer = (SoXtViewer*)userData;
      toggleRedGreenStereo(pViewer);
      return TRUE;    // This event has been handled
    }
  }
  return FALSE;
}
#endif //_WIN32

/////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
  // Initialize Inventor
  Widget myWindow = SoXt::init(argv[0]);

    // Create a simple scene graph
  SoSeparator *pRoot = new SoSeparator;
  pRoot->ref();
  pRoot->addChild(new SoCube);

  // Create and initialize viewer
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(pRoot);
  myViewer->setTitle("Press R to enable/disable the Red/Cyan stereo");
  toggleRedGreenStereo(myViewer);
  myViewer->show();

    // Create and initialize event callback node for keypress events.
    // Have to do this here (instead of above) so we can pass the
    // address of the viewer to the callback function.
    //
    // Note this callback will only be called when the viewer is in
    // selection mode, not in viewing mode!
  SoEventCallback *pEvCB = new SoEventCallback;
  pEvCB->addEventCallback(SoKeyboardEvent::getClassTypeId(),
			  EventCallback, (void*)myViewer);
  pRoot->insertChild(pEvCB, 0);
#ifdef _WIN32
  // This callback does not depend on the viewer mode
  myViewer->setEventCallback(EventCB, (void*)myViewer);
#endif

  // Loop forever
  SoXt::show(myWindow);
  SoXt::mainLoop();

  pRoot->unref();
  delete myViewer;
  SoXt::finish();
  return 0;
}
