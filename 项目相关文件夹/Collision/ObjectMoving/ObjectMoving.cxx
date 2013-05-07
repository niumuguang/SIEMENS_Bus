/*****************************************************************-*-c++-*-*****
 * File: ObjectMoving.cxx
 * Author(s) : Fabien ARNAUD
 * Date: 07/03/97
 * Revision: 1.0
 *==============================================================================
 * (c) Copyright 1997 G5G, all rights reserved
 *==============================================================================
 *
 * Purpose : This program is an example of the use of the SoCollisionManager
 *           class. It allows the user to manipulate an object in a scene and
 *           prevents him to put this object on another.
 *
 ******************************************************************************/

//------------------------------------------------------------------------------
// Scenes and Objects
#define D_NUM_SCENES 2
const char* scenes_title[D_NUM_SCENES] = {
  "box.iv",
  "Barcelona.iv"};

#define D_NUM_OBJECTS 8
const char* objects_title[D_NUM_OBJECTS] = {
  "cube.iv",
  "canstick.iv",
  "ceilingLamp.iv",
  "chair.iv",
  "martini.iv",
  "plant.iv",
  "table.iv",
  "trackLights.iv"};

//------------------------------------------------------------------------------
// Inventor declarations
#include <Inventor/collision/SoCollisionManager.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/actions/SoWriteAction.h>

//------------------------------------------------------------------------------
// Control dialog declarations
#include <Inventor/dialog/PoXtDialog.h>
#include <Inventor/dialog/PoXtToggleButtonData.h>
#include <Inventor/dialog/PoXtChoiceButtonData.h>
#include <Inventor/dialog/PoXtTriggerButtonData.h>
#include <Inventor/dialog/PoXtIntSliderData.h>
#include <Inventor/dialog/PoXtLabelData.h>

#define D_NUM_CONTROLS     10
#define D_MANIPULATOR       0
#define D_MANAGER_ON        1
#define D_SCENE_SELECTION   2
#define D_SCENE_BBOX_ONLY   3
#define D_OBJECT_SELECTION  4
#define D_OBJECT_BBOX_ONLY  5
#define D_GLUING_ON         6
#define D_COPY_TO_SCENE     7
#define D_SAVE_SCENE        8
#define D_GLUING_LEVEL      9

//------------------------------------------------------------------------------
// Global objects
SoSeparator*        m_root;
SoSeparator*        m_scene;
SoSeparator*        m_object;
SoTransform*        m_object_transform;
SoTransformManip*   m_manip;
SoPath*             m_transform_path;
SoSwitch*           m_switch_material;
SoCollisionManager* m_collision_manager;

//------------------------------------------------------------------------------
// Functions declarations
void onControlChange(void*, PoXtDialog::PoXtDialogDataCB *data);
void flashObject();
void unflashObject(void*, SoSensor*);
SoSeparator* readIvFile(const char* filename);
SoCollisionManager::Resp onCollision(void*, 
				     const SoCollidingPrimitive*, 
				     const SoCollidingPrimitive*);

//******************************************************************************
// Main function
int
main(int, char **argv)
{
  // Windows initialisation ----------------------------------------------------
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL)
    exit(1);

  // Scene graph initialisation ------------------------------------------------
  m_root = new SoSeparator;
  m_root->setName("m_root");
  m_root->ref();
 
  m_scene = new SoSeparator;
  m_scene->setName("m_scene");
  m_root->addChild(m_scene);

  char filename [1024];
  sprintf(filename, "Scenes/%s", scenes_title [0]);
  SoSeparator* sep = readIvFile(filename);
  m_scene->addChild(sep);

  m_object_transform = new SoTransform;
  m_object_transform->setName("m_object_transform");
  m_object_transform->translation.setValue(10, 0, -10);
  m_object_transform->ref();
  m_root->addChild(m_object_transform);

  m_switch_material = new SoSwitch;
  m_switch_material->setName("m_switch_material");
  m_switch_material->whichChild.setValue(SO_SWITCH_NONE);
  m_root->addChild(m_switch_material);

  SoMaterial* material = new SoMaterial;
  material->setName("material");
  material->diffuseColor.setValue(1, 0, 0);
  material->setOverride(TRUE);
  m_switch_material->addChild(material);

  m_object = new SoSeparator;
  m_object->setName("m_object");
  m_root->addChild(m_object);

  sprintf(filename, "Objects/%s", objects_title [0]);
  sep = readIvFile(filename);
  m_object->addChild(sep);

  // Set up the manipulator and the path to the transformation -----------------
  m_manip = NULL;
  m_transform_path = new SoPath(m_root);
  m_transform_path->append(m_object_transform);
  m_transform_path->ref();

  // Set up viewer -------------------------------------------------------------
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(m_root);
  myViewer->setTitle("Object Moving");
  myViewer->show();

  // Set up the collision manager ----------------------------------------------
  SoPath* object_path = new SoPath(m_root);
  object_path->append(m_object);
  object_path->ref();
  m_collision_manager = new SoCollisionManager
    (object_path, m_root, m_object_transform);
#ifdef __osf__
  m_collision_manager->addCollisionCallback((SoCollisionManager::SoCollisionCB *)onCollision);
#else
  m_collision_manager->addCollisionCallback(onCollision);
#endif

  // Setup the control dialog --------------------------------------------------
  PoXtElementData *tab[D_NUM_CONTROLS];
  int ind = 0;
 
  const char* items[7] = {"None", "Center Ball", "Handle Box",  "Jack",
                          "Tab Box", "TrackBall", "Transform Box"};
  tab[ind] = new PoXtChoiceButtonData
    ("Manipulator type", 7, items);
  tab[ind]->setID(D_MANIPULATOR);
  ind ++;
 
  tab[ind] = new PoXtToggleButtonData
    ("Collision Detection", PoXtToggleButtonData::ON, "on", "off");
  tab[ind]->setID(D_MANAGER_ON);
  ind ++;
 
  tab[ind] = new PoXtToggleButtonData
    ("Gluing", PoXtToggleButtonData::ON, "on", "off");
  tab[ind]->setID(D_GLUING_ON);
  ind ++;

  tab[ind] = new PoXtIntSliderData
    ("Gluing level", 0, 10, 1);
  tab[ind]->setID(D_GLUING_LEVEL);
  ind ++;

  tab[ind] = new PoXtToggleButtonData
    ("Object BBox only", PoXtToggleButtonData::OFF, "on", "off");
  tab[ind]->setID(D_OBJECT_BBOX_ONLY);
  ind ++;
 
  tab[ind] = new PoXtChoiceButtonData
    ("Object file", D_NUM_OBJECTS, objects_title);
  tab[ind]->setID(D_OBJECT_SELECTION);
  ind ++;
 
  tab[ind] = new PoXtToggleButtonData
    ("Scene BBox only", PoXtToggleButtonData::OFF, "on", "off");
  tab[ind]->setID(D_SCENE_BBOX_ONLY);
  ind ++;
 
  tab[ind] = new PoXtChoiceButtonData
    ("Scene file", D_NUM_SCENES, scenes_title);
  tab[ind]->setID(D_SCENE_SELECTION);
  ind ++;
  
  tab[ind] = new PoXtTriggerButtonData
    ("Copy to scene", "Copy");
  tab[ind]->setID(D_COPY_TO_SCENE);
  ind ++;
  
  tab[ind] = new PoXtTriggerButtonData
    ("Save scene to file", "output.iv");
  tab[ind]->setID(D_SAVE_SCENE);
  ind ++;

  PoXtDialog* control_dlg = new PoXtDialog 
    (NULL, "Object Moving Control", FALSE, D_NUM_CONTROLS,
     (const PoXtElementData **)tab);
  control_dlg->addValueChangedCallback 
    ((PoXtDialog::PoXtDialogCB*) onControlChange, NULL);
  control_dlg->show();

  // Main loop -----------------------------------------------------------------
  SoXt::show(myWindow);
  SoXt::mainLoop();

  m_root->unref();
  delete control_dlg;
  delete myViewer;
  SoXt::finish();

  return 0;
}

//******************************************************************************
// Function called when there is a change in the control box
void
onControlChange(void*, PoXtDialog::PoXtDialogDataCB *data)
{
  const PoXtElementData* element = data->dialog->get1Value(data->widgetNumber);

  switch (element->getID()) {
  case D_MANIPULATOR:
    switch (((PoXtChoiceButtonData*) element)->getSelectedItem()) {

      // Centerball manipulator
    case 1:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoCenterballManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // Handlebox manipulator
    case 2:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoHandleBoxManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // Jack manipulator
    case 3:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoJackManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // TabBox manipulator
    case 4:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoTabBoxManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // Trackball manipulator
    case 5:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoTrackballManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // TransformBox manipulator
    case 6:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
      }
      m_manip = new SoTransformBoxManip;
      m_manip->setName("m_manip");
      m_manip->ref();
      m_manip->replaceNode(m_transform_path);
      m_collision_manager->setTransform(m_manip);
      break;

      // No manipulator
    default:
      if (m_manip != NULL) {
        m_manip->replaceManip(m_transform_path, NULL);
        m_manip->unref();
        m_manip = NULL;
        m_collision_manager->setTransform(m_object_transform);
      }
      break;
    }
    break;

  case D_MANAGER_ON:
    if (((PoXtToggleButtonData*) element)->getState() ==
        PoXtToggleButtonData::ON) {
      m_collision_manager->activate();
    } else {
      m_collision_manager->activate(FALSE);
    }
    break;

  case D_GLUING_ON:
    if (((PoXtToggleButtonData*) element)->getState() ==
        PoXtToggleButtonData::ON) {
      m_collision_manager->setGluing();
    } else {
      m_collision_manager->setGluing(FALSE);
    }
    break;

  case D_COPY_TO_SCENE:
    {
      SoSeparator* sep = new SoSeparator;
      SoTransform* t = new SoTransform;
      if (m_manip == NULL) {
        t->translation.setValue 
	  (m_object_transform->translation.getValue());
        t->rotation.setValue 
	  (m_object_transform->rotation.getValue());
        t->scaleFactor.setValue 
	  (m_object_transform->scaleFactor.getValue());
        t->scaleOrientation.setValue 
	  (m_object_transform->scaleOrientation.getValue());
        t->center.setValue 
	  (m_object_transform->center.getValue());
      } else {
        t->translation.setValue(m_manip->translation.getValue());
        t->rotation.setValue(m_manip->rotation.getValue());
        t->scaleFactor.setValue(m_manip->scaleFactor.getValue());
        t->scaleOrientation.setValue(m_manip->scaleOrientation.getValue());
        t->center.setValue(m_manip->center.getValue());
      }
      sep->addChild(t);
      sep->addChild(m_object);
      m_scene->addChild(sep);
    }
    break;

  case D_SAVE_SCENE:
    {
      SoWriteAction myAction;
      myAction.getOutput()->openFile("output.iv");
      myAction.getOutput()->setBinary(FALSE);
      myAction.apply(m_scene);
      myAction.getOutput()->closeFile();
    }
    break;

  case D_OBJECT_BBOX_ONLY:
    if (((PoXtToggleButtonData*) element)->getState() ==
        PoXtToggleButtonData::ON) {
      m_collision_manager->setObjectBBoxOnly();
    } else {
      m_collision_manager->setObjectBBoxOnly(FALSE);
    }
    break;

  case D_SCENE_BBOX_ONLY:
    if (((PoXtToggleButtonData*) element)->getState() ==
        PoXtToggleButtonData::ON) {
      m_collision_manager->setSceneBBoxOnly();
    } else {
      m_collision_manager->setSceneBBoxOnly(FALSE);
    }
    break;

  case D_OBJECT_SELECTION:
    {
      char filename [1024];
      sprintf(filename, "Objects/%s", 
	      objects_title [((PoXtChoiceButtonData*) element)->
			    getSelectedItem()]);
      SoSeparator* sep = readIvFile(filename);
      if (sep != NULL) {
	if (m_manip != NULL) {
	  m_manip->replaceManip(m_transform_path, NULL);
	}
	m_object->removeAllChildren();
        m_object->addChild(sep);
	if (m_manip != NULL) {
	  m_manip->replaceNode(m_transform_path);
	}
      } 
      else {
        flashObject();
      }
    }
    break;

  case D_SCENE_SELECTION:
    {
      char filename [1024];
      sprintf(filename, "Scenes/%s", 
	      scenes_title [((PoXtChoiceButtonData*) element)->
			   getSelectedItem()]);
      SoSeparator* sep = readIvFile(filename);
      if (sep != NULL) {
        m_scene->removeAllChildren();
        m_scene->addChild(sep);
      } else {
        flashObject();
      }
    }
    break;

  case D_GLUING_LEVEL:
    m_collision_manager->setGluingLevel 
      (((PoXtIntSliderData*) element)->getValue());
    break;

  default:;
  }
}

//******************************************************************************
// Function called when there is a collision
SoCollisionManager::Resp
onCollision(void*, const SoCollidingPrimitive*, const SoCollidingPrimitive*)
{
  flashObject();
  return SoCollisionManager::ABORT;
}

//******************************************************************************
// Flah one time the object
void
flashObject()
{
  static SoAlarmSensor* sensor = NULL;

  m_switch_material->whichChild.setValue(SO_SWITCH_ALL);
  if (sensor == NULL) {
    sensor = new SoAlarmSensor(unflashObject, NULL);
  }
  sensor->setTimeFromNow(SbTime(0.15));
  sensor->schedule();
}

//******************************************************************************
// Unflash the object
void
unflashObject(void*, SoSensor*)
{
  m_switch_material->whichChild.setValue(SO_SWITCH_NONE);
}

//******************************************************************************
// Reading a file
SoSeparator*
readIvFile(const char* filename)
{
  SoInput input;
  if (!input.openFile(filename))  return NULL;
  SoSeparator* sep = SoDB::readAll(&input);
  input.closeFile();
  return sep;
}

