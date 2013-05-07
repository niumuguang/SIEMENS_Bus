/*****************************************************************-*-c++-*-*****
 * File: Dart.cxx
 * Author(s) : Fabien ARNAUD
 * Date: 07/03/97
 * Revision: 1.0
 *==============================================================================
 * (c) Copyright 1997 G5G, all rights reserved
 *==============================================================================
 *
 * Purpose : This program is an example of the use of the SoCollisionManager
 *           class. It's the beginning of a dart game program.
 *
 ******************************************************************************/

#ifdef _AIX
#  include <sys/time.h>
#endif

//------------------------------------------------------------------------------
// Inventor declarations
#include <Inventor/collision/SoCollisionManager.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoShapeHints.h>

#include <Inventor/SoMacApp.h>

//------------------------------------------------------------------------------
// Global objects
int                 m_current_dart;
SoSeparator *       m_root;
SoTimerSensor *     m_timer;
SoText3 *           m_score_text;
SoSwitch *          m_cone_switch_material;
SoSeparator *       m_dart_separator;
SoTransformManip *  m_dart_manip;
SoSeparator *       m_dart[3];
SoTransform *       m_dart_orientation[3];
SoPath *            m_dart_orientation_path[3];
SoPath *            m_dart_skeleton_path[3];
SoSeparator *       m_dart_shape[3];
SoTransform *       m_dart_translation[3];
SoPickStyle *       m_dart_pick_style[3];
SoDrawStyle *       m_dart_visibility[3];
SbBool              m_game_over;
int                 m_score;
SoCollisionManager *m_collision_manager;
SbBool              m_dart_has_reached_target;

//------------------------------------------------------------------------------
// Functions declarations
SoSeparator *readIvFile(const char *filename);
void writeIvFile(SoSeparator *root, const char *filename);
SoCollisionManager::Resp onCollision(void *, 
                                     const SoCollidingPrimitive *, 
                                     const SoCollidingPrimitive *);
void onKeyPress(void *, SoEventCallback *event);
void onTimer(void *, SoSensor *);
void newGame();
void newDart();
SbBool filter(void *, const SoPath *, const SoPath *scene);
void setScore();

//******************************************************************************
// Main function
int 
main(int, char **argv)
{
  // Windows initialisation ----------------------------------------------------
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL)
    exit(1);

  // Inventor shared objects ---------------------------------------------------
  SoSeparator *skeleton = new SoSeparator;
  skeleton->setName("skeleton");

  m_cone_switch_material = new SoSwitch;
  m_cone_switch_material->setName("m_cone_switch_material");
  m_cone_switch_material->whichChild.setValue(0);
  skeleton->addChild(m_cone_switch_material);

  SoDrawStyle *invisible_cone_material = new SoDrawStyle;
  invisible_cone_material->setName("invisible_cone_material");
  invisible_cone_material->style.setValue(SoDrawStyle::INVISIBLE);
  m_cone_switch_material->addChild(invisible_cone_material);

  SoMaterial *transparent_material = new SoMaterial;
  transparent_material->setName("transparent_material");
  transparent_material->diffuseColor.setValue(1, 1, 1);
  transparent_material->transparency.setValue(0.3f);
  m_cone_switch_material->addChild(transparent_material);

  SoTransform *cone_transform_1 = new SoTransform;
  cone_transform_1->setName("cone_transform_1");
  cone_transform_1->rotation.setValue(SbVec3f(0, 0, 1) , (float)M_PI);
  cone_transform_1->scaleFactor.setValue(0.08f, 0.08f, 0.08f);
  skeleton->addChild(cone_transform_1);

  SoTransform *cone_transform_2 = new SoTransform;
  cone_transform_2->setName("cone_transform_2");
  cone_transform_2->translation.setValue(0, -1, 0);
  skeleton->addChild(cone_transform_2);

  SoCone *cone = new SoCone;
  cone->setName("cone");
  cone->bottomRadius.setValue(0.5);
  skeleton->addChild(cone);

  SoTransform *dart_scale = new SoTransform;
  dart_scale->setName("dart_scale");
  dart_scale->scaleFactor.setValue(6.25f, 6.25f, 6.25f);
  dart_scale->rotation.setValue(SbVec3f(1, 0, 0), (float)(M_PI/2.0));

  m_dart_shape[0] = new SoSeparator;
  m_dart_shape[0]->setName("m_dart_shape[0]");
  m_dart_shape[0]->ref();
  m_dart_shape[0]->addChild(dart_scale);
  m_dart_shape[0]->addChild(readIvFile("blue_dart.iv"));
  m_dart_shape[0]->addChild(skeleton);

  m_dart_shape[1] = new SoSeparator;
  m_dart_shape[1]->setName("m_dart_shape[1]");
  m_dart_shape[1]->ref();
  m_dart_shape[1]->addChild(dart_scale);
  m_dart_shape[1]->addChild(readIvFile("red_dart.iv"));
  m_dart_shape[1]->addChild(skeleton);

  m_dart_shape[2] = new SoSeparator;
  m_dart_shape[2]->setName("m_dart_shape[2]");
  m_dart_shape[2]->ref();
  m_dart_shape[2]->addChild(dart_scale);
  m_dart_shape[2]->addChild(readIvFile("green_dart.iv"));
  m_dart_shape[2]->addChild(skeleton);
  
  // Scene graph initialisation ------------------------------------------------
  m_root = new SoSeparator;
  m_root->setName("m_root");
  m_root->ref();
  
  SoDirectionalLight *dir_light = new SoDirectionalLight;
  dir_light->setName("dir_light");
  dir_light->direction.setValue(-1, -1, -10);
  dir_light->intensity.setValue(0.7f);
  m_root->addChild(dir_light);
  
  SoPerspectiveCamera *camera = new SoPerspectiveCamera;
  camera->position.setValue(-3.61f, -0.115f, 2.55f);
  camera->orientation.setValue(SbVec3f(0, -1, 0), 0.77f);
  m_root->addChild(camera);

  SoSeparator *room = new SoSeparator;
  room->setName("room");
  m_root->addChild(room);
  
  SoTransform *wall_transform = new SoTransform;
  wall_transform->setName("wall_transform");
  wall_transform->translation.setValue(0, 0, -5);
  room->addChild(wall_transform);

  SoSeparator *wall_separator = new SoSeparator;
  wall_separator->setName("wall_separator");
  room->addChild(wall_separator);

  SoSeparator *score = new SoSeparator;
  score->setName("score");
  wall_separator->addChild(score);
  
  SoMaterial *score_mtl = new SoMaterial;
  score_mtl->setName("score_mtl");
  score_mtl->diffuseColor.setValue(0.0f, 0.8f, 1.0f);
  score->addChild(score_mtl);
  
  SoTransform *score_transform = new SoTransform;
  score_transform->setName("score_transform");
  score_transform->translation.setValue(2.2f, -2.2f, 0.15f);
  score_transform->scaleFactor.setValue(0.07f, 0.07f, 0.07f);
  score->addChild(score_transform);

  m_score_text = new SoText3;
  m_score_text->setName("m_score_text");
  m_score_text->string.setValue("0000");
  m_score_text->justification.setValue(SoText3::RIGHT);
  m_score_text->parts.setValue(SoText3::FRONT + SoText3::SIDES);
  score->addChild(m_score_text);

  SoSeparator *target = new SoSeparator;
  target->setName("target");
  room->addChild(target);

  SoMaterialBinding *target_mtl_binding = new SoMaterialBinding;
  target_mtl_binding->setName("target_mtl_binding");
  target_mtl_binding->value.setValue(SoMaterialBinding::OVERALL);
  target->addChild(target_mtl_binding);

  SoTransform *target_transform = new SoTransform;
  target_mtl_binding->setName("target_mtl_binding");
  target_transform->rotation.setValue(SbVec3f(1, 0, 0), (float)(M_PI/2.0));
  target->addChild(target_transform);

#define TARGET_NUM_POINTS 25
  float target_vertexes[TARGET_NUM_POINTS * 8][3];
  float alpha = 0.0f;
  for (int i = 0; i < TARGET_NUM_POINTS; i++) {
    target_vertexes[ TARGET_NUM_POINTS * 0 + i][0] = (float)cos(alpha) * 5.0f;
    target_vertexes[ TARGET_NUM_POINTS * 0 + i][1] = 0.0f;
    target_vertexes[ TARGET_NUM_POINTS * 0 + i][2] = (float)sin(alpha) * 5.0f;
    target_vertexes[ TARGET_NUM_POINTS * 1 + i][0] = (float)cos(alpha) * 0.9f;
    target_vertexes[ TARGET_NUM_POINTS * 1 + i][1] = 0.0f;
    target_vertexes[ TARGET_NUM_POINTS * 1 + i][2] = (float)sin(alpha) * 0.9f;
    target_vertexes[ TARGET_NUM_POINTS * 2 + i][0] = (float)cos(alpha) * 0.9f;
    target_vertexes[ TARGET_NUM_POINTS * 2 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 2 + i][2] = (float)sin(alpha) * 0.9f;
    target_vertexes[ TARGET_NUM_POINTS * 3 + i][0] = (float)cos(alpha) * 0.7f;
    target_vertexes[ TARGET_NUM_POINTS * 3 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 3 + i][2] = (float)sin(alpha) * 0.7f;
    target_vertexes[ TARGET_NUM_POINTS * 4 + i][0] = (float)cos(alpha) * 0.5f;
    target_vertexes[ TARGET_NUM_POINTS * 4 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 4 + i][2] = (float)sin(alpha) * 0.5f;
    target_vertexes[ TARGET_NUM_POINTS * 5 + i][0] = (float)cos(alpha) * 0.3f;
    target_vertexes[ TARGET_NUM_POINTS * 5 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 5 + i][2] = (float)sin(alpha) * 0.3f;
    target_vertexes[ TARGET_NUM_POINTS * 6 + i][0] = (float)cos(alpha) * 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 6 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 6 + i][2] = (float)sin(alpha) * 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 7 + i][0] = (float)cos(alpha) * 0.00001f;
    target_vertexes[ TARGET_NUM_POINTS * 7 + i][1] = 0.1f;
    target_vertexes[ TARGET_NUM_POINTS * 7 + i][2] = (float)sin(alpha) * 0.00001f;
    alpha += (float)(2 * M_PI / TARGET_NUM_POINTS);
  }
  SoCoordinate3 *target_coords = new SoCoordinate3;
  target_coords->setName("target_coords");
  target_coords->point.setValues(0, TARGET_NUM_POINTS * 8, 
                                 (const float(*)[3])target_vertexes);
  target->addChild(target_coords);

  SoSeparator *out_sep = new SoSeparator;
  out_sep->setName("out_sep");
  target->addChild(out_sep);

  SoTexture2 *out_target_mtl = new SoTexture2;
  out_target_mtl->setName("out_target_mtl");
  out_target_mtl->filename.setValue("wood.rgb");
  out_sep->addChild(out_target_mtl);
  
  int j;
  int32_t target_indexes[2 *(TARGET_NUM_POINTS + 1)];
  int ind = TARGET_NUM_POINTS * 0;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_out = new SoIndexedTriangleStripSet;
  target_out->setName("target_out");
  target_out->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				   target_indexes);
  out_sep->addChild(target_out);

  SoMaterial *black_target_mtl = new SoMaterial;
  black_target_mtl->setName("black_target_mtl");
  black_target_mtl->diffuseColor.setValue(0, 0, 0);
  target->addChild(black_target_mtl);
  
  ind = TARGET_NUM_POINTS * 1;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_edge = new SoIndexedTriangleStripSet;
  target_edge->setName("target_edge");
  target_edge->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				    target_indexes);
  target->addChild(target_edge);

  ind = TARGET_NUM_POINTS * 2;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_10 = new SoIndexedTriangleStripSet;
  target_10->setName("target_10");
  target_10->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				  target_indexes);
  target->addChild(target_10);

  ind = TARGET_NUM_POINTS * 4;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_100 = new SoIndexedTriangleStripSet;
  target_100->setName("target_100");
  target_100->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				   target_indexes);
  target->addChild(target_100);

  SoMaterial *yellow_target_mtl = new SoMaterial;
  yellow_target_mtl->setName("yellow_target_mtl");
  yellow_target_mtl->diffuseColor.setValue(1.0f, 0.9f, 0.0f);
  target->addChild(yellow_target_mtl);

  ind = TARGET_NUM_POINTS * 3;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_50 = new SoIndexedTriangleStripSet;
  target_50->setName("target_50");
  target_50->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				  target_indexes);
  target->addChild(target_50);

  ind = TARGET_NUM_POINTS * 5;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_500 = new SoIndexedTriangleStripSet;
  target_500->setName("target_500");
  target_500->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				   target_indexes);
  target->addChild(target_500);

  SoMaterial *red_target_mtl = new SoMaterial;
  red_target_mtl->setName("red_target_mtl");
  red_target_mtl->diffuseColor.setValue(1, 0, 0);
  target->addChild(red_target_mtl);

  ind = TARGET_NUM_POINTS * 6;
  target_indexes[2 * TARGET_NUM_POINTS    ] = ind;
  target_indexes[2 * TARGET_NUM_POINTS + 1] = ind + TARGET_NUM_POINTS;
  for (j=0; j<TARGET_NUM_POINTS; j++) {
    target_indexes[j * 2    ] = ind;
    target_indexes[j * 2 + 1] = ind + TARGET_NUM_POINTS;
    ind ++;
  }
  SoIndexedTriangleStripSet *target_1000 = new SoIndexedTriangleStripSet;
  target_1000->setName("target_1000");
  target_1000->coordIndex.setValues(0, 2 *(TARGET_NUM_POINTS + 1), 
				    target_indexes);
  target->addChild(target_1000);

  m_dart_separator = new SoSeparator;
  m_dart_separator->setName("m_dart_separator");
  m_root->addChild(m_dart_separator);

  // Keyboard event ------------------------------------------------------------
  SoEventCallback *event = new SoEventCallback;
  event->setName("event");
  event->addEventCallback(SoKeyboardEvent::getClassTypeId(), onKeyPress, NULL);
  m_root->addChild(event);

  // Set up viewer -------------------------------------------------------------
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(m_root);
  myViewer->setTitle("Dart");
  myViewer->setViewing(FALSE);
  myViewer->show();

  // Moving timer --------------------------------------------------------------
  m_timer = new SoTimerSensor(onTimer, NULL);
  m_timer->setInterval(SbTime(0.1));

  // Starting game -------------------------------------------------------------
#ifdef _AIX
  srand(time_t(NULL));
#else
  srand(time(NULL));
#endif
  m_dart_manip = new SoTrackballManip;
  m_dart_manip->ref();

  newGame();
  newDart();

  // Main loop -----------------------------------------------------------------
  SoXt::show(myWindow);
  SoXt::mainLoop();

  m_root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}

//******************************************************************************
// Starting a new game
void newGame()
{
  static SbBool first_time = TRUE;
  m_game_over = FALSE;
  m_score = 0;
  setScore();

  if (!first_time) {
    first_time = FALSE;
    for (int i=0; i<3; i++) {
      m_dart_orientation[i]->unref();
      m_dart_orientation_path[i]->ref();
    }
  }
  first_time = FALSE;

  m_dart_separator->removeAllChildren();
  for (int i=0; i<3; i++) {
    char buff[256];
    sprintf(buff, "[%d]", i);

    m_dart[i] = new SoSeparator;
    sprintf(buff, "m_dart[%d]", i);
    m_dart[i]->setName(buff);

    m_dart_pick_style[i] = new SoPickStyle;
    sprintf(buff, "m_dart_pick_style[%d]", i);
    m_dart_pick_style[i]->setName(buff);
    m_dart_pick_style[i]->style.setValue(SoPickStyle::UNPICKABLE);
    m_dart[i]->addChild(m_dart_pick_style[i]);

    m_dart_visibility[i] = new SoDrawStyle;
    sprintf(buff, "m_dart_visibility[%d]", i);
    m_dart_visibility[i]->setName(buff);
    m_dart_visibility[i]->style.setValue(SoDrawStyle::INVISIBLE);
    m_dart[i]->addChild(m_dart_visibility[i]);

    m_dart_orientation[i] = new SoTransform;
    sprintf(buff, "m_dart_orientation[%d]", i);
    m_dart_orientation[i]->setName(buff);
    m_dart_orientation[i]->ref();
    m_dart[i]->addChild(m_dart_orientation[i]);
    
    m_dart_translation[i] = new SoTransform;
    sprintf(buff, "m_dart_translation[%d]", i);
    m_dart_translation[i]->setName(buff);
    m_dart[i]->addChild(m_dart_translation[i]);

    m_dart[i]->addChild(m_dart_shape[i]);
    
    m_dart_separator->addChild(m_dart[i]);

    m_dart_orientation_path[i] = new SoPath(m_root);
    m_dart_orientation_path[i]->setName(buff);
    m_dart_orientation_path[i]->append(m_dart_separator);
    m_dart_orientation_path[i]->append(m_dart[i]);
    m_dart_orientation_path[i]->append(m_dart_orientation[i]);
    m_dart_orientation_path[i]->ref();

    m_dart_skeleton_path[i] = new SoPath(m_root);
    m_dart_skeleton_path[i]->setName(buff);
    m_dart_skeleton_path[i]->append(m_dart_separator);
    m_dart_skeleton_path[i]->append(m_dart[i]);
    m_dart_skeleton_path[i]->append(m_dart_shape[i]);
    m_dart_skeleton_path[i]->append(2);
    m_dart_skeleton_path[i]->ref();
  }
  m_current_dart = -1;
}

//******************************************************************************
// Managing a new dart.
void newDart()
{
  m_dart_has_reached_target = FALSE;
  m_current_dart ++;
  if (m_current_dart == 3) {
    delete m_collision_manager;
    m_collision_manager = NULL;
    m_game_over = TRUE;
  }

  else {
    if (m_collision_manager == NULL) {
      m_collision_manager = new SoCollisionManager
	(m_dart_skeleton_path[m_current_dart],
         m_root, 
         m_dart_translation[m_current_dart]);
      m_collision_manager->setGluing(FALSE);
#ifdef __osf__
      m_collision_manager->addCollisionCallback((SoCollisionManager::SoCollisionCB*)onCollision);
#else
      m_collision_manager->addCollisionCallback(onCollision);
#endif
      m_collision_manager->setFilterCallback(filter);
    }
    else {
      m_collision_manager->setObject(m_dart_skeleton_path[m_current_dart]);
      m_collision_manager->setTransform(m_dart_translation[m_current_dart]);
    }

    float d =((float)rand()) / RAND_MAX;
    float alpha = (float)((rand() * M_PI * 2.0 / RAND_MAX) - M_PI);
    m_dart_pick_style[m_current_dart]->style.setValue(SoPickStyle::SHAPE);
    m_dart_visibility[m_current_dart]->style.setValue(SoDrawStyle::FILLED);
    m_dart_orientation[m_current_dart]->translation.setValue(d * (float)cos(alpha), 
							     d * (float)sin(alpha), 
							     0);
    m_dart_manip->replaceNode(m_dart_orientation_path[m_current_dart]);
  }
}

//******************************************************************************
// Throwing a dart or new game.
void
onKeyPress(void*, SoEventCallback *event_cbk)
{
  const SoEvent *event = event_cbk->getEvent();
  if (SO_KEY_PRESS_EVENT(event, SPACE)) {

    if (m_game_over) {
      newGame();
      newDart();
    }

    else {
      m_dart_manip->replaceManip(m_dart_orientation_path[m_current_dart], NULL);
      m_timer->schedule();
    }
  }
}

//******************************************************************************
// Moving a dart.
void
onTimer(void*, SoSensor*)
{
  if (m_dart_translation[m_current_dart]->translation.getValue().length() > 7) {
    m_dart_visibility[m_current_dart]->style.setValue(SoDrawStyle::INVISIBLE);
    m_timer->unschedule();
    newDart();
    return;
  }
  if (m_dart_has_reached_target) {
    m_timer->unschedule();
    newDart();
    return;
  }
  m_dart_translation[m_current_dart]->translation.setValue
    (m_dart_translation[m_current_dart]->translation.getValue() + 
     SbVec3f(0, 0, -0.25));
}

//******************************************************************************
// Collision filter
SbBool
filter(void *, const SoPath *, const SoPath *scene)
{
  if (scene->containsNode(m_dart_separator))
    return FALSE;
  return TRUE;
}

//******************************************************************************
// Function called when there is a collision
SoCollisionManager::Resp
onCollision(void *, const SoCollidingPrimitive *, const SoCollidingPrimitive *scene)
{
  m_dart_has_reached_target = TRUE;
  if (scene->path->getTail()->getName() == "target_10") {
    m_score += 10;
  }
  else if (scene->path->getTail()->getName() == "target_50") {
    m_score += 50;
  }
  else if (scene->path->getTail()->getName() == "target_100") {
    m_score += 100;
  }
  else if (scene->path->getTail()->getName() == "target_500") {
    m_score += 500;
  }
  else if (scene->path->getTail()->getName() == "target_1000") {
    m_score += 1000;
  }
  setScore();
  return SoCollisionManager::ABORT;
}

//******************************************************************************
// Setting score text
void
setScore()
{
  static char buffer[1024];
  sprintf(buffer, "%d", m_score);
  m_score_text->string.setValue(buffer);
}

//******************************************************************************
// Reading a file
SoSeparator*
readIvFile(const char *filename)
{
  SoInput input;
  if (!input.openFile(filename))
    return NULL;
  SoSeparator *sep = SoDB::readAll(&input);
  input.closeFile();
  return sep;
}

//******************************************************************************
// Writing a file
void
writeIvFile(SoSeparator *root, const char *filename)
{
  SoWriteAction action;
  action.getOutput()->openFile(filename);
  action.getOutput()->setBinary(FALSE);
  action.apply(root);
  action.getOutput()->closeFile();
}


