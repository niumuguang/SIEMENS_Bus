
#ifdef _WIN32
#  include <windows.h>
#else
#  include <X11/Intrinsic.h>
#endif

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/Xt/devices/SoXtSpaceball.h>
#include "Inventor/events/SoSpaceballButtonEvent.h"
#include "Inventor/events/SoMotion3Event.h"

#ifdef _WIN32
#  define SoXtSpaceball SoWinSpaceball
#endif

SoXtSpaceball *sb;

SbBool rotationMode;
SbBool translationMode;
float rotScaleFactor;
float transScaleFactor;
int stereoMode;
SoXtExaminerViewer *vwr;

void
spaceballButtonCB(void *, SoEventCallback *cb)
{
  const SoSpaceballButtonEvent *ev = 
    (const SoSpaceballButtonEvent *) cb->getEvent();
  
  if (ev->getState() == SoButtonEvent::DOWN) {
    int which = ev->getButton();
    printf(" button %d\n",which);
    switch (which) {
    case 1:
      printf("rotation only mode \n");
      rotationMode    = 1;
      translationMode = 0;
      break;
    case 2:
      printf("translation only mode \n");
      translationMode = 1;
      rotationMode    = 0;
      break;
    case 3:
      printf("translation and rotation mode \n");
      translationMode = 1;
      rotationMode    = 1;
      break;
    case 4:
      rotScaleFactor *= 2.;
      printf("Increasing Rotation Scale Factor : %f\n",rotScaleFactor);
      sb->setRotationScaleFactor(rotScaleFactor);
      break;
    case 5:
      rotScaleFactor /= 2.;
      printf("Decreasing Rotation Scale Factor : %f\n",rotScaleFactor);
      sb->setRotationScaleFactor(rotScaleFactor);
      break;
    case 6:
      transScaleFactor *= 2.;
      printf("Increasing Translation Scale Factor: %f\n",transScaleFactor);
      sb->setTranslationScaleFactor(transScaleFactor);
      break;
    case 7:
      transScaleFactor /= 2;
      printf("Decreasing Translation Scale Factor: %f\n",transScaleFactor);
      sb->setTranslationScaleFactor(transScaleFactor);
      break;
    case 8:
      stereoMode = 1-stereoMode;
      printf("Stereo mode %d\n",stereoMode);
      vwr->setStereoViewing(stereoMode);
      break;
    default: 
      break;
    }
  }
}

void
motion3TranslationCB(void *userData, SoEventCallback *cb)
{
  if (!translationMode) return;
  
  const SoMotion3Event *ev = 
    (const SoMotion3Event *) cb->getEvent();
  SoTranslation *translation = (SoTranslation *) userData;
  
  
  translation->translation = translation->translation.getValue() + ev->getTranslation();
  cb->setHandled();
}   

void
motion3RotationCB(void *userData, SoEventCallback *cb)
{
  if (!rotationMode) return;
  
  const SoMotion3Event *ev = 
    (const SoMotion3Event *) cb->getEvent();
  
  SoRotation *rotation = (SoRotation *) userData;
  
  rotation->rotation = rotation->rotation.getValue() * ev->getRotation();
  cb->setHandled();
}   

SoNode *
buildSceneGraph(int argc, char *argv[])
{
  SoSeparator *sep = new SoSeparator;
  sep->ref();
  SoEventCallback *cb = new SoEventCallback;
  SoRotation *rotation = new SoRotation;
  SoTranslation *translation = new SoTranslation;
  
  SoInput input;
  if (!input.openFile("axis.iv")) {
    printf("Cannot open file axis.iv \n");
    exit(-1);
  }
  SoSeparator *axis = SoDB::readAll(&input);
  axis->ref();
  //SoSeparator *axis = new SoSeparator;
  
  if (argc == 2) {
    if (!input.openFile(argv[1])) {
      printf("Cannot open file %s \n",argv[1]); 
    }
  }
  SoSeparator *scene = SoDB::readAll(&input);
  scene->ref();
  
  sep->addChild(translation);
  sep->addChild(rotation);
  sep->addChild(cb);
  
  sep->addChild(axis);
  sep->addChild(scene);
  
  // Set up event callbacks
  cb->addEventCallback(
    SoSpaceballButtonEvent::getClassTypeId(),
    spaceballButtonCB, 
    sep);
  cb->addEventCallback(
    SoMotion3Event::getClassTypeId(),
    motion3TranslationCB, 
    translation);
  cb->addEventCallback(
    SoMotion3Event::getClassTypeId(),
    motion3RotationCB, 
    rotation);
  
  return sep;
}

int
main(int argc, char **argv)
{
  Widget mainWindow = SoXt::init(argv[0]);
  
  vwr = new SoXtExaminerViewer(mainWindow);
  vwr->setSceneGraph(buildSceneGraph(argc,argv));
  vwr->setTitle("Space Ball and Space Mouse Device");
  vwr->setViewing(FALSE);   // come up in pick mode
  vwr->setHeadlight(TRUE); 
  
  
  if (! SoXtSpaceball::exists()) {
    fprintf(stderr, "Sorry, no Space Ball or Magellan Space Mouse on this display!\n");
  } 
  else {
    sb = new SoXtSpaceball;
    vwr->registerDevice(sb);
    rotScaleFactor   = sb->getRotationScaleFactor();
    transScaleFactor = sb->getTranslationScaleFactor();
    
    printf("Default rotation scale factor %f\n",rotScaleFactor);
    printf("Default translation scale factor %f\n",transScaleFactor);
  }
  stereoMode = 0;
  
  rotationMode    = 1;
  translationMode = 0;
  
  vwr->show();
  SoXt::show(mainWindow);
  SoXt::mainLoop();
 
  vwr->getSceneGraph()->unref();
  delete vwr;
  SoXt::finish();

  return 0;
}
