#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>

/////////////////////////////////////////////////////////////////
// Example of custom node that redefines behavior of actions
// by using an alternate representation
/////////////////////////////////////////////////////////////////
class SoCustomCone : public SoCone
{
  SO_NODE_HEADER(SoCustomCone);

public:
  // Constructor
  SoCustomCone()
  {
    SO_NODE_CONSTRUCTOR( SoCustomCone );
  }

  // Custom node database initializer
  static void initClass()
  {
    SO__NODE_INIT_CLASS(SoCustomCone, "CustomCone", SoCone);
  }
  
  // Custom node database cleaner
  static void exitClass()
  {
    SO__NODE_EXIT_CLASS(SoCustomCone);
  }

protected:
  // Destructor
  virtual ~SoCustomCone()
  {};

  // depending on action type, provides an alternate representation
  virtual SoNode* getAlternateRep(SoAction *action)
  {
    // for rendering we will use an SoCube
    if ( action->isOfType(SoGLRenderAction::getClassTypeId()) )
    {
      return new SoCube();
    }
    // for writing to a file or buffer we use a sphere
    else if ( action->isOfType(SoWriteAction::getClassTypeId()) )
    {
      return new SoSphere();
    }
    // for callback action we use a real Cone !!
    else if ( action->isOfType(SoCallbackAction::getClassTypeId()) )
    {
      return new SoCone();
    }
    // for all others action we keep our own representation
    else
    {
      return this;
    }
  }
};
SO_NODE_SOURCE(SoCustomCone);



///////
/////// Main example
///////
int
main(int, char **argv)
{
  Widget myWindow = SoXt::init(argv[0]);
  if (myWindow == NULL)
    exit(1);
  SoCustomCone::initClass();

  SoSeparator *root = new SoSeparator;
  root->ref();

  // Material
  SoMaterial *myMaterial = new SoMaterial;
  myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
  root->addChild(myMaterial);

  // Custom Cone
  root->addChild(new SoCustomCone);

  
  // Set up viewer:
  SoXtExaminerViewer *myViewer = new SoXtExaminerViewer(myWindow);
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Examiner Viewer");
  myViewer->show();
  
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();

  delete myViewer;
  
  SoCustomCone::exitClass();
  SoXt::finish();
  return 0;
}
