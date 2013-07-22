
// OpenInventorMFCDoc.h : COpenInventorMFCDoc 类的接口
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
protected: // 仅从序列化创建
	COpenInventorMFCDoc();
	DECLARE_DYNCREATE(COpenInventorMFCDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~COpenInventorMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
