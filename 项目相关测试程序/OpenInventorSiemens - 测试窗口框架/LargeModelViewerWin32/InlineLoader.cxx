
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/VRMLnodes/SoVRMLInline.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include "InlineLoader.h"

#ifdef _WIN32
#include <Inventor/Win/SoWinRenderArea.h>
#else
#include <Inventor/Xt/SoXtRenderArea.h>
#endif

// --------------------------------------------------------------------------


typedef struct sWWWFetchData {
	SbString url;
	SoNode *wwwinline;
	float size;
	SbBool isWWWInline;  // if FALSE, is VRMLInline
} WWWFetchData; 



InlineLoader::InlineLoader()
{
   numEmptyFetch =0; 
   loadAllImmediate = TRUE;
   readingInlineCallback = NULL;
   readingInlineData = NULL;

   sensor = new SoOneShotSensor(FetchFileCB, this);
#ifdef _WIN32
   sensor->setPriority(SoWinRenderArea::getDefaultRedrawPriority()+10);
#else
   sensor->setPriority(SoXtRenderArea::getDefaultRedrawPriority()+10);
#endif
   SoWWWInline::setFetchURLCallBack(WWWInlineFetchURLCB, this);
   SoVRMLInline::setFetchURLCallBack(VRMLInlineFetchURLCB, this);
   	
}

InlineLoader::~InlineLoader()
{
   delete sensor;
   ClearLists();
}

void
InlineLoader::ClearLists()
{
	numEmptyFetch = 0;

	// delete structures in fetchQueue:
	int num = fetchQueue.getLength();
	for (int i=num-1; i>=0; i--) {
		delete (WWWFetchData*) fetchQueue[i];	
	}
	fetchQueue.truncate(0);
}

SoGroup *
InlineLoader::readFile(const char *filename, SbBool isVRML)
{
   // Open the input file
   SoInput mySceneInput;
   if (!mySceneInput.openFile(filename)) {
      fprintf(stderr, "Cannot open file %s\n", filename);
      return NULL;
   }

   // Read the whole file into the database, 
   // using IV or VRML read function
   SoGroup *myGraph = NULL;
   if (isVRML) myGraph = SoDB::readAllVRML(&mySceneInput);
   else myGraph = SoDB::readAll(&mySceneInput);

   if (myGraph == NULL) {
      fprintf(stderr, "Problem reading file\n");
      return NULL;
   } 

   const char *fullpath = mySceneInput.getCurFileName();
       
   // Find directory from full path in order to know where to 
   // look for WWWInlines later.
   size_t pos, len = strlen(fullpath);

   for (pos=len-1; pos>=0; pos--) 
	if ((fullpath[pos] == '\\') || (fullpath[pos] == '/')) break;


   if (pos > 0) {
        // break off path from filename, and
        // give to SoInput as place to look first:
        char *dir = new char [len+1];
        strncpy(dir, fullpath, pos);
        dir [pos] = '\0';

	// If already in directory list, remove and then add
	// so we don't get too many of the same directory:
	SoInput::removeDirectory(dir);

        SoInput::addDirectoryLast(dir);
            
        delete [] dir;          
   }

   mySceneInput.closeFile();

   return myGraph;
}


// ---------------------------------------------------------------
// File scene graph processing:
// Removing cameras, adding new SpatialOrdering, etc.

void
InlineLoader::removeTail(SoPath *p)
{ 
        int index = p->getIndexFromTail(0);
        SoNode *parent = p->getNodeFromTail(1);
        if (!parent) return;
        
        // Not necessarily SoGroup ...
        if (parent->isOfType(SoGroup::getClassTypeId())) {
        	((SoGroup *)parent)->removeChild(index);
        
           // Adjust switch that we may be changing:
           if (parent->isOfType(SoSwitch::getClassTypeId())) {
                SoSwitch *sw = (SoSwitch*) parent;
                if (sw->whichChild.getValue() > index)
                    sw->whichChild = sw->whichChild.getValue()-1;
                else if (sw->whichChild.getValue() == index)
                    sw->whichChild = -1;
           }
        }

}

void
InlineLoader::removeNodesOfType(SoNode *graph, SoType type)
{
    SoSearchAction typeSearch;
    typeSearch.setInterest(SoSearchAction::ALL);
    typeSearch.setType( type );
    typeSearch.apply(graph);
    
    SoPathList paths = typeSearch.getPaths();
    for (int i=paths.getLength()-1; i>=0; i--) {
    	removeTail(paths[i]);
    }    
 

}

void
InlineLoader::FixWWWInlines(SoNode *graph)
{
    SoSearchAction typeSearch;
    typeSearch.setInterest(SoSearchAction::ALL);
    typeSearch.setType( SoWWWInline::getClassTypeId() );
    typeSearch.apply(graph);    
    
    SoPathList paths = typeSearch.getPaths();
    for (int i=paths.getLength()-1; i>=0; i--) {
    	SoPath *path = paths[i];
    	SoWWWInline *node = (SoWWWInline*) path->getTail();
    	
    	if (loadAllImmediate) {
    		node->requestURLData();
    		continue;
    	}
    	if (!node->bboxSize.isDefault()) continue;
    	
    	SbBool fixed = FALSE;
    	for (int j=1; j<path->getLength(); j++) {
    		SoNode *trav = (SoNode*) path->getNodeFromTail(j);
    		if (trav->isOfType(SoLOD::getClassTypeId())) {
    			// Found an SoLOD above this WWWInline	
    			// Try and find bounding box of SoLOD's
    			// simplest child:
    			SoLOD *lod = (SoLOD*) trav;
    			int numChildren = lod->getNumChildren();
    			if (path->getIndexFromTail(j) < numChildren-1) {
    				// There is a simpler version of the
    				// WWInline:
    				SoNode *subgraph = 
    					lod->getChild(numChildren-1);
    				SoGetBoundingBoxAction bba =
	                            SoGetBoundingBoxAction(SbViewportRegion());
	                        bba.apply(subgraph);
	                        SbBox3f inlineBbox = bba.getBoundingBox();
	                        SbVec3f bbsize;
	    			inlineBbox.getSize(bbsize[0], 
	    				bbsize[1], bbsize[2]);
	    			SbVec3f bbcenter = inlineBbox.getCenter();
	    			
	    			node->bboxCenter.setValue(bbcenter);
	                        node->bboxSize.setValue(bbsize);
	                        
	                        fixed = TRUE;
	                        break;
    			}
    		}
    		else if (trav->isOfType(SoWWWInline::getClassTypeId())) {
    			// found nested WWWInlines, break out
    			break;
    		}
    	} 
    	
    	if (!fixed) {
    		node->requestURLData();
    	}
    	
    }    

}

// ---------------------------------------------------------------


// Fetchs next file from queue:
void
InlineLoader::FetchNextFile()
{
	// Take first file off of queue, and read it in:
	WWWFetchData *data;
	data = (WWWFetchData *) fetchQueue[0];
	fetchQueue.remove(0);
	// decrement number of empty shapes 
	if (numEmptyFetch > 0) numEmptyFetch--;
	
	SbString url = data->url;
	SoNode *node = data->wwwinline;
	
	// Call user callback to notify that reading is happening now:
	if (readingInlineCallback) {
		(*readingInlineCallback)(url.getString(),
			readingInlineData, this);
	}

	// Read file
	SoGroup *sep = readFile(url.getString(), !data->isWWWInline);
        if (sep) {
                sep->ref(); 
                removeNodesOfType(sep, SoCamera::getClassTypeId());
               
                // Find WWWInlines with no bboxSize set:
                FixWWWInlines(sep);

 		if (data->isWWWInline)
                	((SoWWWInline*)node)->setChildData(sep);
                else 
                	((SoVRMLInline*)node)->setChildData(sep);
                sep->unref();
		
		
		// Now search for other requests for the same filename:
		int num = fetchQueue.getLength();
		for (int i=num-1; i>=0; i--) {
		   WWWFetchData *fdata = (WWWFetchData *) fetchQueue[i];
		   if (fdata->url == url) {
			// Found similar request.
			if (data->isWWWInline)
                	   ((SoWWWInline*)fdata->wwwinline)->setChildData(sep);
                	else 
                	   ((SoVRMLInline*)fdata->wwwinline)->setChildData(sep);
			fetchQueue.remove(i);
			if (numEmptyFetch > i) numEmptyFetch--;
			delete fdata;
		   }
		}
        }
        else {
                fprintf(stderr, "Could not read file: %s\n", url.getString() );
        }
	
	delete data;


}

void 
InlineLoader::FetchAllEmpty()
{
	while (numEmptyFetch > 0) {
		FetchNextFile();
	}

}

// Callback from Idle Sensor
void 
InlineLoader::FetchFileCB(void *userData, SoSensor *sensor)
{
	InlineLoader *thisp = (InlineLoader*) userData;

	if (thisp->fetchQueue.getLength() == 0) return;
	
	SbTime beginTime = SbTime::getTimeOfDay();
	
   getanother:
	
	SbBool isStill = TRUE;  // assume TRUE until we check for events
	if (isStill)
		thisp->FetchNextFile();
	
	SbTime endTime = SbTime::getTimeOfDay();
	

	if (thisp->fetchQueue.getLength() > 0) {
		// If we haven't taken too much time, get another one now:
		if (isStill && ((endTime-beginTime).getValue() < 1.0)) {
			// less than one second, so go get another:
			goto getanother;
		}
	
		// Reschedule Sensor:
		sensor->schedule();
	}
}


void 
InlineLoader::WWWInlineFetchURL(const SbString &url,
        				SoWWWInline *node)
{
//        fprintf(stderr, "Inline Fetch: %s\n",url.getString() );

/* This seems to mess up the curPath in a SoCallbackAction ?
	for (int f=0; f<filenames.getLength(); f++) {
		if (*(filenames[f]) == url) {
			// We have already loaded file.
			cerr << "   Already loaded #" << f << " " 
				<< filenames[f]->getString() << " "
				<< (unsigned long) filenodes[f] << endl;
			
			node->setChildData(filenodes[f] );
			return;
		}
	}
*/


        // Add to fetch queue:
        WWWFetchData *data = new WWWFetchData;
        data->url = url;
        data->wwwinline = node;
	data->isWWWInline = TRUE;      // SoWWWInline  node
        SbVec3f bbsize = node->bboxSize.getValue();
        
        AddToQueue(data, bbsize);
}

void 
InlineLoader::VRMLInlineFetchURL(const SbString &url,
        				SoVRMLInline *node)
{
//        fprintf(stderr, "Inline Fetch: %s\n",url.getString() );

        // Add to fetch queue:
        WWWFetchData *data = new WWWFetchData;
        data->url = url;
        data->wwwinline = node;	
	data->isWWWInline = FALSE;   // SoVRMLInline  node
        SbVec3f bbsize = node->bboxSize.getValue();
        
        AddToQueue(data, bbsize);
}

       
void
InlineLoader::AddToQueue(void* vdata, SbVec3f bbsize) 
{        
	WWWFetchData *data = (WWWFetchData*) vdata;
	
        // sum dimensions of box:
        data->size = bbsize[0] + bbsize[1] + bbsize[2];
        if (data->size < 0.0) data->size = 0.0;
        
        // Insert into queue according to size of bounding box:
        if (data->size == 0.0) {
        	// We don't know size of boundingbox, so we need to go
        	// get the file first:
        	fetchQueue.insert((void*) data, numEmptyFetch++);
        }
        else {
        	int num = fetchQueue.getLength();
        	//if (num == 0) {
        	if (num == numEmptyFetch) {
        		// empty queue, so just throw it at the end
        		fetchQueue.append((void*) data);
        	}
        	else {
        	   // See if the node goes at the end (if smallest shape):
        	   WWWFetchData *fdata = (WWWFetchData *) fetchQueue[num-1];
       		   if (fdata->size >= data->size) {
        	   	fetchQueue.append((void*)data);
         	   }
        	   else {
        	   	// Insert into queue
         	  	for (int i=0; i<num; i++) {
				fdata = (WWWFetchData *) fetchQueue[i];
				if ((fdata->size > 0.0) &&
					(fdata->size < data->size)) {
					fetchQueue.insert((void*)data, i);
					break;
				}
	  	 	}
	  	   } 
		}
	}
	
	// Schedule Idle Sensor to go get the file:
	sensor->schedule();
}



