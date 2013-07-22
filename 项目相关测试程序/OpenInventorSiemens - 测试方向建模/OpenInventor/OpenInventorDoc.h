
// OpenInventorDoc.h : COpenInventorDoc 类的接口
//


#pragma once
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoText2.h>
#include <SolidViz/SoCSGOperation.h>
#include <Inventor/nodes/SoSelection.h>

class SoSeparator;

class COpenInventorDoc : public CDocument
{
protected: // 仅从序列化创建
	COpenInventorDoc();
	DECLARE_DYNCREATE(COpenInventorDoc)

	// 加入公共成员
public:
	SoSeparator *root;

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
	virtual ~COpenInventorDoc();
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
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

//////////////改变软件框架////////////////////////////////////////////////////////////
	// 主节点
	//SoSeparator *m_MainRoot;
};
