#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/actions/SoWriteAction.h>

#define NUM_MARKERS     10
#define NUM_MARKER_TYPE 93

int
main(int, char **argv)
{
  // Initialize Inventor and Xt
  Widget myWindow = SoXt::init(argv[0]);

  // Array containing the coordinates of the markers.
  SbVec3f markersCoords[NUM_MARKERS] = {
    SbVec3f(0.5f, 0.5f, 0.0f), SbVec3f(1.5f, 3.8f, 0.0f), 
    SbVec3f(1.8f, 1.2f, 0.0f), SbVec3f(2.4f, 1.9f, 0.0f), 
    SbVec3f(3.2f, 6.3f, 0.0f), SbVec3f(4.5f, 7.2f, 0.0f),
    SbVec3f(6.3f, 9.3f, 0.0f), SbVec3f(6.9f, 7.2f, 0.0f), 
    SbVec3f(8.0f, 6.0f, 0.0f), SbVec3f(8.5f, 6.3f, 0.0f)
  };

  // Array containing the colors of the markers.
  SbColor markersColors[NUM_MARKERS] = {
    SbColor(1,0,0), SbColor(0,1,0), 
    SbColor(0,0,1), SbColor(1,1,1), 
    SbColor(1,1,0), SbColor(1,0,1), 
    SbColor(0,1,1), SbColor(1,0,0), 
    SbColor(0,1,0), SbColor(0,0,1)
  };

  // Array containing the indices of the markers.
  int32_t markersIndexes[NUM_MARKERS] = {
    90, 91, 92, SoMarkerSet::CIRCLE_FILLED_9_9, SoMarkerSet::SQUARE_FILLED_9_9,
    SoMarkerSet::DIAMOND_FILLED_9_9, SoMarkerSet::TRIANGLE_FILLED_9_9,
    SoMarkerSet::RHOMBUS_FILLED_9_9, SoMarkerSet::HOURGLASS_FILLED_9_9
  };

  // Array containing a new marker "arrow"
  static unsigned char arrowBits[] = {
    0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff,
    0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01,
    0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00,
    0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff,
    0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00,
    0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff,
    0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01,
    0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00,
    0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff,
    0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0x01,
    0xfe, 0xff, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x3f, 0x00, 0xe0, 0xff,
    0xff, 0x0f, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0xff, 0xff, 0x01,
    0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00,
    0xe0, 0x07, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00
  };

  // Array containing a new marker "target"
  static unsigned char targetBits[] = {
    0xe0, 0x0f, 0x00, 0x18, 0x31, 0x00, 0x04, 0x41, 0x00, 0xc2, 0x87, 0x00,
    0x22, 0x89, 0x00, 0x11, 0x11, 0x01, 0x09, 0x21, 0x01, 0x09, 0x21, 0x01,
    0xff, 0xff, 0x01, 0x09, 0x21, 0x01, 0x09, 0x21, 0x01, 0x11, 0x11, 0x01,
    0x22, 0x89, 0x00, 0xc2, 0x87, 0x00, 0x04, 0x41, 0x00, 0x18, 0x31, 0x00,
    0xe0, 0x0f, 0x00
  };

  // Array containing a new marker "tgs"
  static unsigned char tgsBits[] = {
    0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x04, 0x11, 0x11,
    0x04, 0x01, 0x01, 0x04, 0x1d, 0x1f, 0x04, 0x11, 0x10, 0x04, 0x11, 0x11,
    0x04, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x1f
  };

  // Add the new markers.
  SoMarkerSet::addMarker(90, SbVec2s(17, 17), targetBits);
  SoMarkerSet::addMarker(91, SbVec2s(33, 33), arrowBits);
  SoMarkerSet::addMarker(92, SbVec2s(21, 11), tgsBits);

  SoSeparator *root = new SoSeparator;

  root->ref();

  // Sets the coordinates of the markers.
  SoCoordinate3 *coords = new SoCoordinate3;
  coords->point.setValues(0, NUM_MARKERS, markersCoords);

  // Sets the indices of the markers.
  SoMarkerSet  *markerSet = new SoMarkerSet;
  markerSet->markerIndex.setValues(0, NUM_MARKERS, markersIndexes);

  SoMaterialBinding *matBind = new SoMaterialBinding;
  matBind->value = SoMaterialBinding::PER_VERTEX;

  // Sets the color of the markers.
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor.setValues(0, NUM_MARKERS, markersColors);

  root->addChild(coords);
  root->addChild(new SoLineSet);
  root->addChild(matBind);
  root->addChild(mat);
  root->addChild(markerSet);

  // Create a viewer
  SoXtExaminerViewer *myViewer;
  myViewer = new SoXtExaminerViewer(myWindow);

  // attach and show viewer   
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Marker");
  //myViewer->setSize(SbVec2s(554, 380));

  myViewer->show();
  myViewer->viewAll();

  // Loop forever
  SoXt::show(myWindow);
  SoXt::mainLoop();

  root->unref();
  delete myViewer;
  SoXt::finish();

  return 0;
}
