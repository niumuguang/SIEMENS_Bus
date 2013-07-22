
// LargeModelViewerDoc.h : CLargeModelViewerDoc 类的接口
//
//
// BEGIN OIV
//
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

class COctreeOrderingDialog;
class CValueOrderingDialog;

//
// END OIV
//

#pragma once


class CLargeModelViewerDoc : public CDocument
{
protected: // 仅从序列化创建
	CLargeModelViewerDoc();
	DECLARE_DYNCREATE(CLargeModelViewerDoc)

// 特性
public:

// 操作
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
	virtual ~CLargeModelViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
