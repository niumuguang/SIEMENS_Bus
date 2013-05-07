#include <Inventor/Xt/SoXt.h>

#include <DialogViz/SoDialogVizAll.h>

#include <Inventor/SoWinApp.h>

#include "DemoCollider.h"
#include "DemoColliderDialog.h"

/////////////////////////////////////////////////////////////
int
main(int, char **argv)
{
  // Initialize Inventor and Xt
  Widget myWindow = SoXt::init(argv[0]);
  
  SoDialogViz::init();
  DemoColliderDialog dialog(myWindow);

  DemoCollider collider(1000,100);
  dialog.init(&collider);

  // Loop forever
  SoXt::show(myWindow);
  SoXt::mainLoop();
  return 0;
}

