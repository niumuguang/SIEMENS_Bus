
// OpenInventorMFCDoc.h : COpenInventorMFCDoc ��Ľӿ�
//
// niu 
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
// niu end

#pragma once


class COpenInventorMFCDoc : public CDocument
{
protected: // �������л�����
	COpenInventorMFCDoc();
	DECLARE_DYNCREATE(COpenInventorMFCDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~COpenInventorMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

// niu
private:
	SoSeparator *sceneGraph;
	// Lights, camera, environment!
	SoGroup 	    	*lightsCameraEnvironment;
	SoLabel 	    	*envLabel;
	SoEnvironment   	*environment;
	SoGroup 	    	*lightGroup;
	SoSeparator    *sepOrdering;
	SoSeparator  *orderingNode;  // which is current
	SoSelection	    	*selection;	   

	SbDict      *sceneKitCamNumDict;

	void	createLightsCameraEnvironment();
	// cameras
	void	removeCameras(SoGroup *root);
	void        switchOffSceneKitCameras(SoGroup *root);

// niu end

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
