#include <DemoCollider.h>
#include <DemoColliderDialog.h>

//--------------------------------------------------------------------------------
DemoCollider::DemoCollider(int maxTforStatic, int maxTforMoving) :
  m_lastHighLightGroup(NULL) ,
  m_lastPath(NULL),
  m_numc(0),
  m_staticScene(NULL),
  m_maxStaticT(maxTforStatic),
  m_maxMovingT(maxTforMoving),
  m_dialog(NULL)
{
  m_movingBbox.setBounds(0,0,0,1,1,1);
  m_staticBbox.setBounds(0,0,0,1,1,1);

  m_manip = new SoHandleBoxManip();

  // build the group that highlight the colliding triangle pair 
  SoFaceSet *face_set = new SoFaceSet;
  face_set->numVertices.set1Value(0,3);
  face_set->numVertices.set1Value(1,3);
  m_trianglesCoord = new SoCoordinate3;
  m_collidingSwitch = new SoSwitch;
  m_collidingSwitch->whichChild = SO_SWITCH_NONE;
  SoMaterial *colliding_mat = new SoMaterial;
  colliding_mat->diffuseColor.setValue(1,1,0);
  SoSeparator *colliding_sep = new SoSeparator;

  // build the common point of current pair of intersecting triangle
  m_commonPtsCoord = new SoCoordinate3;
  SoMarkerSet *marker_set = new SoMarkerSet;
  marker_set->markerIndex.setValue(SoMarkerSet::SATELLITE_FILLED_9_9);
  m_root = new SoSeparator; 
  m_root->ref();

  m_movingRoot = new SoSeparator;
  m_movingScene = new SoGroup;

  // define an highlight node (special color attr) that will be inserted just before the
  // colliding shape
  m_highlight = new SoMaterial;
  m_highlight->diffuseColor = SbColor(0,1,0);
  m_highlight->ref();

  // assemble the scene graph
  m_root->addChild(m_movingRoot);
  {
    m_movingRoot->addChild(colliding_sep);
    {
      colliding_sep->addChild(m_collidingSwitch);
      {
        m_collidingSwitch->addChild(m_trianglesCoord);
        m_collidingSwitch->addChild(colliding_mat);
        m_collidingSwitch->addChild(new SoPolygonOffset);
        m_collidingSwitch->addChild(face_set);
        m_collidingSwitch->addChild(m_commonPtsCoord);
        m_collidingSwitch->addChild(marker_set);
        
      }
    }
    m_movingRoot->addChild(m_manip); 
    m_movingRoot->addChild(m_movingScene);
  }

  m_staticScene_path = new SoPath;
  newStaticScene(defaultScene());

  m_movingScene_path = new SoPath(m_root);
  newMovingScene(defaultScene());

  activate(m_manip);
  m_isactive = TRUE;
}

//--------------------------------------------------------------------------------
SoGroup* 
DemoCollider::defaultScene() 
{
   SoGroup *parent = new SoGroup;

   SoSphere* m_object_sphere = new SoSphere;
  //SoCube* m_object_sphere = new SoCube;
   SoMaterial  *sphere_mat  = new SoMaterial;
   sphere_mat->diffuseColor.setValue( 1, 0, 0 );
   SoComplexity* cps = new SoComplexity;
   cps->value.setValue(1);

   parent->addChild(sphere_mat);
   parent->addChild(cps);
   parent->addChild(m_object_sphere);
 
   return parent;
}

//--------------------------------------------------------------------------------
void 
DemoCollider::newMovingScene(SoNode *scene) 
{
  m_movingScene->removeAllChildren();
  m_movingScene->addChild(scene);

  m_movingScene_path->truncate(0);
  m_movingScene_path->append(m_root);
  m_movingScene_path->append(m_movingRoot);
  m_movingScene_path->append(m_movingScene);

  // update the position and size of the moving object according to the static scene
  SoGetBoundingBoxAction *act = new SoGetBoundingBoxAction(SbViewportRegion());
  m_movingScene->ref();
  act->apply(m_movingScene);
  m_movingBbox = act->getBoundingBox();
  m_movingScene->unrefNoDelete();

  updateManip(m_movingBbox,m_staticBbox);

  setMovingScene(m_movingScene_path,m_maxMovingT);
}

//--------------------------------------------------------------------------------
void 
DemoCollider::newStaticScene(SoNode *scene) 
{
  SoNode *last_static_scene = m_staticScene;
  m_staticScene = scene;

  if (last_static_scene == NULL)
    m_root->addChild(m_staticScene);
  else
    m_root->replaceChild(last_static_scene,m_staticScene);

  m_staticScene_path->truncate(0);
  m_staticScene_path->append(m_root);
  m_staticScene_path->append(m_staticScene);

  setStaticScene(m_staticScene_path,m_maxStaticT);

  // update the position and size of the moving object according to the static scene
  SoGetBoundingBoxAction *act = new SoGetBoundingBoxAction(SbViewportRegion());
  m_staticScene->ref();
  act->apply(m_staticScene);
  m_staticBbox = act->getBoundingBox();
  m_staticScene->unrefNoDelete();

  updateManip(m_movingBbox,m_staticBbox);

  m_lastPath = NULL;  
}

//--------------------------------------------------------------------------------
void
DemoCollider::updateManip(SbBox3f &moving_bbox, SbBox3f &static_bbox)
{
  // scale the "moving" object according to the static scene size
  float sx,sy,sz,smax,mmax;
  static_bbox.getSize(sx,sy,sz);
  smax = sx;
  if (sy > smax) smax = sy;
  if (sz > smax) smax = sz;
  smax /= 2;

  moving_bbox.getSize(sx,sy,sz);
  mmax = sx;
  if (sy > mmax) mmax = sy;
  if (sz > mmax) mmax = sz;

  // workaround the bug #1415
  SoHandleBoxManip *manip = new SoHandleBoxManip();
  m_movingRoot->replaceChild(m_manip,manip);
  m_manip = manip;
  enableCollisions(m_isactive);
  // end workaround

  // translate the "moving" object at the lower corner of the static scene
  m_manip->translation = -static_bbox.getMin() - moving_bbox.getCenter();
  m_manip->scaleFactor = SbVec3f(smax/mmax,smax/mmax,smax/mmax);
  m_manip->center = moving_bbox.getCenter();
}


//--------------------------------------------------------------------------------
SbBool 
DemoCollider::checkCollision() const
{
  SbBool result = SoDualSceneCollider::checkCollision();
  if (result == FALSE) {
    if (m_lastPath != NULL) {
      // a new object was colliding during last checkCollision
      m_lastHighLightGroup->removeChild(m_highlight);
      m_collidingSwitch->whichChild = SO_SWITCH_NONE;
      messageCollision("Collision : ",0);
      messageWarning("Warning : ");
    }
    m_lastPath = NULL;
  }
  return result;
}


//--------------------------------------------------------------------------------
SbBool 
DemoCollider::searchNextIntersection() const { 
  // a new intersection occurs
  messageCollision("Collision : ",m_numc++);

    // get the path in the static scene of the current colliding shape
  const SoPath *path = getCollidingStaticPath(); 
  int shape_index = path->getIndexFromTail (0);
  if (m_lastPath != path) {
    // a new object is colliding, highlight it by a inserting the given m_highlight node
    if (m_lastPath != NULL)  m_lastHighLightGroup->removeChild(m_highlight);
    SoNode *parent = path->getNodeFromTail(1);
    if (parent->getTypeId() == SoGroup::getClassTypeId() ||
        parent->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) {
      m_lastHighLightGroup = (SoGroup *)parent; 
      m_lastHighLightGroup->insertChild(m_highlight,shape_index); 
      messageWarning("Warning : ");
    } else {
       // the parent node may be a VRML node like VRML shape
      messageWarning("Warning : highlight of VRML node is not managed by this demo");
      path = NULL;
    }
  } 

  m_collidingSwitch->whichChild = SO_SWITCH_ALL;
  SbVec3f v0,v1,v2;
  getCollidingStaticTriangle(v0,v1,v2);
  m_trianglesCoord->point.set1Value(0,v0);
  m_trianglesCoord->point.set1Value(1,v1);
  m_trianglesCoord->point.set1Value(2,v2);
  getCollidingMovingTriangle(v0,v1,v2);
  m_trianglesCoord->point.set1Value(3,v0);
  m_trianglesCoord->point.set1Value(4,v1);
  m_trianglesCoord->point.set1Value(5,v2);

  getCommonPoints(v0,v1);
  m_commonPtsCoord->point.set1Value(0,v0);
  m_commonPtsCoord->point.set1Value(1,v1);

  m_lastPath = path;  
  return FALSE; 
}


//--------------------------------------------------------------------------------
void 
DemoCollider::staticTriangleListBuilt(int num_t) const 
{
  m_numST = num_t;
  m_percentS = 5;
  m_numst = 0;
  messageStaticScene(0);
}

//--------------------------------------------------------------------------------
void  
DemoCollider::staticLeafBuilt(int num_triangles) const 
{
  m_numst += num_triangles;
  if (m_numst >= (m_numST * m_percentS)/100) {
    messageStaticScene(m_percentS);
    m_percentS += 5;
  }
}


//--------------------------------------------------------------------------------
void 
DemoCollider::movingTriangleListBuilt(int num_t) const 
{
  m_numMT = num_t;
  m_percentM = 5;
  m_nummt = 0;
  messageMovingScene(0);
}

//--------------------------------------------------------------------------------
void  
DemoCollider::movingLeafBuilt(int num_triangles) const 
{
  m_nummt += num_triangles;
  if (m_nummt >= (m_numMT * m_percentM)/100) {
    messageMovingScene(m_percentM);
    m_percentM += 5;
  }
}


//--------------------------------------------------------------------------------
void  
DemoCollider::enableCollisions(SbBool check)
{
   if (check) {
    activate(m_manip);
    messageCollision("Collision : detection enabled !");
    m_isactive = TRUE;
   } else {
    activate(NULL);
    messageCollision("Collision : detection disabled !");
    m_isactive = FALSE;
   }
}

//--------------------------------------------------------------------------------
void  
DemoCollider::setMaxTriangleForMoving(int maxT)
{
  setMovingScene(m_movingScene_path,maxT);
  m_maxStaticT = maxT;
}

//--------------------------------------------------------------------------------
void  
DemoCollider::setMaxTriangleForStatic(int maxT)
{
  setStaticScene(m_staticScene_path,maxT);
  m_maxMovingT = maxT;
}

//--------------------------------------------------------------------------------
void  
DemoCollider::messageStaticScene(int percentDone) const
{
  char str[80];
  sprintf(str,"Static scene : %d triangles found, %3d%% processed",m_numST,percentDone);
  if (m_dialog != NULL) m_dialog->messageStatic(str);

}

//--------------------------------------------------------------------------------
void  
DemoCollider::messageMovingScene(int percentDone) const
{
  char str[80];
  sprintf(str,"Moving scene : %d triangles found, %3d%% processed",m_numMT,percentDone);
  if (m_dialog != NULL) m_dialog->messageMoving(str);
}

//--------------------------------------------------------------------------------
void  
DemoCollider:: messageCollision(const char * msg, int numc) const
{
  char str[80];
  if (numc==0)
    sprintf(str,"%s",msg);
  else
    sprintf(str,"%s %d",msg,numc);
  if (m_dialog != NULL) m_dialog->messageCollision(str);
}

//--------------------------------------------------------------------------------
void  
DemoCollider:: messageWarning(const char * msg) const
{
  if (m_dialog != NULL) m_dialog->messageWarning(msg);
}

