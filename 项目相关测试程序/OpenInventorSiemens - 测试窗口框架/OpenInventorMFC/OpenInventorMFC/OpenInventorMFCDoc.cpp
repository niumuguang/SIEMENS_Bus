
// OpenInventorMFCDoc.cpp : COpenInventorMFCDoc 类的实现
//

#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenInventorMFC.h"
#endif

#include "OpenInventorMFCDoc.h"

#include <propkey.h>
//niu
#include "MainFrm.h"
//#include "OctreeOrderingDialog.h"
//#include "ValueOrderingDialog.h"

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


#include <Inventor/actions/SoSplitGeometryAction.h>
#include "InlineLoader.h"

#include <string.h>
// niu end

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenInventorMFCDoc

IMPLEMENT_DYNCREATE(COpenInventorMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenInventorMFCDoc, CDocument)
END_MESSAGE_MAP()


// COpenInventorMFCDoc 构造/析构

COpenInventorMFCDoc::COpenInventorMFCDoc()
{
	// TODO: 在此添加一次性构造代码
	// niu
	sceneGraph = new SoSeparator();
	sceneGraph->ref();

	createLightsCameraEnvironment();
	sceneGraph->addChild(lightsCameraEnvironment);

	sepOrdering = new SoSeparator;
	sepOrdering->ref();

	orderingNode = sepOrdering;    	// use Separator as default
	sceneGraph->addChild(orderingNode);

	selection = new SoSelection;
	orderingNode->addChild(selection);

	sceneKitCamNumDict = new SbDict(5);
	switchOffSceneKitCameras(selection);
	removeCameras(selection);
	// niu end
}

COpenInventorMFCDoc::~COpenInventorMFCDoc()
{
	sceneGraph->unref();
}

BOOL COpenInventorMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// COpenInventorMFCDoc 序列化

void COpenInventorMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void COpenInventorMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void COpenInventorMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void COpenInventorMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenInventorMFCDoc 诊断

#ifdef _DEBUG
void COpenInventorMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenInventorMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

void COpenInventorMFCDoc::createLightsCameraEnvironment()
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

void COpenInventorMFCDoc::switchOffSceneKitCameras( SoGroup *root )
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

void COpenInventorMFCDoc::removeCameras( SoGroup *root )
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

#endif //_DEBUG


// COpenInventorMFCDoc 命令
