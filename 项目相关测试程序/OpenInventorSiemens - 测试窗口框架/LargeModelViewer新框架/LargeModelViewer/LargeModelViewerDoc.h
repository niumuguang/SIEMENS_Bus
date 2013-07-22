
// LargeModelViewerDoc.h : CLargeModelViewerDoc ��Ľӿ�
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
protected: // �������л�����
	CLargeModelViewerDoc();
	DECLARE_DYNCREATE(CLargeModelViewerDoc)

// ����
public:

// ����
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
	virtual ~CLargeModelViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
