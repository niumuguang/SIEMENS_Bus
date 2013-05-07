#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/SoPath.h>

#include <Inventor/collision/SoDualSceneCollider.h>
#include <Inventor/STL/iostream>
#include <Inventor/STL/vector>

#include <Inventor/SoMacApp.h>

/////////////////////////////////////////////////////////////
class  GettingIntersectionPoints : public SoDualSceneCollider 
{
private:
  mutable std::vector<SbVec3f> common_p0;
  mutable std::vector<SbVec3f> common_p1;
public :
  SbBool checkCollision() const 
  {
    common_p0.clear();
    common_p1.clear();
    SbBool result = SoDualSceneCollider::checkCollision();
    printCommonPoints();
    return result;
  }	
  SbBool searchNextIntersection() const 
  {
    SbVec3f p0,p1;
    getCommonPoints(p0,p1);
    common_p0.push_back(p0);
    common_p1.push_back(p1);
    return TRUE; // yes, search again other intersections !
  }
private:
  void printCommonPoints() const
  {
    SbVec3f p0,p1;
    for (int i=0; i<common_p0.size(); i++) {
      p0 = common_p0[i];
      p1 = common_p1[i];
      printf("point pair (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n",
                p0[1],p0[1],p0[2],p1[1],p1[1],p1[2]);
    }
    printf("Number of triangle intersection %d\n",common_p1.size());
  }
};

/////////////////////////////////////////////////////////////
int
main(int, char **argv)
{
  // Initialize Inventor
  SoDB::init();

  SoSeparator *root = new SoSeparator;
  SoSeparator *trss = new SoSeparator; // transforming scene separator
  SoTransform *transform = new SoTransform;
  SoSphere *sphere = new SoSphere;
  SoComplexity *complexity = new SoComplexity;

  transform->translation = SbVec3f(0.5f,0.5f,0.5f);
  complexity->value.setValue(1);
  
  root->ref();
  root->addChild(trss);
  {
    trss->addChild(transform);
    trss->addChild(sphere);
  }
  root->addChild(sphere);
   
  SoPath *static_path = new SoPath(root);
  static_path->append(sphere);
      
  SoPath *moving_path = new SoPath(root);
  moving_path->append(trss);
  moving_path->append(sphere);
    
  GettingIntersectionPoints ci;
  ci.setStaticScene(static_path);
  ci.setMovingScene(moving_path);
  ci.checkCollision();

#ifdef WIN32
  system("PAUSE");
#endif
  return 0;
}
