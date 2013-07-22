
// OpenInventorView.cpp : COpenInventorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenInventor.h"
#include <Inventor/Win/SoWin.h>
#endif

#include "OpenInventor.h"
#include "OpenInventorDoc.h"
#include "OpenInventorView.h"
#include <SolidViz/SoCSGOperation.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include "MenuTest.h"
#include "Resource.h"
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoInfo.h>
#include <string>
#include "TestDlg.h"
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include "JudgementDLG.h"
#include "LStyleBar.h"

using namespace std;

// 全局变量 这个可以移到view类中
static float reddish[] = {1.0f, 0.2f, 0.2f};  // Color when selected
static float white[] = {0.8f, 0.8f, 0.8f};    // Color when not selected
SoMaterial *textMaterial, *sphereMaterial;


#if defined(_DEBUG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// COpenInventorView

IMPLEMENT_DYNCREATE(COpenInventorView, CView)

BEGIN_MESSAGE_MAP(COpenInventorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenInventorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_CreatModel, &COpenInventorView::OnCreatmodel)
ON_COMMAND(ID_FILE_1, &COpenInventorView::OnFile1)
ON_WM_DESTROY()
ON_COMMAND(ID_VIEW_STATUS_BAR, &COpenInventorView::OnViewStatusBar)
ON_COMMAND(ID_APP_ABOUT, &COpenInventorView::rotView)
END_MESSAGE_MAP()

// COpenInventorView 构造/析构

COpenInventorView::COpenInventorView()
{
	SbVec3f axt;
	axt.setValue(1,0,0);
	totalRotation.setValue(axt, 0);
	//m_RadiusView = 2000000;
	// TODO: 在此处添加构造代码
	ss = 0;
	ViewType = 1;
	//textMaterial = new SoMaterial;
	//textMaterial->diffuseColor.setValue(0.8f, 0.8f, 0.8f);

}

COpenInventorView theView;

COpenInventorView::~COpenInventorView()
{

}

BOOL COpenInventorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COpenInventorView 绘制

void COpenInventorView::OnDraw(CDC* /*pDC*/)
{
	COpenInventorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: 在此处为本机数据添加绘制代码
}


// COpenInventorView 打印


void COpenInventorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenInventorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenInventorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COpenInventorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void COpenInventorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenInventorView 诊断

#ifdef _DEBUG
void COpenInventorView::AssertValid() const
{
	CView::AssertValid();
}

void COpenInventorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenInventorDoc* COpenInventorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenInventorDoc)));
	return (COpenInventorDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenInventorView 消息处理程序

void mouseMoveCB(void *userdata, SoEventCallback *eventCB)
{
	//// 鼠标移动事件
	const SoMouseButtonEvent *event = (SoMouseButtonEvent *)eventCB->getEvent();
	//SoSeparator	* tempRoot = theApp.m_MainRoot;
	//
	////SbVec3f test = &theView.m_mainCam->position.getValue();

	// 计算当前鼠标半球位置坐标
	short testX, testY;
	SbVec2s pos = event->getPosition();
	pos.getValue(testX, testY);
	CPoint ss;
	ss.SetPoint(testX, testY);

	//CMenu menu;
	//menu.LoadMenu(IDR_MENU1);
	//CMenu *pPopup=menu.GetSubMenu(0);//获取菜单对象指针
	//ClientToScreen(theApp.m_ViewHwnd,&ss);//显示坐标转化


	CRect rc;
	//MainCWnd->GetWindowRect(&rc);
	//getCWND().GetWindowRect(&rc);
	theApp.m_ViewCWnd->GetWindowRect(&rc);
	CPoint MidPt = rc.CenterPoint();
	if (ss.y <= MidPt.y)
	{
		ss.y = MidPt.y + abs(ss.y - MidPt.y) + 27;
	}
	else
	{
		ss.y = MidPt.y - abs(ss.y - MidPt.y) + 27;
	}
	// 设置现在鼠标位置（这个位置也没关联屏幕）
	theApp.m_MouseCurPos = ss;

	//theView.hemishere(ss.x, ss.y, rc.Height(), theView.curPos);
	float d, dx, dy, dz;
	theView.hemishere(testX, testY, rc.Height(), theView.curPos);
	dx = get<0>(theView.curPos) - get<0>(theView.lastPos);
	dy = get<1>(theView.curPos) - get<1>(theView.lastPos);
	dz = get<2>(theView.curPos) - get<2>(theView.lastPos);
	
	if (theApp.m_rotationSwitch== true)
	{
		if (dx || dy || dz)
		{	
			// 计算移动距离，用来近似移动的球面距离
			d = sqrt(dx * dx + dy * dy + dz * dz);
			// 通过移动距离计算移动的角度
			theApp.theta = d;
			// 计算移动平面的法向量，即：lastPos × curPos
			get<0>(theView.m_axis)= get<1>(theView.lastPos) * get<2>(theView.curPos) - get<2>(theView.lastPos) * get<1>(theView.curPos);
			//theView.axis[0] = theView.lastPos[1] * theView.curPos[2] - theView.lastPos[2] * theView.curPos[1];
			get<1>(theView.m_axis)= get<2>(theView.lastPos) * get<0>(theView.curPos) - get<0>(theView.lastPos) * get<2>(theView.curPos);
			//theView.axis[1] = theView.lastPos[2] * theView.curPos[0] - theView.lastPos[0] * theView.curPos[2];
			get<2>(theView.m_axis)= get<0>(theView.lastPos) * get<1>(theView.curPos) - get<1>(theView.lastPos) * get<0>(theView.curPos);
			//theView.axis[2] = theView.lastPos[0] * theView.curPos[1] - theView.lastPos[1] * theView.curPos[0];
			// 记录当前的鼠标单位半球面坐标
			get<0>(theView.lastPos) = get<0>(theView.curPos);
			get<1>(theView.lastPos) = get<1>(theView.curPos);
			get<2>(theView.lastPos) = get<2>(theView.curPos);

 		//	SbVec3f x;
			//x.setValue(theView.axis[0], theView.axis[1], theView.axis[2]);
			//SbRotation rotationTemp;

			SbVec3f ax;
			ax.setValue(get<0>(theView.m_axis), get<1>(theView.m_axis),get<2>(theView.m_axis));
			//SbRotationd ff;
			//rotationTemp.setValue(ax, PI/2);
			//ff = ff * ff;

			SbVec3f getX; float getA;
			SbRotation xx;
			xx.setValue(ax, theApp.theta);
			//theView.totalRotation = theView.totalRotation * xx;
			
			//
			theApp.m_pViewer->rotateCamera(xx);
			//theApp.m_pViewer->rotateCamera(theView.totalRotation);
// 			SoOrthographicCamera * newCam =new SoOrthographicCamera;
// 			newCam->orientation.setValue(ax, theView.theta);
// 			newCam->setName("ProCam");
		}
	}
	//else
	//{
	//}
	//if (theApp.m_translationSwitch == true)
	//{
	//}
	////MessageBox(m_HWND2,_T("mykeypress"),_T("dd"),0);
}

void COpenInventorView::mouseWheelCB(void *, SoEventCallback *eventCB)
{
	printf("ddd");
}

//void COpenInventorView::
void mousePressCB(void *, SoEventCallback *eventCB)
{
	// 鼠标点击事件
	//CString newValue = theApp.pPropXcoord->GetValue().bstrVal;
	theApp.pPropXcoord->SetValue((_variant_t) 500l);
	//MessageBox(_T("ChoosePixelFormat Failed"));
	const SoMouseButtonEvent *event = (SoMouseButtonEvent *)eventCB->getEvent();
	SbVec2s pos = event->getPosition();
	short testX, testY;
	pos.getValue(testX, testY);
	CPoint ss, tss;
	ss.SetPoint(testX, testY);

	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu *pPopup=menu.GetSubMenu(0);//获取菜单对象指针
	ScreenToClient(theApp.m_ViewHwnd,&tss);
	ClientToScreen(theApp.m_ViewHwnd,&ss);//显示坐标转化
	//ClientToScreen(&ss);

	CRect rc;
	//MainCWnd->GetWindowRect(&rc);
	theApp.m_ViewCWnd->GetWindowRect(&rc);
	CPoint MidPt = rc.CenterPoint();

	if (ss.y <= MidPt.y)
	{
		ss.y = MidPt.y + abs(ss.y - MidPt.y) + 27;
	}
	else
	{
		ss.y = MidPt.y - abs(ss.y - MidPt.y) + 27;
	}

	if (event->getButton() == SoMouseButtonEvent::BUTTON2)
	{
		if(TRUE == event->isButtonPressEvent(event, SoMouseButtonEvent::BUTTON2) )
		{
			theApp.m_translationSwitch = true;
			theApp.m_MousePressPos = ss;
		}
		else
		{
			theApp.m_translationSwitch = false;
		}
	}
	if (event->getButton() == SoMouseButtonEvent::BUTTON3)
	{
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, ss.x, ss.y, theApp.m_ViewCWnd);
	}

	if (event->getButton() == SoMouseButtonEvent::BUTTON1)
	{
		//MessageBox(theApp.m_ViewHwnd,_T("mykeypress"),_T("dd"),0);
		if(TRUE == event->isButtonPressEvent(event, SoMouseButtonEvent::BUTTON1) )
		{
			theApp.m_rotationSwitch = true;
			//theApp.m_MousePressPos = ss;
			theView.hemishere(testX, testY, rc.Height(), theView.lastPos);
		}
		else if(TRUE == event->isButtonReleaseEvent(event, SoMouseButtonEvent::BUTTON1))
		{
			theApp.m_rotationSwitch = false;
			//theApp.m_MouseReleasePos = ss;
			//SoNode * testRotation = theView.m_MainNode->getByName("ProRotation");
			//SoNode * testRo = theView.m_MainNode->getByName("ProRo");
			//SoNode * tempCam = theView.m_MainNode->getByName("ProMainCam");

			//SbVec3f x;
			//x.setValue(get<0>(theView.m_axis), get<1>(theView.m_axis), get<2>(theView.m_axis));
			//theView.m_rotation->rotation.setValue(x, theView.theta);
			//////////////////////////////////////////////////////////////////////////
			//SoPerspectiveCamera * newCam = new SoPerspectiveCamera;
			//theView.m_MainNode->replaceChild(testRotation, ccp );


			//SbVec3f rotationAxis;SbVec2s tt;
			//double rotationAngle;
			//double tempDis = theView.get2PointDis(theView.getMousePressPos(), theView.getMouseReleasePos());
			//rotationAngle = theView.getAngle(tempDis);
			//double slopeK = theView.getSlope(theView.getMousePressPos(), theView.getMouseReleasePos());
			//double ArcSlopeK = theView.getArcSlope(slopeK);
			//SbVec2f pt4 = theView.getPt4(tempDis, ArcSlopeK);
			//SbVec2f midPt = theView.getMidPt(theView.getMousePressPos(), theView.getMouseReleasePos());
			//rotationAxis = theView.getNormalVec(pt4, midPt);

			//SoNode * testRotation = theView.getRoot()->getByName("ProRotation");
			//SoRotation * ccp = new SoRotation;
			//ccp->setName("ProRotation");
			
			/*SoRotation * ccp = new SoRotation;
		ccp->setName("ProRotation");
		SbVec3f x;x.setValue(10, 0, 0);
		ccp->rotation.setValue(x, 20);
		tempRoot->replaceChild(testRotation, ccp );*/

			//SbVec3f x;x.setValue(10, 0, 0);
			//ccp->rotation.setValue(x, rotationAngle);
			//theView.getRoot()->replaceChild(testRotation, ccp );
			//rotationAxis.setValue()
		}
		//pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, ss.x, ss.y, m_CWnd2);
	}
	//MessageBox(m_HWND2,_T("mykeypress"),_T("dd"),0);
}


//void getPosition(void *data, SoSensor *sensor)
//{
//	SoSFVec3f *pos = (SoSFVec3f *)((SoFieldSensor *)sensor)->getAttachedField();  
//	pos->getValue()[1];
//	pos->getValue()[2];
//	pos->getValue()[3];
//	//printf("detected camera movement: %f %f %f\n",   
//	//	pos->getValue()[0], pos->getValue()[1], pos->getValue()[2]);
//}

//static void  
//	triangle_cb(void * userdata, SoCallbackAction * action,  
//	const SoPrimitiveVertex * v1,  
//	const SoPrimitiveVertex * v2,  
//	const SoPrimitiveVertex * v3)  
//{  
//	printf("ss");
//}  
//
//
void
	mySelectionCB(void *, SoPath *selectionPath)
{
	SoSFEnum dd = theApp.m_Selection->policy;
	if (dd.getValue() == 1)
	{
		int SelNum = theApp.m_Selection->getNumSelected();

		SbViewportRegion myViewport(theApp.m_pViewer->getViewportRegion());
		SoGetBoundingBoxAction bboxAction(myViewport);
		SoPath * path = new SoPath(*selectionPath);
		path->ref();
		//path->truncate(path->getLength() - 1);
		SoSeparator * group = (SoSeparator *) path->getTail();
		//SoTransform * transform = (SoTransform *) group->getChild(0);
		if ( group->isOfType(SoFaceSet::getClassTypeId()))
		{
			int testLength = path->getLength();
			SoNode * testNode = path->getNodeFromTail(2);
			if (testNode->isOfType(SoGroup::getClassTypeId()))
			{
				//SoChildList *testList = testNode->getChildren();
				SbName dd = testNode->getName();

				SbString sbStr1,sbStr2;
				sbStr1.fromWideChar(theApp.SbName2WChar(dd));
				sbStr2.fromWideChar(_T("COLOR"));
				sbStr1 = sbStr1+sbStr2;
				//theApp.myMessageBox(sbStr1);
				SoNode *testColor = testNode->getByName(sbStr1);
				if (testColor->isOfType(SoMaterial::getClassTypeId()))
				{
					SbString diffuse,temp;
					SbName nameFiled;
					nameFiled = "diffuseColor";
					temp = "0 1 0";
					if (SelNum == 1)
					{
						theApp.xx = testColor->getField(nameFiled);
						theApp.xx->set(temp);
					}
					if (SelNum == 2)
					{
						theApp.yy = testColor->getField(nameFiled);
						theApp.yy->set(temp);
					}
					/*int dddd = theApp.m_Selection->getNumSelected();
					theApp.myMessageBox(dddd);*/
					//SoPath *selectPath = theApp.m_Selection->getPath(1);
				}
			}
			//return;
		}
		bboxAction.apply(path);
		SbBox3f cpt = bboxAction.getBoundingBox();
		//path->append(transform);
		//SoTransformBoxManip *manip = new SoTransformBoxManip;
		//manip->replaceNode(path);
		SoSwitch * ff = new SoSwitch;
		path->unref();
		//////////////////////////////////////////////////////////////////////////
		
		if (SelNum == 2)
		{
			//MessageBox(_T("begin"));
			// 是否确定对话框
			JudgementDLG judge;
			judge.DoModal();
			//////////////////////////////////////////////////////////////////////////
			//theApp.m_Selection->deselectAll();
			//theApp.m_Selection->policy.setValue(SoSelection::SINGLE);
			return;
		}
	}
	
	
	if (dd.getValue() == 0)
	{
		SbViewportRegion myViewport(theApp.m_pViewer->getViewportRegion());
		SoGetBoundingBoxAction bboxAction(myViewport);
		SoPath * path = new SoPath(*selectionPath);
		path->ref();
		//path->truncate(path->getLength() - 1);
		SoSeparator * group = (SoSeparator *) path->getTail();
		//SoTransform * transform = (SoTransform *) group->getChild(0);
		if ( group->isOfType(SoFaceSet::getClassTypeId()))
		{
			int testLength = path->getLength();
			SoNode * testNode = path->getNodeFromTail(2);
			if (testNode->isOfType(SoGroup::getClassTypeId()))
			{
				//SoChildList *testList = testNode->getChildren();
				SbName dd = testNode->getName();

				SbString sbStr1,sbStr2;
				sbStr1.fromWideChar(theApp.SbName2WChar(dd));
				sbStr2.fromWideChar(_T("COLOR"));
				sbStr1 = sbStr1+sbStr2;
				//theApp.myMessageBox(sbStr1);
			
				SoNode *testColor = testNode->getByName(sbStr1);
				if (testColor->isOfType(SoMaterial::getClassTypeId()))
				{
					SbString diffuse,temp;
					SbName nameFiled;
					nameFiled = "diffuseColor";
					theApp.xx = testColor->getField(nameFiled);
					/*int dddd = theApp.m_Selection->getNumSelected();
					theApp.myMessageBox(dddd);*/
					//SoPath *selectPath = theApp.m_Selection->getPath(1);
					temp = "0 1 0";
					theApp.xx->set(temp);
				}
			}
			return;
		}
		bboxAction.apply(path);
		SbBox3f cpt = bboxAction.getBoundingBox();
		//path->append(transform);
		//SoTransformBoxManip *manip = new SoTransformBoxManip;
		//manip->replaceNode(path);
		SoSwitch * ff = new SoSwitch;
		path->unref();
	}
	
}

// This routine is called whenever an object gets deselected. 
// We determine which object was deselected, and reset 
// that objects material color.
void
	myDeselectionCB(void *, SoPath *deselectionPath)
{
	SbString temp = "1 0 0";
	SoSFEnum dd = theApp.m_Selection->policy;
	if (dd.getValue() == 1)
	{
		int SelNum = theApp.m_Selection->getNumSelected();
		if (SelNum == 0)
		{
			theApp.xx->set(temp);
		}
		if (SelNum == 1)
		{
			//deselectionPath
			SbViewportRegion myViewport(theApp.m_pViewer->getViewportRegion());
			SoGetBoundingBoxAction bboxAction(myViewport);
			SoPath * path = new SoPath(*deselectionPath);
			path->ref();
			if (path->getLength() == 0)
			{
				theApp.xx->set(temp);
			}
		}
		if (SelNum == 2)
		{
			SbViewportRegion myViewport(theApp.m_pViewer->getViewportRegion());
			SoGetBoundingBoxAction bboxAction(myViewport);
			SoPath * path = new SoPath(*deselectionPath);
			path->ref();
			if (path->getLength() == 0)
			{
				theApp.xx->set(temp);
				theApp.yy->set(temp);
				theApp.m_Selection->deselectAll();
			}
		}
	}
	if (dd.getValue() == 0)
	{
		theApp.xx->set(temp);
		//int dddd = theApp.m_Selection->getNumSelected();
		//theApp.myMessageBox(dddd);
		if (deselectionPath->getTail()->
			isOfType(SoCube::getClassTypeId())) {
				textMaterial->diffuseColor.setValue(white);
		} else if (deselectionPath->getTail()->
			isOfType(SoCylinder::getClassTypeId())) {
				sphereMaterial->diffuseColor.setValue(white);
		}
	}
	
	
}

void ViewerCB(void *, SoWinViewer *viewIn)
{
	printf("ss");
	viewIn->getCamera();
	//theApp.m_pViewer->setFeedbackSize(100);
	//theApp.m_pViewer->setFeedbackVisibility(TRUE);
}

void selectCB(void * closure, SoPath * p)
{
	SbViewportRegion myViewport(theApp.m_pViewer->getViewportRegion());
	SoGetBoundingBoxAction bboxAction(myViewport);
	SoPath * path = new SoPath(*p);
	path->ref();
	//path->truncate(path->getLength() - 1);
	SoSeparator * group = (SoSeparator *) path->getTail();
	//SoTransform * transform = (SoTransform *) group->getChild(0);
	if ( !group->isOfType(SoFaceSet::getClassTypeId()) )
		return;
	bboxAction.apply(path);
	SbBox3f cpt = bboxAction.getBoundingBox();
	//path->append(transform);
	//SoTransformBoxManip *manip = new SoTransformBoxManip;
	//manip->replaceNode(path);
	SoSwitch * ff = new SoSwitch;
	path->unref();
}

void COpenInventorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	// TODO: 在此添加专用代码和/或调用基类
	// BEGIN OIV
	//
	// 初始化窗口句柄  窗口相关设置
	theApp.m_ViewHwnd = GetSafeHwnd();
	theApp.m_ViewCWnd = GetParent();

	theApp.m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
	//theApp.m_pViewer = new SoWinExaminerViewer(GetSafeHwnd(),0,1,SoWinFullViewer::BUILD_NONE,	SoWinViewer::BROWSER);
	theApp.m_pViewer->setTransparencyType(SoGLRenderAction::SORTED_LAYERS_BLEND);
	theApp.m_pViewer->addStartCallback(ViewerCB, NULL);
	//theApp.m_pViewer = new SoWinExaminerViewer(GetSafeHwnd());
//	theApp.m_pViewer->setEventCallback();
	//SoWinViewerCB(void * userData, SoWinViewer * viewer);
	//theApp.m_pViewer->addStartCallback(SoWinViewerCB(), NULL);

	theApp.m_Selection = new SoSelection;
	theApp.m_Selection->ref();

	theApp.m_Selection->policy.setValue(SoSelection::SINGLE);
	//theApp.m_Selection->policy.setValue(SoSelection::TOGGLE);
	theApp.m_Selection-> addSelectionCallback(mySelectionCB);
	theApp.m_Selection-> addDeselectionCallback(myDeselectionCB);
	
	SoSolidViz::init(); // 支持布尔运算
	// 初始化根节点
	theApp.m_Root = new SoSeparator();

	theApp.m_MainRoot = new SoSeparator();
	//theApp.m_Root->addChild(theApp.m_MainRoot);
	//theApp.m_MainRoot = new SoSelection();
	theApp.m_MainRoot->setName("ProRoot");
	theApp.m_MainRoot->ref();
	
	//SoGLRenderAction::setSmoothing(TRUE);


	//SoPolygonOffset * offset = new SoPolygonOffset;
	//offset->styles.setValue(SoPolygonOffset::FILLED);
	//offset->factor.setValue(-1);
	//offset->units.setValue(1);
	//offset->on.setValue(TRUE);
	//theApp.m_MainRoot->addChild(offset);

	// 深度缓存
	theApp.blueDepth =  new SoDepthBuffer;
	theApp.blueDepth->setName("ProDepth");
	theApp.blueDepth->test.setValue(TRUE);
	//blueDepth->function.setValue(SoDepthBuffer::ALWAYS);
	//theApp.m_MainRoot->addChild(theApp.blueDepth);
	//m_Selection->addChild(m_MainNode);
	//m_Selection->addChild(m_Selection);
	//theApp.sh = new SoShapeHints ;
	//theApp.sh->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE ;
	//theApp.m_MainRoot->addChild(theApp.sh);
	//////////////////////////////////////////////////////////////////////////
	// 初始化事件节点
	theApp.m_EventNode = new SoEventCallback;
	theApp.m_EventNode->setName("ProLiveEvent");
	//myEventCB->addEventCallback(SoKeyboardEvent::getClassTypeId(), myKeyPressCB, NULL);
	theApp.m_EventNode->addEventCallback(SoLocation2Event::getClassTypeId(), mouseMoveCB, NULL);
	theApp.m_EventNode->addEventCallback(SoMouseButtonEvent::getClassTypeId(), mousePressCB, NULL);
	//m_EventNode->addEventCallback(SoMouseWheelEvent::getClassTypeId(), this::mouseWheelCB, NULL);
	//theApp.m_MainRoot->addChild(theApp.m_EventNode);

	theApp.m_mainCam = theApp.m_pViewer->getCamera();
	//SoCallbackAction ca;  
	//ca.addTriangleCallback(SoCamera::getClassTypeId(), triangle_cb, NULL);  
	//ca.apply(m_MainNode);
	//Cam->callback(testAction);
	
	//////////////////////////////////////////////////////////////////////////
	// 初始化位移节点
	theApp.m_translation = new SoTranslation();
	theApp.m_translation->setName("ProTranslation");
	//theApp.m_MainRoot->addChild(theApp.m_translation);
	//m_Selection->addChild(m_translation);
	//////////////////////////////////////////////////////////////////////////
	// 初始化旋转节点
	//SoGetMatrixAction *tt;

	//m_rotation = new SoRotor();
	//m_rotation->speed.setValue(0);
	////m_rotation->getMatrix()
	//m_rotation->setName("ProRotation");
	//m_MainNode->addChild(m_rotation);

	//m_ro = new SoRotation();
	////SbRotation sb1 = m_ro->rotation.getValue();
	////SbRotation sb2;
	////sb2.setValue(1,1,1,1);
	////sb2 = sb1*sb2;
	////m_ro->rotation.setValue(0.2,0,0,0);
	//m_ro->setName("ProRo");
	//m_MainNode->addChild(m_ro);
	//////////////////////////////////////////////////////////////////////////
	// 初始化光照节点
	theApp.m_DirectionalLight= new SoDirectionalLight;
	theApp.m_DirectionalLight->direction.setValue(100, 0, 0);
	theApp.m_DirectionalLight->color.setValue(1, 1, 1);
	theApp.m_DirectionalLight->setName("ProDirectionalLight");
	//theApp.m_MainRoot->addChild(theApp.m_DirectionalLight);

	//////////////////////////////////////////////////////////////////////////
	// 加载坐标节点
	SoInput Coordinate;
	if (Coordinate.openFile("axis.iv") == NULL)
	{
		exit(-1);
	}
	SoSeparator *axis = SoDB::readAll(&Coordinate);
	//theApp.m_MainRoot->addChild(axis);
	////////////////////////////////////////////////////////////////////////////
	////	加入自动化连接测试端头
	//SbVec3d temp1Vec, temp2Vec;
	//temp1Vec.setValue(0,0,0);
	//temp2Vec.setValue(-20, 20, -20);
	//CJointBar ConnectBar1(_T("Joint1"),temp1Vec),
	//	ConnectBar2(_T("Joint2"),temp2Vec, 0, 1.57, 0);

	//theApp.m_MainRoot->addChild(ConnectBar1.CreateBar());
	//theApp.m_MainRoot->addChild(ConnectBar2.CreateBar());

	//CLStyleBar testLBar;
	//testLBar.CreateBar();

	//m_Selection->addChild(axis);
	//SoMaterial *material = new SoMaterial;
	//material->diffuseColor.setValue(1.0, 0.0, 0.0);
	//SoGroup * Cone = new SoGroup;
	//Cone->addChild(material);Cone->addChild(new SoCone);
	//m_Root->addChild(Cone);
	
	theApp.m_pViewer->saveHomePosition();

	//////objectA
	//SoSeparator *objectA = new SoSeparator;
	//SoCube * myCube = new SoCube;
	//myCube->width.setValue(30);
	//myCube->height.setValue(100);
	//myCube->depth.setValue(10);
	///*objectA->addChild(TestTranslation);*/
	//theApp.m_MainRoot->addChild(myCube);
	//////objectB
	//SoSeparator *objectB = new SoSeparator;
	//SoCylinder * cylinder = new SoCylinder;
	//cylinder->height.setValue(20);
	//cylinder->radius.setValue(3);
	//SoRotation * NineRotationX = new SoRotation;
	//SbVec3f X_Axis;
	//X_Axis.setValue(1,0,0);
	//NineRotationX->rotation.setValue(X_Axis, 3.1415926535897932384626433832795/2);
	//SoTranslation * X_trans = new SoTranslation;
	//X_trans->translation.setValue(5, 40, 0);

	//theApp.m_MainRoot->addChild(X_trans);
	//theApp.m_MainRoot->addChild(NineRotationX);
	//theApp.m_MainRoot->addChild(cylinder);

// 	SoCube * testSphere = new SoCube;
// 	testSphere->width.setValue(1);testSphere->depth.setValue(1);testSphere->height.setValue(1);
// 	theApp.m_MainRoot->addChild(testSphere);
	//m_Selection->addChild(testSphere);
	//
	//SoCube * ccube2 = new SoCube;
	//ccube2->height.setValue(1);
	//ccube2->width.setValue(2);
	//ccube2->depth.setValue(1);
	//SoTranslation * t2 = new SoTranslation;
	//t2->translation.setValue(50,0,0);
	//m_MainNode->addChild(t2);

	SbColor BackColor(0.3, 0.3, 0.3);
	//theApp.m_pViewer->setDrawStyle(SoWinViewer::NORMAL, SoWinViewer::BROWSER);
	////m_pViewer->setCursorStyle(SoWinViewer::CLASSIC);
	theApp.m_pViewer->setBackgroundColor(BackColor);
	//theApp.m_pViewer->setCursorEnabled(FALSE);
	
	theApp.m_pViewer->setCursorEnabled(TRUE);
	//theApp.m_pViewer->setFeedbackVisibility(TRUE);
	theApp.m_pViewer->setViewing(FALSE);

	//theApp.m_pViewer->setFeedbackSize(100);
	//theApp.m_pViewer->setFeedbackVisibility(TRUE);
	
	//theApp.m_Selection->addSelectionCallback(selectCB, NULL);
	theApp.m_Selection->addChild(theApp.m_MainRoot);
	//theApp.m_Root->addChild(theApp.m_Selection);

	theApp.m_pViewer->setAntialiasing(TRUE, 5);
	theApp.m_pViewer->setSceneGraph(theApp.m_Selection);
	//theApp.m_pViewer->setSceneGraph(theApp.m_MainRoot);
	theApp.m_pViewer->setDecoration(FALSE);
	theApp.m_pViewer->viewAll();
	theApp.m_pViewer->show();
	//OnCreatmodel();
	//// END OIV
}

//char * stringToLPCTSTR(string sFrom)
//
//{
//	_TCHAR* sBuff = new _TCHAR[sFrom.length() + 2]; 
//
//	int iLength = 0;
//
//	iLength = wsprintf (sBuff,sFrom.c_str ());
//
//	sBuff[iLength+1] = '/0';
//
//	return sBuff;
//
//}

void COpenInventorView::OnCreatmodel()
{
	// 此乃测试 soinfo 的代码 测试 oiv编码
	// TODO: 在此添加命令处理程序代码
	SoInfo * testInfo = new SoInfo;
	
	CString ssd  = _T("dfas");
	//ssd->SetString(_T("df"));
	//ssd.SetString("sdf");
	SbString ss = _T("10"), sb, tt;
	// cstring 转 char
	CString origCString("Hello, World!"); 
	wchar_t* wCharString = origCString.GetBuffer(origCString.GetLength()+1); 
	/*	size_t origsize = wcslen(wCharString) + 1; 
	size_t convertedChars = 0; 
	char *CharString; 
	CharString=new char(origsize);*/ 
	//ss.fromUtf8(CharString);
	ss.fromWideChar(wCharString);
	//ss.fromUtf8("中文");
	testInfo->string.setValue(ss);
	
	//testInfo->string.set(ss);
	//testInfo->set(ss);
	//SbBool re = testInfo->set(ss);
	sb = testInfo->string.getValue();
	int idd = sb.toInt();
	//SbString en = sb.getSubString(0,1);
	const char * td = sb.getString();
	CString go;
	go = td;
	LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	MessageBox(lpstr);

	//SoCSGOperation *test = new SoCSGOperation;
	//test->setOperationType(SoCSGOperation::CUT);
	//// Object A
	//SoSeparator *objectA = new SoSeparator;
	//objectA->addChild(new SoCube);

	//// Object B
	//SoSeparator *objectB = new SoSeparator();
	//SoSphere *sphere = new SoSphere;
	//SoTranslation *translation = new SoTranslation;
	//translation->translation.setValue(0.4f, 0.0f, 0.0f);
	//sphere->radius = 1.2f;
	//objectB->addChild(translation);
	//objectB->addChild(sphere);

	//test->setSceneGraphs(objectA, objectB);
	//SoTranslation * TestTranslation = new SoTranslation;
	//TestTranslation->translation.setValue(ss, 0.0, 0.0);
	//SoSeparator *result = new SoSeparator;
	//result->addChild(TestTranslation);
	//result->addChild(test->getSceneGraph());
	//m_Root->addChild(result);
	//ss = ss + 10;
	//m_pViewer->resetToHomePosition();
	
}

SoSeparator * COpenInventorView::addNodeFun( void )
{
	// 木有用了
	SoSeparator * resSep = new SoSeparator;
	SoSeparator * tempSep = new SoSeparator;
	// SoTranslation * TestTranslation = new SoTranslation;
	// TestTranslation->translation.setValue(ss, 0.0, 0.0);
	//SoCSGGroup *m_csgGroup;
	SoCSGOperation * CSGOP;
	CSGOP->setOperationType(SoCSGOperation::CUT);
	//m_csgGroup = new SoCSGGroup;
	//m_csgGroup->expression.setValue("A-B");
	//tempSep->addChild(m_csgGroup);


	//SoMaterial * mat = new SoMaterial;
	//mat->diffuseColor.setValue(SbColor(0.0, 1.0, 0.0));

	// object A
	SoSeparator *objectA = new SoSeparator;

	////objectA->addChild(mat);
	SoSphere * mySphere = new SoSphere;

	//objectA->addChild(TestTranslation);
	objectA->addChild(mySphere);
	//objectA->unref();
	//m_csgGroup->addChild(objectA);

	//ss = ss + 100;

	// object B
	SoSeparator *objectB = new SoSeparator;
	SoCylinder * cylinder = new SoCylinder;
	cylinder->height.setValue(200);
	cylinder->radius.setValue(10);
	//SoSphere * sphere = new SoSphere;
	//SoTranslation * translation = new SoTranslation;
	//translation->translation.setValue(0.4, 0.0, 0.0);
	//sphere->radius = 1.2;
	//objectB->addChild(mat);
	//objectB->addChild(translation);
	objectB->addChild(cylinder);
	//objectB->unref();
	//m_csgGroup->addChild(objectB);

	CSGOP->setSceneGraphs(objectA,objectB);
	//CSGOP.setSceneGraphs(objectA,objectB);
	SoSeparator *result = CSGOP->getSceneGraph();
	theApp.m_MainRoot->addChild(result);

	//SoTranslation * TestTranslation = new SoTranslation;
	//TestTranslation->translation.setValue(ss, 0.0, 0.0);
	//resSep->addChild(TestTranslation);
	//resSep->addChild(tempSep);
	//ss = ss + 100;

	return resSep;
}

void COpenInventorView::testFun( void *userdata, SoEventCallback *node )
{
	// 测试函数
}

SoWinExaminerViewer * COpenInventorView::getViewer( void )
{
	return theApp.m_pViewer;
}


void COpenInventorView::OnFile1()
{
	MessageBox(_T("You program run there!"));
	// TODO: 在此添加命令处理程序代码

}

void COpenInventorView::OnDestroy()
{
	CView::OnDestroy();
	//delete m_pViewer;
	// TODO: 在此处添加消息处理程序代码
}

double COpenInventorView::get2PointDis( CPoint pt1, CPoint pt2 )
{
	
	double res = (pt1.x - pt2.x)*(pt1.x-pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y);
	//LONG res = (pt1.x - pt2.x)*(pt1.x-pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y);
	if (res == 0)
	{
		return 0;
	}
	res = sqrt(res);
	return res;
}

double COpenInventorView::getAngle( double distance2Pt )
{
	// R 要乘以一个比例的
	double res = 2 * atan(distance2Pt/(2*theApp.m_RadiusView));
	return res;
}

double COpenInventorView::getSlope( CPoint pt1, CPoint pt2 )
{
	if ((pt1.x - pt2.x) == 0)
	{
		return 0;
	}
	double k = (pt1.y - pt2.y)/(pt1.x - pt2.x);
	return k;
}

double COpenInventorView::getArcSlope( double k )
{
	return -(1/k);
}

SbVec2f COpenInventorView::getPt4( double dis, double arcK )
{
	float x = sqrt((dis*dis)/(1+arcK*arcK));
	float y = arcK*x;
	SbVec2f res;
	res.setValue(x,y);
	return res;
}

SbVec2f COpenInventorView::getMidPt( CPoint pt1, CPoint pt2 )
{
	SbVec2f res;
	res.setValue((pt1.x+pt2.x)/2, (pt1.y+pt2.y)/2);
	return res;
}

SbVec3f COpenInventorView::getNormalVec( SbVec2f pt4, SbVec2f midPt )
{
	SbVec3f res;
	SbVec2f temp = pt4-midPt;
	float x,y;
	temp.getValue(x, y);
	x = x/temp.length();
	y = y/temp.length();
	res.setValue(x, y, 0);
	return res;
}

int COpenInventorView::hemishere( int x, int y, int d,COORDINATE3 &v)
{
	float z;
	
	// 计算x, y坐标
	//get<0>(TableUnit) = 0;
	get<0>(v) = (float)x * 2.0 - (float)d;
	get<1>(v) = (float)d - (float)y * 2.0;
	//v[0] = (float)x * 2.0 - (float)d;
	//v[1] = (float)d - (float)y * 2.0;
	// 计算z坐标
	z = d * d - get<0>(v) * get<0>(v) - get<1>(v) * get<1>(v);
	//z = d * d - v[0] * v[0] - v[1] * v[1];
	if (z < 0)
	{
		return 0;
	}
	get<2>(v) = sqrt(z);
	// 单位化
	get<0>(v) /= (float)d;
	get<1>(v) /= (float)d;
	get<2>(v) /= (float)d;
	return !0;
}

void COpenInventorView::OnViewStatusBar()
{
	// TODO: 在此添加命令处理程序代码
	// viweing 测试代码
	// TODO: 在此添加命令处理程序代码
	if(theApp.ViewingToF == TRUE)
	{
		theApp.m_pViewer->setViewing(FALSE);
		theApp.m_pViewer->setFeedbackVisibility(TRUE);
		//theApp.m_pViewer->setFeedbackSize(1000);
		theApp.m_pViewer->setAnimationEnabled(false);
		theApp.m_pViewer->setViewerMode();
		theApp.ViewingToF = false;
	}
	else
	{
		theApp.m_pViewer->setViewing(TRUE);
		theApp.m_pViewer->setFeedbackVisibility(TRUE);
		theApp.m_pViewer->setAnimationEnabled(false);
		theApp.m_pViewer->setViewerMode();
		theApp.ViewingToF = true;
	}
}

void COpenInventorView::rotView()
{
	//m_pViewer->rotateCamera(tt);
	//m_pViewer->camera->position.getValue();
	//*m_pV =  m_pViewer;
// 	SoWinExaminerViewer *temp;
// 	temp = *m_pV;
	theApp.m_pViewer->setViewing(true);
	//temp->setViewing(true);
	//*m_pV->getCamera();
	//&m_pV->rotateCamera(tt);
}

void COpenInventorView::TestSoFaceSet()
{
	static const float norms[3][3] =
	{ 
		{0.0f, 0.0f, 1.0f}, {1.f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}
	};

	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	myVertexProperty->normal.setValues(0, 3, norms);
	myVertexProperty->normalBinding = SoNormalBinding::PER_FACE;
	static const float vertices[12][3] =
	{
		{ 0.0f, 0.0f, 0.0f}, {10.0f, 0.0f, 0.0f}, { 10.0f, 10.0f, 0.0f}, { 0.0f, 10.0f, 0.0f}, //xy
		{ 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 10.0f}, { 0.0f,10.0f, 10.0f}, { 0.0f, 10.0f, 0.0f}, //zy
		{ 0.0f, 0.0f, 0.0f}, {0.0f,0.0f, 10.0f}, { 10.0f,0.0f, 10.0f}, { 10.0f, 0.0f, 0.0f}   //zx
	};
	myVertexProperty->vertex.setValues(0,12,vertices);
	//static int32_t numvertices[8] = {3, 3, 3, 3, 4, 4, 4, 4};
	static int32_t numvertices[3] = {4, 4, 4};
	SoFaceSet *myFaceSet = new SoFaceSet;
	myFaceSet->numVertices.setValues(0, 3,numvertices);
	myFaceSet->vertexProperty.setValue(myVertexProperty);
	theApp.m_MainRoot->addChild(myFaceSet);
}
