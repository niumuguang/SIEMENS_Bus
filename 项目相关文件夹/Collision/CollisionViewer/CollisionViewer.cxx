/*****************************************************************-*-c++-*-*****
 * File: Viewer.cxx
 * Author(s) : Fabien ARNAUD
 * Date: 07/03/97
 * Revision: 1.0
 *==============================================================================
 * (c) Copyright 1997 G5G, all rights reserved
 *==============================================================================
 *
 * Purpose : This program is an example of the use of the SoXtCollisionViewer
 *           class. It allow the user to walk over a scene, preventing him to
 *           collide with any part of that scene.
 *
 ******************************************************************************/

//------------------------------------------------------------------------------
// Inventor declarations
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtCollisionViewer.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#define MAC_CONSOLE
#include <Inventor/SoMacApp.h>

//------------------------------------------------------------------------------
// Global objects
SoXtCollisionViewer *m_collision_viewer;

//------------------------------------------------------------------------------
// Functions declarations
//void onControlChange (void *, PoXtDialog::PoXtDialogDataCB *);
void onCollision(void *, SoXtCollisionViewer *);

//******************************************************************************
// Main function

int
main(int argc, char **argv)
{
  // Window initialisation -----------------------------------------------------
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL) exit(1);

  // Which file to open --------------------------------------------------------
  char filename [1024];
  if (argc == 1)
    strcpy (filename, "../../../data/Barcelona.iv");
  else
    strcpy (filename, argv[1]);

  // Scene graph initialisation ------------------------------------------------
  SoInput my_scene_input;
  if (!my_scene_input.openFile(filename)) 
    exit(-1);
  SoSeparator *root = SoDB::readAll(&my_scene_input);
  if (root == NULL)
    exit(-1);
  my_scene_input.closeFile ();

  root->ref();

  // Set up viewer -------------------------------------------------------------
  SoXtWalkViewer *my_viewer = new SoXtWalkViewer(myWindow);
  my_viewer->setSceneGraph(root);
  my_viewer->setTitle("Collision Walk Viewer");
  my_viewer->show();

  // Adding collision prevention -----------------------------------------------
  m_collision_viewer = new SoXtCollisionViewer(my_viewer);
  m_collision_viewer->addCallback (onCollision, NULL);

  // Main loop -----------------------------------------------------------------
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete my_viewer;
  delete m_collision_viewer;
  SoXt::finish();

  return 0;
}

//******************************************************************************
// Function called when there is a collision
void
onCollision(void *, SoXtCollisionViewer *)
{
#ifdef _WIN32
  PlaySound ("Collision.wav", NULL, SND_FILENAME + SND_ASYNC);
#else 
  printf ("%c", 7);
  fflush (stdout);
#endif
}
