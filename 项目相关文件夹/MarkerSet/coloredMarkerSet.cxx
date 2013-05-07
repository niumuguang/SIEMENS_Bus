#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoMarker.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/actions/SoWriteAction.h>

#define NUM_MARKERS     8
#define NUM_MARKER_TYPE 92

int
main(int, char **argv)
{
  // Initialize Inventor and Xt
  Widget myWindow = SoXt::init(argv[0]);

  // Array containing the coordinates of the markers.
  SbVec3f markersCoords[NUM_MARKERS] = {
    SbVec3f(0.5f, 0.5f, 0.0f),
    SbVec3f(1.5f, 2.5f, 0.0f), 
    SbVec3f(1.8f, 1.2f, 0.0f),
    SbVec3f(2.4f, 1.9f, 0.0f), 
    SbVec3f(3.2f, 6.3f, 0.0f),
    SbVec3f(4.5f, 7.2f, 0.0f),
    SbVec3f(6.3f, 9.3f, 0.0f),
    SbVec3f(6.9f, 7.2f, 0.0f)
  };

  // Array containing the colors of the markers.
  SbColor markersColors[NUM_MARKERS] = {
    SbColor(1,0,0),
    SbColor(0,1,0),
    SbColor(0,0,1),
    SbColor(1,1,1),
    SbColor(1,1,0),
    SbColor(1,0,1),
    SbColor(0,1,1),
    SbColor(0,0,0)
  };

  // Array containing the indices of the markers.
  int32_t markersIndexes[NUM_MARKERS] =
  {
    91,
    91,
    91,
    90,
    SoMarkerSet::SQUARE_FILLED_9_9,
    SoMarkerSet::DIAMOND_FILLED_9_9,
    SoMarkerSet::TRIANGLE_FILLED_9_9,
    92
  };

  // Array containing a new marker "colored target"
  static unsigned char myMarker1[] = {
    0x70, 0x0, 0x8c, 0x01,
    0x22, 0x02, 0x52, 0x02,
    0x89, 0x04, 0x05, 0x05,
    0x89, 0x04, 0x52, 0x02,
    0x22, 0x02, 0x8c, 0x01,
    0x70, 0x0 };

  static uint32_t myMarkerColorMask1[] = {
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF000000, 0xFF000000, 0xFF,       0xFFFF0000, 0xFF,       0xFF000000, 0xFF000000, 0xFF,       0xFF,
    0xFF, 0xFF000000, 0xFF000000, 0xFF,       0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFF,       0xFF000000, 0xFF000000, 0xFF, 
    0xFF, 0xFF000000, 0xFF,       0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFF,       0xFF000000, 0xFF, 
    0xFF, 0xFF000000, 0xFF000000, 0xFF,       0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFF,       0xFF000000, 0xFF000000, 0xFF, 
    0xFF, 0xFF,       0xFF000000, 0xFF000000, 0xFF,       0xFFFF0000, 0xFF,       0xFF000000, 0xFF000000, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF000000, 0xFF000000, 0xFF000000, 0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF
  };

  static uint32_t myMarkerColorMask2[] = {
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF00001F, 0xDF00005F, 0xFF,       0xFFFF007F, 0xFF,       0xDF00005F, 0xDF00005F, 0xFF,       0xFF,
    0xFF, 0xDF00005F, 0xFF00001F, 0xFF,       0xFFFF007F, 0xFFFF007F, 0xFFFF007F, 0xFF,       0xDF00005F, 0xDF00005F, 0xFF, 
    0xFF, 0xDF00005F, 0xFF,       0xFFFF007F, 0xFFFF007F, 0xFFFF007F, 0xFFFF007F, 0xFFFF007F, 0xFF,       0xDF00005F, 0xFF, 
    0xFF, 0xDF00005F, 0xDF00005F, 0xFF,       0xFFFF007F, 0xFFFF007F, 0xFFFF007F, 0xFF,       0xDF00005F, 0xDF00005F, 0xFF, 
    0xFF, 0xFF,       0xDF00005F, 0xDF00005F, 0xFF,       0xFFFF007F, 0xFF,       0xDF00005F, 0xDF00005F, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xDF00005F, 0xDF00005F, 0xDF00005F, 0xFF,       0xFF,       0xFF,       0xFF, 
    0xFF, 0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF,       0xFF
  };
  
  SoMarkerSet::addMarker(90, SbVec2s(11, 11), myMarker1);
  SoMarker* marker = new SoMarker;
  marker->size.setValue(9,9);
  marker->upToDown = TRUE;
  marker->isLSBFirst = FALSE;
  marker->bitmap.setNum(18);
  uint8_t* bitmap_ptr = marker->bitmap.startEditing();
  *bitmap_ptr++ = 0x00; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x00; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x76; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x76; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x08; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x08; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x63; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x3e; *bitmap_ptr++ = 0x00;
  *bitmap_ptr++ = 0x00; *bitmap_ptr++ = 0x00;
  marker->bitmap.finishEditing();
  SoMarkerSet::addMarker(91, marker);

  // Add the new markers.
  //SoMarkerSet::addMarker(91, SbVec2s(11, 11), myMarker1, myMarkerColorMask1);
  SoMarkerSet::addMarker(92, SbVec2s(11, 11), myMarker1, myMarkerColorMask2);

  SoSeparator *root = new SoSeparator;

  root->ref();

  // Sets the coordinates of the markers.
  SoCoordinate3 *coords = new SoCoordinate3;
  coords->point.setValues(0, NUM_MARKERS, markersCoords);

  // Sets the indices of the markers.
  SoMarkerSet* markerSet = new SoMarkerSet;
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

    // Add marker to SceneGraph in order to export them in IV file.
  root->addChild(SoMarker::getMarkerDesc(90));
  root->addChild(SoMarker::getMarkerDesc(91));
  root->addChild(SoMarker::getMarkerDesc(92));

  // Create a viewer
  SoXtExaminerViewer *myViewer;
  myViewer = new SoXtExaminerViewer(myWindow);

  // attach and show viewer   
  myViewer->setSceneGraph(root);
  myViewer->setTitle("Marker");

  myViewer->show();
  myViewer->viewAll();


  SoOutput out;
  SoWriteAction wa(&out);
  out.openFile("markerOutput.iv");
  wa.apply(root);
  out.closeFile();

  // Loop forever
  SoXt::show(myWindow);
  SoXt::mainLoop();
  delete myViewer;
  root->unref();
  SoXt::finish();

  return 0;
}
