// LargeModelViewerDoc.h : interface of the CLargeModelViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include <Inventor/SbBasic.h>

class SbDict;
class SoEnvironment;
class SoGroup;
class SoNode;
class SoAction;
class SoCamera;
class SoCallback;
class SoSeparator;
class SoSelection;
class SoShapeHints;
class SoLabel;
class SoOctreeOrdering;
class SoValueOrdering;
class SoPath;
class SoPickedPoint;
class InlineLoader;

//class COctreeOrderingDialog;
//class CValueOrderingDialog;

class CLargeModelViewerDoc : public CDocument
{
protected: // create from serialization only
	CLargeModelViewerDoc();
	DECLARE_DYNCREATE(CLargeModelViewerDoc)

// Attributes
public:

// Operations
public:

enum Ordering {
    LMV_ORD_SEPARATOR = 0, 
    LMV_ORD_VALUE_ORDERING, 
    LMV_ORD_OCTREE_ORDERING
};


enum LmvESimplify {
    LMV_SIMP_NONE = 0, 
    LMV_SIMP_GLOBAL, 
    LMV_SIMP_SUBGROUP,
    LMV_SIMP_SHAPE
};



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLargeModelViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLargeModelViewerDoc();
#if defined(_DEBUG)
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// returns root of Document's scene graph
	SoSeparator *getSceneRoot() { return sceneGraph; };

	// Reads file and inserts graph into scene
	SbBool         insertFile(const char *filename);
	
	void 		switchOrderingNode(Ordering whichType);
    
private:
    SoSeparator *sceneGraph;    
    SoSelection	    	*selection;	    	
    
    // Lights, camera, environment!
    SoGroup 	    	*lightsCameraEnvironment;
    SoLabel 	    	*envLabel;
    SoEnvironment   	*environment;
    SoGroup 	    	*lightGroup;

    //InlineLoader *inlineLoader;

    void	deleteScene();
    SoSeparator *readOIVFile(const char *filename);
    void	createLightsCameraEnvironment();
 	
	// cameras
    void	removeCameras(SoGroup *root);
    // Returns the last scene kit camera in a scene.
    SoCamera    *getLastSceneKitCamera(SoGroup *root);
    // Copies our camera into the last scene kit camera in a scene.
    void        setLastSceneKitCamera(SoGroup *root, SoCamera *theCam);
    // Switches off/ restores camera num settings of scenekits.
    void        switchOffSceneKitCameras(SoGroup *root);
    void        restoreSceneKitCameras(SoGroup *root);
    // Copies from given camera into this viewer's camera, if types match.
    //void        setCameraFields( SoCamera *fromCam );	// move to View Class
    SbDict      *sceneKitCamNumDict;
   
    //static SoPath *pickFilterCB(void *userData, const SoPickedPoint *pick);
    //static void readingInlineCB(const char *filename, void *userData, InlineLoader *);

 // Separator or RenderList node:
    SoSeparator  *orderingNode;  // which is current
    SoSeparator    *sepOrdering;
    //SoValueOrdering *valueOrdering;
    //SoOctreeOrdering *octreeOrdering;
    
    //COctreeOrderingDialog *octreeOrderingDlg;
    //CValueOrderingDialog  *valueOrderingDlg;
    
    //void 		toggleBackfaceCulling(SbBool onOff);
   // SbBool 		backfaceCulling;
   // void 		toggleDelayedTransparency(SbBool onOff);
   // SbBool 		delayedTransparency;	    
    //void 		toggleSmoothing(SbBool onOff);
    //SbBool 		smoothing;	     	    
    //void 		toggleNormalize(SbBool onOff);
    //SbBool 		noNormalize; 
    //SoCallback		*callback;
    //static void         noNormalizeCB(void *, SoAction*);

    //SoShapeHints     *shapeHints;
    //SbBool 	      findVRMLTimers;
    //void	      processAnimations(SoNode*);
    //SbBool	 isReorganize;
    //LmvESimplify isSimplify;
    //void	reorganizeScene();
    //void	shapeSimplifyScene(SbBool doTriStrips);
    //void	globalSimplifyScene(SbBool doTriStrips, SbBool bySubGroup);
    //void	reorganizeSimplified(SoNode *graph);
    //SbBool	isLMVLicensed;

// Generated message map functions
protected:
	//{{AFX_MSG(CLargeModelViewerDoc)
	//afx_msg void OnEditOctreeOrdering();
	//afx_msg void OnEditValueOrdering();
	//afx_msg void OnLoadImmediate();
	//afx_msg void OnLoadFindVrml();
	//afx_msg void OnViewBackface();
	//afx_msg void OnViewSmoothing();
	//afx_msg void OnViewNormalize();
	//afx_msg void OnLoadSimpShape();
	//afx_msg void OnLoadSimpGlobal();
	//afx_msg void OnLoadIvfix();
	//afx_msg void OnLoadSimpShapeTris();
	//afx_msg void OnLoadSimpGlobalTris();
	//afx_msg void OnUpdateTravOctreeOrd(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateTravValueOrd(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateLoadSimpGlobal(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateLoadSimpGlobalTris(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateLoadSimpShape(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateLoadSimpShapeTris(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateEditOctreeOrd(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateEditValueOrd(CCmdUI* pCmdUI);
	//afx_msg void OnLoadSplitShape();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
