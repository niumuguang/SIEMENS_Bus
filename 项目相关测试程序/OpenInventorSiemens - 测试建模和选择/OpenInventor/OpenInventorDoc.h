
// OpenInventorDoc.h : COpenInventorDoc ��Ľӿ�
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
protected: // �������л�����
	COpenInventorDoc();
	DECLARE_DYNCREATE(COpenInventorDoc)

	// ���빫����Ա
public:
	SoSeparator *root;

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
	virtual ~COpenInventorDoc();
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
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

//////////////�ı�������////////////////////////////////////////////////////////////
	// ���ڵ�
	//SoSeparator *m_MainRoot;
};
