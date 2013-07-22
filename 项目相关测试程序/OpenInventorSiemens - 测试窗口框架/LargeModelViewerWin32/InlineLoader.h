/*---------------------------------------------------------------------
 *  Header file for InlineLoader
 *
 *  Copyright (C) 1998 Mercury Computer Systems S.A. All rights reserved
 *
 *  Author: Brian Anderson 
 *-------------------------------------------------------------------*/

// InlineLoader is used to load Inlines that are on the local filesystem,
// it will not fetch files over a network.  It organizes multiple
// requests so that the same subgraph is used if the filename is the same.

#ifndef  _INLINE_LOADER_
#define  _INLINE_LOADER_

class SoWWWInline;
class SoVRMLInline;

class InlineLoader {
   public:

typedef void ReadingInlineCB(const char *filename, 
			   void *userData, InlineLoader *);
	   
	InlineLoader();
   	~InlineLoader();
   
	void  FetchAllEmpty();
	void  FetchNextFile();
	
	void  ClearLists();
	void  FixWWWInlines(SoNode *graph);

 	void setLoadAllImmediate(SbBool onOff) {
		loadAllImmediate = onOff; };
	SbBool isLoadAllImmediate() { return loadAllImmediate; };

	void setReadingInlineCallback(ReadingInlineCB *callback,
                                void *userData = NULL) {
	      readingInlineCallback = callback;
	      readingInlineData = userData; };

   private:
	SoGroup *readFile(const char *filename, SbBool isVRML = FALSE);
	void removeTail(SoPath *p);
	void removeNodesOfType(SoNode *graph, SoType type);
	
	SbBool loadAllImmediate;

	int numEmptyFetch; 
	SbPList fetchQueue;
	SoOneShotSensor *sensor;
	
	void AddToQueue(void *data, SbVec3f size);
	
	static void FetchFileCB(void *userData, SoSensor *sensor);
	static void WWWInlineFetchURLCB(const SbString &url, void *userData, 
        		SoWWWInline *node)
        	{ ((InlineLoader*)userData)->WWWInlineFetchURL(url, node); }
	void WWWInlineFetchURL(const SbString &url, SoWWWInline *node);
	
	static void VRMLInlineFetchURLCB(const SbString &url, void *userData, 
        		SoVRMLInline *node)
        	{ ((InlineLoader*)userData)->VRMLInlineFetchURL(url, node); }
	void VRMLInlineFetchURL(const SbString &url, SoVRMLInline *node);

	
    // ReadingInline callback
    ReadingInlineCB  *readingInlineCallback;
    void         *readingInlineData;

};

#endif

