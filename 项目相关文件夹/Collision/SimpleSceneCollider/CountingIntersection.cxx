#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/SoPath.h>

#include <Inventor/collision/SoDualSceneCollider.h>
#include <Inventor/STL/iostream>

#include <Inventor/SoMacApp.h>

/////////////////////////////////////////////////////////////
class  CountingIntersection : public SoDualSceneCollider 
{
 private:
    mutable int num_ti; // num intersecting pair of triangle.
 public:
  SbBool checkCollision() const 
    {
      num_ti = 0;
      SbBool result = SoDualSceneCollider:: checkCollision();
      std::cout << "Number of triangle intersection " << num_ti << std::endl;
      return result;
    }	

  SbBool searchNextIntersection() const 
    {
      num_ti++;
      return TRUE; // yes, search again other intersections !
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
    
  CountingIntersection ci;
  ci.setStaticScene(static_path);
  ci.setMovingScene(moving_path);
  ci.checkCollision();

#ifdef WIN32
  system("PAUSE");
#endif
  return 0;
}










