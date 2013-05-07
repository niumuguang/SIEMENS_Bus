#ifndef  _DEMO_COLLIDER_
#define  _DEMO_COLLIDER_

#include <Inventor/collision/SoDualSceneCollider.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h> 
#include <Inventor/nodes/SoCube.h> 
#include <Inventor/nodes/SoFaceSet.h> 
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoComplexity.h> 
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMaterial.h> 
#include <Inventor/nodes/SoPolygonOffset.h> 
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/manips/SoHandleBoxManip.h> 
#include <Inventor/actions/SoGetPrimitiveCountAction.h> 
#include <Inventor/actions/SoGetBoundingBoxAction.h> 

class DemoColliderDialog;

class DemoCollider : public SoDualSceneCollider {
public:
  DemoCollider(int maxTforStatic, int maxTforMoving);

  SoGroup *getRoot() const {
    return m_root;
  }

  // checkCollision is overloaded to un-highlight the previous colliding object when no collision occur
  virtual SbBool checkCollision() const;

  // method called by checkCollision when a new collision occur
  // when returning TRUE, the process to find the list of pair of intersecting shape is stopped.
  virtual SbBool searchNextIntersection() const;

  // method called when the list of triangle is built
  void staticTriangleListBuilt(int num_t) const;

  // method called when a new leaf is built
  void staticLeafBuilt(int num_triangles) const;

  // method called when the list of triangle is built
  void movingTriangleListBuilt(int num_t) const;

  // method called when a new leaf is built
  void movingLeafBuilt(int num_triangles) const;

  // define a new static scene
  void newStaticScene(SoNode *scene);

  // define a new moving scene
  void newMovingScene(SoNode *scene);

  SoGroup* defaultScene();

  void enableCollisions(SbBool check);
  void setMaxTriangleForStatic(int maxT);
  void setMaxTriangleForMoving(int maxT);
  void updateManip(SbBox3f &moving_bbox, SbBox3f &static_bbox);

  void messageCollision(const char *, int numc=0) const;
  void messageMovingScene(int percentDone) const;
  void messageStaticScene(int percentDone) const;
  void messageWarning(const char *msg) const;

  void setDialog(DemoColliderDialog *dialog) {
    m_dialog = dialog;
  }

private:
  SoHandleBoxManip       *m_manip;
  SoGroup                *m_movingScene;
  SoSeparator            *m_movingRoot;

  SoSeparator            *m_root;
  SoNode                 *m_staticScene;
  SoPath                 *m_staticScene_path;
  SoPath                 *m_movingScene_path;
  SoMaterial             *m_highlight;
  SoSwitch               *m_collidingSwitch;
  SoCoordinate3          *m_trianglesCoord;
  SoCoordinate3          *m_commonPtsCoord;
  SbBox3f                m_movingBbox;
  SbBox3f                m_staticBbox;

  DemoColliderDialog     *m_dialog;

  int                    m_maxStaticT;
  int                    m_maxMovingT;
  SbBool                 m_isactive;

  mutable SoGroup        *m_lastHighLightGroup;
  mutable const SoPath   *m_lastPath;
  mutable int            m_numc;
  mutable int            m_numST,m_numst,m_percentS;
  mutable int            m_numMT,m_nummt,m_percentM;
};

#endif
