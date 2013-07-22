// LargeModelViewerDoc.cpp : implementation of the CLargeModelViewerDoc class
//

/*
 * Portions from SoSceneViewer Demo program
 * Copyright 1991-1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 */

#include "stdafx.h"
#include "LargeModelViewer.h"
#include "MainFrm.h"

#include "LargeModelViewerDoc.h"

#include "OctreeOrderingDialog.h"
#include "ValueOrderingDialog.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoValueOrdering.h>
#include <Inventor/nodes/SoOctreeOrdering.h> 
#include <Inventor/nodes/SoLevelOfSimplification.h> 
#include <Inventor/actions/SoReorganizeAction.h>
#include <Inventor/actions/SoShapeSimplifyAction.h>
#include <Inventor/actions/SoGlobalSimplifyAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/simplifier/SoDecimator.h> 
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h> 
#include <Inventor/VRMLnodes/SoVRMLTimeSensor.h> 

//jh added - 1/10/2001
#include <Inventor/actions/SoSplitGeometryAction.h>
#include "InlineLoader.h"

#include <string.h>

#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc

IMPLEMENT_DYNCREATE(CLargeModelViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CLargeModelViewerDoc, CDocument)
    //{{AFX_MSG_MAP(CLargeModelViewerDoc)
    ON_COMMAND(IDM_EDIT_OCTREE_ORD, OnEditOctreeOrdering)
    ON_COMMAND(IDM_EDIT_VALUE_ORD, OnEditValueOrdering)
	ON_COMMAND(IDM_LOAD_IMMEDIATE, OnLoadImmediate)
	ON_COMMAND(IDM_LOAD_FIND_VRML, OnLoadFindVrml)
	ON_COMMAND(IDM_VIEW_BACKFACE, OnViewBackface)
	ON_COMMAND(IDM_VIEW_SMOOTHING, OnViewSmoothing)
	ON_COMMAND(IDM_VIEW_NORMALIZE, OnViewNormalize)
	ON_COMMAND(IDM_LOAD_SIMP_SHAPE, OnLoadSimpShape)
	ON_COMMAND(IDM_LOAD_SIMP_GLOBAL, OnLoadSimpGlobal)
	ON_COMMAND(IDM_LOAD_IVFIX, OnLoadIvfix)
	ON_COMMAND(IDM_LOAD_SIMP_SHAPE_TRIS, OnLoadSimpShapeTris)
	ON_COMMAND(IDM_LOAD_SIMP_GLOBAL_TRIS, OnLoadSimpGlobalTris)
	ON_UPDATE_COMMAND_UI(IDM_TRAV_OCTREE_ORD, OnUpdateTravOctreeOrd)
	ON_UPDATE_COMMAND_UI(IDM_TRAV_VALUE_ORD, OnUpdateTravValueOrd)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_SIMP_GLOBAL, OnUpdateLoadSimpGlobal)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_SIMP_GLOBAL_TRIS, OnUpdateLoadSimpGlobalTris)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_SIMP_SHAPE, OnUpdateLoadSimpShape)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_SIMP_SHAPE_TRIS, OnUpdateLoadSimpShapeTris)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_OCTREE_ORD, OnUpdateEditOctreeOrd)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_VALUE_ORD, OnUpdateEditValueOrd)
	ON_COMMAND(IDM_LOAD_SPLIT_SHAPE, OnLoadSplitShape)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
CLargeModelViewerDoc::noNormalizeCB(void *thisp, SoAction*action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())){
        if (((CLargeModelViewerDoc*)thisp)->noNormalize)
			glDisable(GL_NORMALIZE);
	else glEnable(GL_NORMALIZE);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc construction/destruction

CLargeModelViewerDoc::CLargeModelViewerDoc()
{
    isLMVLicensed = TRUE;

    sceneGraph = new SoSeparator();
    sceneGraph->ref();

    // Inline Fetcher is used to resolve WWWInlines on local file system
    inlineLoader = new InlineLoader;
    inlineLoader->setReadingInlineCallback(readingInlineCB, this);

    createLightsCameraEnvironment();
    sceneGraph->addChild(lightsCameraEnvironment);
 
    // Turn off GL_NORMALIZE:
    callback = new SoCallback;
    callback->setCallback(noNormalizeCB, this);	
    noNormalize = FALSE;
    sceneGraph->addChild(callback); 
    

    // Shape Hints node for controlling back-face culling:
    shapeHints = new SoShapeHints;
    shapeHints->shapeType = SoShapeHints::SOLID;
    shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapeHints->faceType.setIgnored(TRUE);
    shapeHints->creaseAngle = 0.5;
    shapeHints->creaseAngle.setIgnored(TRUE);
    sceneGraph->addChild(shapeHints);
    backfaceCulling = TRUE;
    smoothing = FALSE;
    delayedTransparency = FALSE;
    findVRMLTimers = TRUE;
    isReorganize = FALSE;  
    isSimplify = LMV_SIMP_NONE;
    


    // Create all ordering nodes now so that we 
    // can just switch between them, keeping their field values
    // consistent.
    sepOrdering = new SoSeparator;
    sepOrdering->ref();
    valueOrdering = new SoValueOrdering;
    valueOrdering->ref();
    // Set a lower default drop value for ValueOrdering:
    valueOrdering->dropScreenArea = 3;

    octreeOrdering = new SoOctreeOrdering;
    octreeOrdering->ref();
    
    octreeOrderingDlg = new COctreeOrderingDialog;
    valueOrderingDlg  = new CValueOrderingDialog;

    
    // Put our ordering node in here after camera and before
    // selection:
    orderingNode = sepOrdering;    	// use Separator as default
    sceneGraph->addChild(orderingNode);

    selection = new SoSelection;
    orderingNode->addChild(selection);
    
     
    // Create the dictionary that keeps track of original camera
    // settings in scenekits
    sceneKitCamNumDict = new SbDict(5);
    switchOffSceneKitCameras(selection);
    removeCameras(selection);

     //
    // Selection
    //     These callbacks are used to update the SceneViewer state after
    //     the current selection changes (e.g. attach/detach editors and manips).
    //
    //selection->addSelectionCallback(CLargeModelViewerDoc::selectionCallback, this);
    //selection->addDeselectionCallback(CLargeModelViewerDoc::deselectionCallback, this);
    selection->setPickFilterCallback(CLargeModelViewerDoc::pickFilterCB, this);

         
    // If scene had sceneKit with a camera, copy its values into our camera.
    //if (camFromScene)
    //	setCameraFields( camFromScene );  // call on View class
    
}

CLargeModelViewerDoc::~CLargeModelViewerDoc()
{
    sceneGraph->unref();
    delete inlineLoader;
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc serialization

void CLargeModelViewerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc diagnostics

#if defined(_DEBUG)
void CLargeModelViewerDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CLargeModelViewerDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc private support functions
//    based on SGI SoSceneViewer code



////////////////////////////////////////////////////////////////////////
//
// Description:
//	detach everything and nuke the existing scene.
//
// Use: private
void
CLargeModelViewerDoc::deleteScene()
//
////////////////////////////////////////////////////////////////////////
{
     //deselect everything (also detach manips)
     selection->deselectAll();
     
     // remove the geometry under the selection node
     for (int i = selection->getNumChildren(); i>0; i--)
     selection->removeChild(i-1);
     
     // clear memeory of InlineLoader:
     //inlineLoader->ClearLists();
      	


}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Read OpenInventor file using SoDB::readAll()
//
// Use: private

SoSeparator* 
CLargeModelViewerDoc::readOIVFile(const char *filename)
{
   // Open the input file
   SoInput mySceneInput;
   if (!mySceneInput.openFile(filename)) {
      //fprintf(stderr, "Cannot open file %s\n", filename);
      return NULL;
   }

   
    // Set up directory to help SoInput find the rest of the included files:
    size_t pos, len = strlen(filename);

    for (pos=len-1; pos>=0; pos--) {
#ifdef _WIN32
             if (filename[pos] == '\\') break;
#else
			 if (filename[pos] == '/') break;
#endif
    }
    if (pos > 0) {
           // break off path from filename, and
           // give to SoInput as place to look first:
           char *dir = new char [len+1];
           strncpy(dir, filename, pos);
           dir [pos] = '\0';
           SoInput::addDirectoryFirst(dir);

           delete [] dir;
    }
      

   // Read the whole file into the database
   SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
   if (myGraph == NULL) {
      //fprintf(stderr, "Problem reading file\n");
      return NULL;
   } 

   mySceneInput.closeFile();
   return myGraph;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Reads the given file and insert the geometry under the selection
//  node. If the node didn't have any children, the viewAll() method is
//  automatically called.
//
// Use: private
SbBool
CLargeModelViewerDoc::insertFile(const char *filename)
//
////////////////////////////////////////////////////////////////////////
{    
    SbBool hadNoChildren = (selection->getNumChildren() == 0);
    
   // Find directory from full path in order to know where to 
   // look for WWWInlines later.
   //size_t pos, len = strlen(filename);

   //for (pos=len-1; pos>=0; pos--) 
   //     if ((filename[pos] == '\\') || (filename[pos] == '/')) break;

   //if (pos > 0) {
   //     // break off path from filename, and
   //     // give to SoInput as place to look first:
   //     char *dir = new char [len+1];
   //     strncpy(dir, filename, pos);
   //     dir [pos] = '\0';
   //     SoInput::addDirectoryFirst(dir);
   //         
   //     delete [] dir;          
   //}
	size_t pos, len = strlen(filename);
	char *dir = new char [len+1];
   SoInput::addDirectoryFirst(dir);
    // Try reading the file
    SbString errorMessage;
    SoSeparator *sep = readOIVFile(filename);
    
    if (sep == NULL) {
        //SoXt::createSimpleErrorDialog(mgrWidget, "File Error Dialog", 
        //      (char *) errorMessage.getString());
        return FALSE;
    }
    
    // Do the preprocessing of the scene graph before putting under the
    // Selection node, because if one of the Ordering nodes is being
    // used. it will take a long time to do the processing.  The
    // ordering nodes are sensitive to scene graph changes.
    sep->ref();
    
    // Search just the first file for animations, if VRML2.0 animations
    // are in the sub files, then this should be done after all of
    // the inlines are loaded.  But for performance reasons in the general
    // case, I am only going to check the first file:
    //if (findVRMLTimers)
    	//processAnimations(sep);
       
    // Process scene graph to handle Inlines:
   // inlineLoader->FixWWWInlines(sep);
        
    // Force in all of the empty WWWInlines:
   // inlineLoader->FetchAllEmpty();
    
     
    // remove any cameras under selection which were just added
    //switchOffSceneKitCameras(sep);
    //removeCameras(sep);
    
    // add nodes under selection, not sceneGraph
    selection->addChild(sep);
 
    // now, undo our extra ref():
    sep->unref();

    // Reorganize scene if requested:
    /*if (reorganize != LMV_IVFIX_NONE) {
        reorganizeScene(SbBool(reorganize == LMV_IVFIX_SIMP));	
    } */

    if (hadNoChildren) {
        // viewAll(); // On View class?
        //saveHomePosition();     // On ViewClass ?
    }

    // If scene had sceneKit with a camera, copy its values into our camera.
    //if (camFromScene)
    //	setCameraFields( camFromScene );      // on View Class
    
    return TRUE;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Switches type of ordering node used
//   (Separator, ValueOrdering, OctreeOrdering)
//
// Use: private
void
CLargeModelViewerDoc::switchOrderingNode(CLargeModelViewerDoc::Ordering whichType)
//
////////////////////////////////////////////////////////////////////////
{
    
    SoSeparator *newOrdering= NULL;
    switch (whichType) {
        default:
        case LMV_ORD_SEPARATOR:
            // Need to check for exact type match, because
            // all are SoSeparators:
            if (orderingNode->getTypeId() == 
                SoSeparator::getClassTypeId()) 
                return;
                
            newOrdering = sepOrdering;
            break;
        case LMV_ORD_VALUE_ORDERING:
            if (orderingNode->isOfType(
                SoValueOrdering::getClassTypeId())) 
                return;
                
            newOrdering = valueOrdering;
            break;
        
        case LMV_ORD_OCTREE_ORDERING:
            if (orderingNode->isOfType(
                SoOctreeOrdering::getClassTypeId())) 
                return;
                
            newOrdering = octreeOrdering;
            break;
    
    }
    
    // return if no change needed:
    if (newOrdering==NULL) return;
    
    // Unmanage dialogs that are not needed any more:
/*	if (orderingNode->isOfType(SoValueOrdering::getClassTypeId())) {
        if (valueOrderingDialog)
            XtUnmanageChild(valueOrderingDialog);
    }
    else if (orderingNode->isOfType(SoOctreeOrdering::getClassTypeId())) {
        if (octreeOrderingDialog)
            XtUnmanageChild(octreeOrderingDialog);
    } */
    
    // remove ordering from graph
    selection->ref();
    orderingNode->removeChild(selection);  // orderingNode clears scene here
    sceneGraph->removeChild(orderingNode); // orderingNode has been ref()'ed
                
    sceneGraph->addChild(newOrdering);
    newOrdering->addChild(selection);
    selection->unref();
    orderingNode = newOrdering;
            
                
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Create the lights and camera environment structure.
//
// Use: private
//
void
CLargeModelViewerDoc::createLightsCameraEnvironment()
//
////////////////////////////////////////////////////////////////////////
{
     
    lightsCameraEnvironment = new SoGroup;
    environment             = new SoEnvironment;
    lightGroup              = new SoGroup;
    envLabel            = new SoLabel;

    //envLabel->label.setValue(SV_ENV_LABEL);
    lightsCameraEnvironment->addChild(envLabel);
    lightsCameraEnvironment->addChild(new SoPerspectiveCamera);
    lightsCameraEnvironment->addChild(environment);
    lightsCameraEnvironment->addChild(lightGroup);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	This will:
//        [a] find the last sceneKit camera that is being looked through
//            in the scene and return  a pointer to it.
//
// Use: private
SoCamera *
CLargeModelViewerDoc::getLastSceneKitCamera(SoGroup *root)
//
////////////////////////////////////////////////////////////////////////
{
    // Find scene kits under the root node and loop backwards:
    SoSearchAction sa;
    sa.setType(SoSceneKit::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);

    SoPathList paths = sa.getPaths();
    for (int i = paths.getLength() - 1; i >= 0; i--) {
    SoPath *p = paths[i];
    if (p->getTail()->isOfType( SoSceneKit::getClassTypeId() ) ) {
        SoSceneKit *sceneKit = (SoSceneKit *) p->getTail();
        int camNum = sceneKit->getCameraNumber();

        SoNodeKitListPart *camList = 
        (SoNodeKitListPart *) sceneKit->getPart("cameraList",FALSE);
        if (camList && camNum >= 0) {
            SoCameraKit *camKit 
            = (SoCameraKit *)camList->getChild( camNum );
        if (camKit)
            return( (SoCamera *) camKit->getPart( "camera", FALSE ));
        }
    }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This will:
//        [a] find the last sceneKit, its camera number, and replace the
//            given camera for that node.
//
// Use: private
void
CLargeModelViewerDoc::setLastSceneKitCamera(SoGroup *root, SoCamera *theCam)
//
////////////////////////////////////////////////////////////////////////
{
    // Find scene kits under the root node and loop backwards:
    SoSearchAction sa;
    sa.setType(SoSceneKit::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);

    SoPathList paths = sa.getPaths();
    for (int i = paths.getLength() - 1; i >= 0; i--) {
    SoPath *p = paths[i];
    if (p->getTail()->isOfType( SoSceneKit::getClassTypeId() ) ) {
        SoSceneKit *sceneKit = (SoSceneKit *) p->getTail();
        int camNum = sceneKit->getCameraNumber();

        SoNodeKitListPart *camList = 
        (SoNodeKitListPart *) sceneKit->getPart("cameraList",FALSE);
        if (camList && camNum >= 0) {
            SoCameraKit *camKit 
            = (SoCameraKit *)camList->getChild( camNum );
        if (camKit) {
            camKit->setPart("camera",theCam);
            return;
        }
        }
    }
    }
    return;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This will:
//        [a] find scene kits if under the root node.
//        [b] Save the camera number settings of the kits in a dictionary.
//        [c] Switch off the scenekit cameras.
//
// Use: private
void
CLargeModelViewerDoc::switchOffSceneKitCameras(SoGroup *root)
//
////////////////////////////////////////////////////////////////////////
{
    // [a] find scene kits if under the root node.
    SoSearchAction sa;
    sa.setType(SoSceneKit::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);
    
    // Deal with each kit:
    SoPathList paths = sa.getPaths();
    for (int i = paths.getLength() - 1; i >= 0; i--) {
    SoPath *p = paths[i];
    if (p->getTail()->isOfType( SoSceneKit::getClassTypeId() ) ) {
        SoSceneKit *sceneKit = (SoSceneKit *) p->getTail();
        int *camNumPtr = new int;
        *camNumPtr = sceneKit->getCameraNumber();

        // [b] Save the camera number settings of the kits in a dictionary.
        sceneKitCamNumDict->enter((uintptr_t)sceneKit,(void *)camNumPtr);

        // [c] Switch off the scenekit cameras.
        sceneKit->setCameraNumber( SO_SWITCH_NONE );
    }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This will:
//        [a] find scene kits if under the root node.
//        [b] Find the camera number settings of the kits from a dictionary.
//        [c] Restore the sceneKit camera numbers.
//
// Use: private
void
CLargeModelViewerDoc::restoreSceneKitCameras(SoGroup *root)
//
////////////////////////////////////////////////////////////////////////
{
    // [a] find scene kits if under the root node.
    SoSearchAction sa;
    sa.setType(SoSceneKit::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);
    
    // Deal with each kit:
    SoPathList paths = sa.getPaths();
    for (int i = paths.getLength() - 1; i >= 0; i--) {
    SoPath *p = paths[i];
    if (p->getTail()->isOfType( SoSceneKit::getClassTypeId() ) ) {
        SoSceneKit *sceneKit = (SoSceneKit *) p->getTail();
        void *myPtr;
        int camNum;
        if (sceneKitCamNumDict->find((uintptr_t)sceneKit, myPtr))
        camNum = *((int *)myPtr);
        else
        camNum = SO_SWITCH_NONE;

        sceneKit->setCameraNumber( camNum );
    }
    }
}

/*

////////////////////////////////////////////////////////////////////////
//
// Description:
// Copies from given camera into this viewer's camera, if types match.
//
// Use: private
void        
CLargeModelViewerDoc::setCameraFields( SoCamera *fromCam )
//
////////////////////////////////////////////////////////////////////////
{
    SoCamera *myCam = getCamera(); 

    if ( ! myCam || ! fromCam )
    return;

    // Types must match!
    if (  myCam->getTypeId() != fromCam->getTypeId() )
    return;

    myCam->viewportMapping = fromCam->viewportMapping.getValue();
    myCam->position = fromCam->position.getValue();
    myCam->orientation = fromCam->orientation.getValue();
    myCam->aspectRatio = fromCam->aspectRatio.getValue();
    myCam->nearDistance = fromCam->nearDistance.getValue();
    myCam->farDistance = fromCam->farDistance.getValue();
    myCam->focalDistance = fromCam->focalDistance.getValue();

    if (   myCam->isOfType( SoPerspectiveCamera::getClassTypeId() )) {
    ((SoPerspectiveCamera *)myCam)->heightAngle 
        = ((SoPerspectiveCamera *)fromCam)->heightAngle.getValue();
    }
    else if (   myCam->isOfType( SoOrthographicCamera::getClassTypeId() )) {
    ((SoOrthographicCamera *)myCam)->height 
        = ((SoOrthographicCamera *)fromCam)->height.getValue();
    }
}

*/

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This will remove any cameras under root.
//
// Use: private
void
CLargeModelViewerDoc::removeCameras(SoGroup *root)
//
////////////////////////////////////////////////////////////////////////
{
    SoSearchAction sa;
    sa.setType(SoCamera::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);
    
    // remove those cameras!
    SoPathList paths = sa.getPaths();
    for (int i = 0; i < paths.getLength(); i++) {
        SoPath *p = paths[i];
        int cam = p->getIndexFromTail(0);
        SoGroup  *group = (SoGroup *) p->getNodeFromTail(1);
        group->removeChild(cam);
    
        // See if the parent of this camera is a switch node with whichChild set
        // to render the camera. With the camera gone, the switch has nothing
        // to render.
        if (group->isOfType(SoSwitch::getClassTypeId())) {
            SoSwitch *sw = (SoSwitch *) group;
            if (sw->whichChild.getValue() == cam)
            sw->whichChild.setValue(SO_SWITCH_NONE);
        }
    }
}


 
////////////////////////////////////////////////////////////////////////
//
//  Manage the changes in the selected node(s)
//
//  Use: private, static
//
SoPath *
CLargeModelViewerDoc::pickFilterCB(void *,//userData, 
                                   const SoPickedPoint *pick)
//
////////////////////////////////////////////////////////////////////////
{
    SoPath *filteredPath = NULL;
    
// BA 3/98 - added new filter to handle LODs and WWWInlines:
    SoPath *p = pick->getPath();
    int lastWWWInline = -1;
    int endIndex = -1;
    for (int i=0; i<p->getLength(); i++) {
        SoNode *node = p->getNode(i);
        if (node->isOfType(SoWWWInline::getClassTypeId())) {
            // rememeber last WWWInline:
            lastWWWInline = i;
        }
        else if (node->isOfType(SoLOD::getClassTypeId())) {
            // stop either here, or at WWWInline above us:
            if (lastWWWInline < 0) endIndex = i;
            else endIndex = lastWWWInline;
            
            break;
        }
    }
    
    if (endIndex >= 0) filteredPath = p->copy(0, endIndex);
    else filteredPath = p;
    return filteredPath;
}

void 
CLargeModelViewerDoc::readingInlineCB(const char *filename, 
                                      void *, InlineLoader *)
{
  // Use pane for where number of primitives will appear
  // to display when we are reading an inline file:
  SbString infoString("Reading inline: ");

  infoString += filename;
  ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(infoString.getString());
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Switches on or off backface culling by ignoring
//  certain fields in our ShapeHints node near the top of the 
//  scene graph.
//
// Use: private
void
CLargeModelViewerDoc::toggleBackfaceCulling(SbBool onOff)
//
////////////////////////////////////////////////////////////////////////
{
    shapeHints->vertexOrdering.setIgnored(!onOff);
    shapeHints->shapeType.setIgnored(!onOff);
    backfaceCulling = onOff;
    CMenu* pMenu = AfxGetMainWnd()->GetMenu();
    pMenu->CheckMenuItem( IDM_VIEW_BACKFACE, 
	    backfaceCulling ? MF_CHECKED : MF_UNCHECKED);


}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Switches on or off smooth normals by ignoring
//  the creaseAngle field in our ShapeHints node near the top of the 
//  scene graph.
//
// Use: private
void
CLargeModelViewerDoc::toggleSmoothing(SbBool onOff)
//
////////////////////////////////////////////////////////////////////////
{
    shapeHints->creaseAngle.setIgnored(!onOff);
    smoothing = onOff;
    CMenu* pMenu = AfxGetMainWnd()->GetMenu();
    pMenu->CheckMenuItem( IDM_VIEW_SMOOTHING, 
	    smoothing ? MF_CHECKED : MF_UNCHECKED);
	
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Enable and Disables GL_NORMALIZE, through Callback node
//
// Use: private
void
CLargeModelViewerDoc::toggleNormalize(SbBool onOff)
//
////////////////////////////////////////////////////////////////////////
{
    noNormalize = onOff;
    /*if (normalize) callback->setCallback(noNormalizeCB, NULL);
    else callback->setCallback(NULL,NULL); */
    callback->touch();  // break caches above callback node

    CMenu* pMenu = AfxGetMainWnd()->GetMenu();
    pMenu->CheckMenuItem( IDM_VIEW_NORMALIZE, 
	    noNormalize ? MF_CHECKED : MF_UNCHECKED);
	
}



////////////////////////////////////////////////////////////////////////
//
// Description:
//	Switches on or off delayed blended transparency on the current
//   viewer. 
//
// Use: private
void
CLargeModelViewerDoc::toggleDelayedTransparency(SbBool onOff)
//
////////////////////////////////////////////////////////////////////////
{
	/*
     if (onOff)
	currentViewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
     else
	currentViewer->setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
	  */
    delayedTransparency = onOff;

}



////////////////////////////////////////////////////////////////////////
//
// Description:
//	Reorganizes scene graph with or without simplififcation
//
// Use: private
void
CLargeModelViewerDoc::reorganizeScene()
//
////////////////////////////////////////////////////////////////////////
{  
    SbBool  doSimplify = FALSE;	// always no simplify in this demo


    ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
		(doSimplify) ? "Reorganizing and simplifying scene..." 
					: "Reorganizing scene..."   );
 
    // Organize scene under new separator:
    SoSeparator *sep = new SoSeparator;
    sep->ref();
    for (int i=0; i<selection->getNumChildren(); i++) {
    	sep->addChild( selection->getChild(i) );
    }
    
     // Replace scene with reorganized scene graph:
    	int numLevels = 3;
    	static float levels[5] = {1.0f, 0.3f, 0.1f};
    	SoReorganizeAction *simplify;
    	if (doSimplify) {
    	  SoDecimator *decimer = new SoDecimator;
    	  simplify = new SoReorganizeAction(decimer);
    	  simplify->setSimplificationLevels(numLevels,levels);
   	  simplify->setSizeFactor(4.);
   	  simplify->setMinTriangles(20);
   	} 
    	else {
    	  simplify = new SoReorganizeAction; ;
    	  simplify->generateNormals(TRUE);
    	  simplify->generateTriangleStrips(TRUE);
    	}
       
	simplify->apply(sep);
	
	sep->unref();
	
	if (simplify->getSimplifiedSceneGraph() != NULL) {
		SoNode *result = simplify->getSimplifiedSceneGraph();
		
		// Replace old data:
		deleteScene();
		selection->addChild(result);
	}
	
   	delete simplify;
}



////////////////////////////////////////////////////////////////////////
//
// Description:
//	Simplifies scene graph using SoShapeSimplifyAction,
//   replacing the shape nodes within the graph without
//   replacing the whole scene.
//
// Use: private
void
CLargeModelViewerDoc::shapeSimplifyScene(SbBool doTriStrips)
//
////////////////////////////////////////////////////////////////////////
{  
   ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
		"Shape simplifying scene..."  );

     // the Simplifier object: uses geometric simplification
   SoDecimator *decimer = new SoDecimator;

   // the simplification action: uses the global simplification.
   SoShapeSimplifyAction simplify(decimer);

   int numLevels = 3;
   static float levels[5] = {1.0f, 0.3f, 0.1f};
    	
   simplify.setSimplificationLevels(numLevels,levels);
   simplify.setMinTriangles(20);

   simplify.apply(selection);

   // Do triangle stripping on results:
   if (doTriStrips) {
	   ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
		"Triangle stripping simplified results..."  );  
	   reorganizeSimplified(selection);
   }
		
   delete decimer;
	
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Simplifies scene graph using SoGlobalSimplifyAction
//
// Use: private
void
CLargeModelViewerDoc::globalSimplifyScene(SbBool doTriStrips, SbBool bySubGroup)
//
////////////////////////////////////////////////////////////////////////
{  
    if (bySubGroup) fprintf(stderr, "Global Simplify by subgroup\n");
    else fprintf(stderr, "Global simplifying scene\n");

     // Replace scene with reorganized scene graph:
    	int numLevels = 3;
    	static float levels[5] = {1.0f, 0.3f, 0.1f};
    	
    	SoDecimator *decimer = new SoDecimator;
    	SoGlobalSimplifyAction *simplify = 
    		new SoGlobalSimplifyAction(decimer);
    	simplify->setSimplificationLevels(numLevels,levels);
   	simplify->setSizeFactor(4.);
   	simplify->setMinTriangles(20);
	simplify->setVerbosity(TRUE);

	if (bySubGroup) 
		simplify->setSimplificationStrategy(
			SoGlobalSimplifyAction::SIMPLIFY_BY_SUBGROUP);
	else simplify->setSimplificationStrategy(
			SoGlobalSimplifyAction::SIMPLIFY_GLOBALLY);

// Organize scene under new separator:
        SoSeparator *sep = new SoSeparator;
        sep->ref();
        for (int i=0; i<selection->getNumChildren(); i++) {
        	sep->addChild( selection->getChild(i) );
        }
    	simplify->apply(sep);
	
	sep->unref();
	
	if (simplify->getSimplifiedSceneGraph() != NULL) {
		SoNode *result = simplify->getSimplifiedSceneGraph();

		// Do triangle stripping on results:
		if (doTriStrips) {
		   ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
				"Triangle stripping simplified results..."  );  
		   reorganizeSimplified(result);
		}

		// Replace old data:
		deleteScene();
		selection->addChild(result);
				
	}
	
	delete simplify;
	delete decimer;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Reorganizes each child of the LevelOfSimplification
//  nodes under "graph".  This will cause each representation
//  to be in triangle strips instead of face sets.
//      It isn't always possible to reorganize a subgraph, because
//  it may lose the coordinates, materials, or shape hints that are
//  higher in the scene graph; but since we using this
//  on the results of the SimplifyActions, we know that the shapes
//  are using vertex property, and therefore contain all of the 
//  coordinate and normal info.
//
// Use: private
void
CLargeModelViewerDoc::reorganizeSimplified(SoNode *graph)
//
////////////////////////////////////////////////////////////////////////
{
	graph->ref();

	SoSearchAction search;
	search.setType(SoLevelOfSimplification::getClassTypeId());
	search.setInterest(SoSearchAction::ALL);
	search.apply(graph);
	
	SoNodeList alreadyDone;

	SoPathList paths = search.getPaths();
	for (int i=0; i<paths.getLength(); i++) {
		SoFullPath *fullpath = (SoFullPath*)paths[i];
		SoLevelOfSimplification *los = 
			(SoLevelOfSimplification*)fullpath->getTail();
		for (int c=0; c<los->getNumChildren(); c++) {
			SoNode *node = los->getChild(c);

			if (alreadyDone.find(node) >=0) {
				// We have already tri-stripped this node,
				// so skip it now:
				continue;
			}
			alreadyDone.append(node);

			SoReorganizeAction tristripper;
			tristripper.generateNormals(TRUE);
			tristripper.generateTriangleStrips(TRUE);
			tristripper.apply(node);
			if (tristripper.getSimplifiedSceneGraph() != NULL) {
				SoNode *result = 
					tristripper.getSimplifiedSceneGraph();
				if (result != node)
					los->replaceChild(node,result);
			}
		}
	}

	graph->unrefNoDelete();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Searches a scene graph for VRMLTimeSensors, and then tries
// to finds the nodes that will be affected by the TimeSensor.
// It then names the nodes so that the Ordering nodes will treat the
// moving parts as special subgraphs, for performance improvements.
//
// Use: private
void
CLargeModelViewerDoc::processAnimations(SoNode *sep)
//
////////////////////////////////////////////////////////////////////////
{    
  SoSearchAction timeSearch;
  timeSearch.setType(SoVRMLTimeSensor::getClassTypeId());
  timeSearch.setInterest(SoSearchAction::ALL);
  timeSearch.apply(sep);
  SoPathList paths = timeSearch.getPaths();
  // Get nodes from paths:
  SoNodeList interpolators, affectedNodes;
  for (int ts1=0; ts1<paths.getLength(); ts1++) {
    SoFullPath *fpath = (SoFullPath*) paths[ts1];
    SoNode *interp = fpath->getTail();
    if (interpolators.find(interp) < 0)
      interpolators.append( interp );
  }

  // Loop through interpolators:
  for (int ts2=0; ts2<interpolators.getLength(); ts2++) {
    SoVRMLInterpolator *interp = (SoVRMLInterpolator *)interpolators[ts2];

    // Get InterpOutput's that the Interpolator contains:
    SoVRMLInterpOutputList interpOutputs;
    interp->getOutputs(interpOutputs);

    // Loop through outputs:
    for (int os=0; os<interpOutputs.getLength(); os++) {
      SoVRMLInterpOutput *interpOut = interpOutputs[os];

      // Get fields that the InterpOutput is connected to:
      SoFieldList fields;
      interpOut->getForwardConnections(fields); 

      // Loop through fields:
      for (int fs=0; fs<fields.getLength(); fs++) {
        SoFieldContainer *fc = fields[fs]->getContainer();
        if (fc->isOfType(SoVRMLInterpolator::getClassTypeId())){
          // Got another Interpolator, add to our interpolators
          // list unless it is already there.
          if (interpolators.find((SoVRMLInterpolator*) fc) < 0)
            interpolators.append( (SoVRMLInterpolator*) fc );
        }
        else if (fc->isOfType(SoNode::getClassTypeId())){
          // Got a node that we want to have under a render object
          if (affectedNodes.find((SoNode*) fc) < 0)
            affectedNodes.append( (SoNode*) fc );
        }
      }
    }
  }

  // Now we have a list of nodes that we want to have under 
  // render objects.  If these nodes are of type Group 
  // (VRMLTransform, most likely) then it is ok,
  // so we add its name (naming it if necessary) to the
  // list for the ordering node, and remove the node from the list.
  // Any nodes that are left, need to be searched for in the
  // scene graph and have their parents added to the name list.
  for (int ri=affectedNodes.getLength()-1; ri >=0; ri--) {
    SoNode *rn = affectedNodes[ri];
    if (rn->isOfType(SoGroup::getClassTypeId())) {
      if (!rn->getName()) {
        // Give it a name
        rn->setName("RenderObject");
        valueOrdering->addRenderObjectName("RenderObject");
        octreeOrdering->addRenderObjectName("RenderObject");
      }
      else {
        SbString temp = rn->getName().getString();
        if ((temp.getLength() > 5) &&
            (temp.getSubString(temp.getLength()-5) != "_RObj")) {
            temp += "_RObj";
            rn->setName(temp);
        }
        valueOrdering->addRenderObjectName( rn->getName() );
        octreeOrdering->addRenderObjectName( rn->getName() );
      }

      affectedNodes.remove(ri);	
    }
  } 

  // Left over nodes, we need to find groups above them:
  if (affectedNodes.getLength() > 0) {
    SoSearchAction nodeSearch;
    nodeSearch.setInterest(SoSearchAction::ALL);
    for (int ni=affectedNodes.getLength()-1; ni >=0; ni--) {
      nodeSearch.setNode(affectedNodes[ni]);
      nodeSearch.apply(sep);
      SoPathList npaths = nodeSearch.getPaths();
      for (int fi=0; fi<npaths.getLength(); fi++) {
        SoFullPath *fpath = (SoFullPath*) npaths[fi];
        for (int fp=0; fp<fpath->getLength(); fp++) {
          SoNode *rn = fpath->getNodeFromTail(fp);
          if (rn->isOfType(SoGroup::getClassTypeId())) {
            if (!rn->getName()) {
              // Give it a name
              rn->setName("RenderObject");
              valueOrdering->addRenderObjectName("RenderObject");
              octreeOrdering->addRenderObjectName("RenderObject");
            }
            else {
              SbString temp = rn->getName().getString();
              if ((temp.getLength() > 5) &&
                  (temp.getSubString(temp.getLength()-5) != "_RObj")) {
                  temp += "_RObj";
                  rn->setName(temp);
              }
              valueOrdering->addRenderObjectName( rn->getName() );
              octreeOrdering->addRenderObjectName( rn->getName() );
            }
            break;
          }
        }
      }
    }
	}
}


/////////////////////////////////////////////////////////////////////////////
// CLargeModelViewerDoc commands

 
BOOL CLargeModelViewerDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    deleteScene();

    // Clear out the Reorganize and Simplify flags:
    if (AfxGetMainWnd() != NULL) {
       CMenu* pMenu = AfxGetMainWnd()->GetMenu();
       pMenu->CheckMenuItem( IDM_LOAD_IVFIX,       MF_UNCHECKED);
       pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL, MF_UNCHECKED);
       pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,  MF_UNCHECKED); 
       pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_UNCHECKED);
       pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_UNCHECKED);
       isSimplify = LMV_SIMP_NONE;
       isReorganize = FALSE;
    }
	
    return TRUE;
}

 
BOOL CLargeModelViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    deleteScene();

    // Clear out the Reorganize and Simplify flags:
    if (AfxGetMainWnd() && AfxGetMainWnd()->GetMenu()) {
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_IVFIX,       MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL, MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,  MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_UNCHECKED);
    }

    isSimplify = LMV_SIMP_NONE;
    isReorganize = FALSE;

    return insertFile( lpszPathName );
}


void CLargeModelViewerDoc::OnEditOctreeOrdering() 
{
    octreeOrderingDlg->Create(octreeOrdering);
    
}

void CLargeModelViewerDoc::OnEditValueOrdering() 
{
    valueOrderingDlg->Create(NULL, valueOrdering);
    
}

void CLargeModelViewerDoc::OnLoadImmediate() 
{
         inlineLoader->setLoadAllImmediate(!inlineLoader->isLoadAllImmediate());
	 CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	 pMenu->CheckMenuItem( IDM_LOAD_IMMEDIATE, 
		 inlineLoader->isLoadAllImmediate() ? MF_CHECKED : MF_UNCHECKED);
	
}			   

void CLargeModelViewerDoc::OnLoadFindVrml() 
{
	findVRMLTimers = !findVRMLTimers;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_FIND_VRML, 
		 findVRMLTimers ? MF_CHECKED : MF_UNCHECKED);
	
}

void CLargeModelViewerDoc::OnViewBackface() 
{
	toggleBackfaceCulling(!backfaceCulling);  
}

void CLargeModelViewerDoc::OnViewSmoothing() 
{
 	toggleSmoothing(!smoothing);  	
}

void CLargeModelViewerDoc::OnViewNormalize() 
{
	toggleNormalize(!noNormalize);
}

void CLargeModelViewerDoc::OnLoadSimpShape() 
{
 	// Do simplification, if was off before and have data now
        if ((isSimplify == LMV_SIMP_NONE)
		&& (selection->getNumChildren() > 0)) {
        	shapeSimplifyScene(FALSE);  // SoShapeSimplifyAction,
					    // no tri strips
        }
	else return; 

    	isSimplify = LMV_SIMP_SHAPE;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,      MF_CHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL,     MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_UNCHECKED);
	
}

void CLargeModelViewerDoc::OnLoadSimpGlobal() 
{
  	// Do simplification, if was off before and have data now
        if ((isSimplify == LMV_SIMP_NONE)
		&& (selection->getNumChildren() > 0)) {
        	globalSimplifyScene(FALSE, FALSE);  // SoGlobalSimplifyAction,
					// no trips strips, not by subgroup
        }
	else return; 

    	isSimplify = LMV_SIMP_GLOBAL;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL,      MF_CHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,      MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_UNCHECKED);
	
}

void CLargeModelViewerDoc::OnLoadSimpShapeTris() 
{
 	// Do simplification, if was off before and have data now
        if ((isSimplify == LMV_SIMP_NONE)
		&& (selection->getNumChildren() > 0)) {
        	shapeSimplifyScene(TRUE);  // SoShapeSimplifyAction,
					    // do tri strips
        }
	else return; 

    	isSimplify = LMV_SIMP_SHAPE;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,      MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL,     MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_CHECKED);
	
}

void CLargeModelViewerDoc::OnLoadSimpGlobalTris() 
{
  	// Do simplification, if was off before and have data now
        if ((isSimplify == LMV_SIMP_NONE)
		&& (selection->getNumChildren() > 0)) {
        	globalSimplifyScene(TRUE, FALSE);  // SoGlobalSimplifyAction,
					// do trips strips, not by subgroup
        }
	else return; 

    	isSimplify = LMV_SIMP_GLOBAL;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL,      MF_UNCHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE,       MF_UNCHECKED);
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_GLOBAL_TRIS, MF_CHECKED); 
	pMenu->CheckMenuItem( IDM_LOAD_SIMP_SHAPE_TRIS,  MF_UNCHECKED);	
}
 
void CLargeModelViewerDoc::OnLoadIvfix() 
{
	// Do immediate reorganization, if was off before and have data now
        if (!isReorganize && (isSimplify == LMV_SIMP_NONE) 
			&& (selection->getNumChildren() > 0)) {
        	reorganizeScene();  // reorganize to make tri strips, 
					// no simplify
        }
	else return;

    	isReorganize = TRUE;
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->CheckMenuItem( IDM_LOAD_IVFIX,       MF_CHECKED);
	
}


void CLargeModelViewerDoc::OnUpdateTravOctreeOrd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);	
}

void CLargeModelViewerDoc::OnUpdateTravValueOrd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);	
}

void CLargeModelViewerDoc::OnUpdateLoadSimpGlobal(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);
}

void CLargeModelViewerDoc::OnUpdateLoadSimpGlobalTris(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);
}

void CLargeModelViewerDoc::OnUpdateLoadSimpShape(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);
}

void CLargeModelViewerDoc::OnUpdateLoadSimpShapeTris(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);	
}

void CLargeModelViewerDoc::OnUpdateEditOctreeOrd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);	
}

void CLargeModelViewerDoc::OnUpdateEditValueOrd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isLMVLicensed);	
}

void CLargeModelViewerDoc::OnLoadSplitShape() 
{
   ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
		"Splitting scene..."  );

	// TODO: Add your command handler code here
	SoSeparator* root = getSceneRoot();
	SoSplitGeometryAction* splitAction;

	//Check environnement variable :
	int numx = 0;
	int numy = 0;
	char* envx = getenv("OIV_SPLIT_NUM_DIVX");
	char* envy = getenv("OIV_SPLIT_NUM_DIVY");

	if(envx) numx = atoi(envx);
	if(envy) numy = atoi(envy);
	
	if (numx*numy) splitAction = new SoSplitGeometryAction(numx,numy);
	else if (numx) splitAction = new SoSplitGeometryAction(numx,4);
	else if (numy) splitAction = new SoSplitGeometryAction(4,numy);
	else splitAction = new SoSplitGeometryAction(4,4);

	splitAction->setSmartSplitting(TRUE);
	splitAction->setDistinguishSplitParts(TRUE);
	splitAction->apply(root);

   ((CMainFrame*)AfxGetMainWnd())->SetPrimitivePane(
		"Ready"  );

}
