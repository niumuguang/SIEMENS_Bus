#include "StdAfx.h"
#include "StraightBar.h"
#include <Inventor\nodes\SoCylinder.h>
#include <SolidViz\SoCSGOperation.h>
#include <Inventor\nodes\SoCube.h>
#include <SolidViz\SoSolidViz.h>
#include <SolidViz\nodes\SoCSGGroup.h>
#include <Inventor\engines\SoBoolOperation.h>
#include <Inventor\Gui\viewers\SoGuiViewer.h>
#include <Inventor\nodes\SoDepthBuffer.h>
#include <Inventor\nodes\SoFaceSet.h>
#include <Inventor\nodes\SoShapeHints.h>
#include <Inventor\nodes\SoSwitch.h>

CStraightBar::CStraightBar(void)
{
	// 位移值
	m_Trans.setValue(0,0,0);
	// 初始化I型链接排
	m_Name = "CONN1";
	// 高度 其实就是长
	m_Height = 24;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	//////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13;CircleCenterList.push_back(tempSet);
	SoSolidViz::init(); // 支持布尔运算
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
}

CStraightBar::CStraightBar( CString barName, SbVec3d trans)
{
	// 位移
	m_Trans = trans;
	// 初始化I型链接排
	m_Name = barName;
	// 高度 其实就是长
	m_Height = 15;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	//////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13;CircleCenterList.push_back(tempSet);
	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	
	SoSolidViz::init(); // 支持布尔运算
}

CStraightBar::CStraightBar( CString barName, SbVec3d trans, float xRot, float yRot, float zRot )
{
	// 位移
	m_Trans = trans;
	// 初始化I型链接排
	m_Name = barName;
	// 高度 其实就是长
	m_Height = 24;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	//////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14+9;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14+9;CircleCenterList.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13+9;CircleCenterList.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13+9;CircleCenterList.push_back(tempSet);
	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = xRot; m_yRot = yRot; m_zRot = zRot;
	SoSolidViz::init(); // 支持布尔运算
}

CStraightBar::~CStraightBar(void)
{
	
}
//////////////////////////////////////////////////////////////////////////
// 老式的布尔运算方法
SoGroup * CStraightBar::CreateStraightBar()
{
	//////////////////////////////////////////////////////////////////////////
	// 老式的布尔运算方法

	// 返回的组
	m_Bar = new SoSeparator;
	//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
	//blueDepth->test.setValue(TRUE);
	//blueDepth->function.setValue(SoDepthBuffer::ALWAYS);
	//m_Bar->addChild(blueDepth);
	// 名称节点
	tempStr = m_Name+_T("GROUP");
	m_tempName = (char *)tempStr.GetBuffer();
	m_Bar->setName(m_tempName);
	// 颜色节点
	m_Color = new SoBaseColor;
	m_Color->rgb.setValue(252, 132, 41);
	tempStr = m_Name+_T("COLOR");
	m_tempName = (char *)tempStr.GetBuffer();
	m_Color->setName(m_tempName);
	// 位移节点
	m_Translation = new SoTranslation;
	m_Translation->translation.setValue(m_Trans);
	tempStr = m_Name+_T("TRANSLATION");
	m_tempName = (char *)tempStr.GetBuffer();
	m_Translation->setName(m_tempName);
	// 旋转节点
	m_RotationX = new SoRotationXYZ;
	m_RotationY = new SoRotationXYZ;
	m_RotationZ = new SoRotationXYZ;
	m_RotationX->axis.setValue(SoRotationXYZ::X);
	m_RotationY->axis.setValue(SoRotationXYZ::Y);
	m_RotationZ->axis.setValue(SoRotationXYZ::Z);

	SoGroup * m_Rotation = new SoGroup;
	tempStr = m_Name+_T("ROTATION");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Rotation->setName(tempSbStr);
	m_Rotation->addChild(m_RotationX);m_Rotation->addChild(m_RotationY);m_Rotation->addChild(m_RotationZ);
	//////////////////////////////////////////////////////////////////////////
	m_Bar->addChild(m_Translation);
	m_Bar->addChild(m_Rotation);
	m_Bar->addChild(m_Color);
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//		使用SoCSGGroup实现布尔运算
	//////////////////////////////////////////////////////////////////////////
	/*SoCSGGroup *m_csgGroup;*/
	m_csgGroup = new SoCSGGroup;
	m_csgGroup->expression.setValue("A-B-C-D-E-F-G");
	m_Bar->addChild(m_csgGroup);

	////objectA
	SoSeparator *objectA = new SoSeparator;
	SoCube * myCube = new SoCube;
	myCube->width.setValue(m_Width);
	myCube->height.setValue(m_Height);
	myCube->depth.setValue(m_Depth-5);
	m_csgGroup->addChild(myCube);

	////objectB
	SoSeparator *objectB = new SoSeparator;
	SoCylinder * cylinder = new SoCylinder;
	cylinder->height.setValue(m_Depth*3);
	cylinder->radius.setValue(m_Diameter);
	SoRotation * NineRotationX = new SoRotation;
	SbVec3f X_Axis;X_Axis.setValue(1,0,0);
	NineRotationX->rotation.setValue(X_Axis, (float)PI/2);
	SoTranslation * X_trans1 = new SoTranslation;
	X_trans1->translation.setValue(8, 40, 0);
	objectB->addChild(X_trans1);
	objectB->addChild(NineRotationX);
	objectB->addChild(cylinder);
	m_csgGroup->addChild(objectB);

	SoSeparator *objectC = new SoSeparator;
	SoTranslation * X_trans2 = new SoTranslation;
	X_trans2->translation.setValue(-8, 40, 0);
	objectC->addChild(X_trans2);
	objectC->addChild(NineRotationX);
	objectC->addChild(cylinder);
	m_csgGroup->addChild(objectC);

	SoSeparator *objectD = new SoSeparator;
	SoTranslation * X_trans3 = new SoTranslation;
	X_trans3->translation.setValue(-8, -40, 0);
	objectD->addChild(X_trans3);
	objectD->addChild(NineRotationX);
	objectD->addChild(cylinder);
	m_csgGroup->addChild(objectD);

	SoSeparator *objectE = new SoSeparator;
	SoTranslation * X_trans4 = new SoTranslation;
	X_trans4->translation.setValue(8, -40, 0);
	objectE->addChild(X_trans4);
	objectE->addChild(NineRotationX);
	objectE->addChild(cylinder);
	m_csgGroup->addChild(objectE);

	SoSeparator *objectF = new SoSeparator;
	SoTranslation * X_trans5 = new SoTranslation;
	X_trans5->translation.setValue(8, -30, 0);
	objectF->addChild(X_trans5);
	objectF->addChild(NineRotationX);
	objectF->addChild(cylinder);
	m_csgGroup->addChild(objectF);

	SoSeparator *objectG = new SoSeparator;
	SoTranslation * X_trans6 = new SoTranslation;
	X_trans6->translation.setValue(-8, -30, 0);
	objectG->addChild(X_trans6);
	objectG->addChild(NineRotationX);
	objectG->addChild(cylinder);
	m_csgGroup->addChild(objectG);
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//
	//		使用SoCSGOperation 进行布尔运算
	//////////////////////////////////////////////////////////////////////////
//	SoCSGOperation * CSGOP = new SoCSGOperation;
//	CSGOP->setOperationType(SoCSGOperation::AND);
//	//CSGOP->setOperationType(SoCSGOperation::MERGE);
//
//	////objectA
//	SoSeparator *objectA = new SoSeparator;
//
//	SoCube * myCube = new SoCube;
//	myCube->width.setValue(m_Width);
//	myCube->height.setValue(m_Height);
//	myCube->depth.setValue(m_Depth);
//	objectA->addChild(myCube);
//	////objectB
//	SoSeparator *objectB = new SoSeparator;
//
//	SoCylinder * cylinder = new SoCylinder;
//	cylinder->height.setValue(m_Depth+1);
//	cylinder->radius.setValue(m_Diameter);
//	SoRotation * NineRotationX = new SoRotation;
//	SbVec3f X_Axis;X_Axis.setValue(1,0,0);
//	NineRotationX->rotation.setValue(X_Axis, (float)PI/2);
//	SoTranslation * X_trans1 = new SoTranslation;
//	X_trans1->translation.setValue(8, 40, 0);
//	objectB->addChild(X_trans1);
//	objectB->addChild(NineRotationX);
//
//	objectB->addChild(cylinder);
//
//	//SoCylinder * cylinder2 = new SoCylinder;
//	//cylinder2->height.setValue(m_Depth*2);
//	//cylinder2->radius.setValue(m_Diameter);
//	SoTranslation * X_trans2 = new SoTranslation;
//	X_trans2->translation.setValue(-16, 0, 0);
//	objectB->addChild(X_trans2);
//	objectB->addChild(cylinder);
//
// 	SoTranslation * X_trans3 = new SoTranslation;
// 	X_trans3->translation.setValue(0, 0, 10);
// 	//objectB->addChild(X_trans3);
// 	//objectB->addChild(cylinder);
// 	objectB->addChild(X_trans3);
// 	objectB->addChild(cylinder);
//
// 	SoTranslation * X_trans4 = new SoTranslation;
// 	X_trans4->translation.setValue(16, 0, 0);
// 	objectB->addChild(X_trans4);
// 	objectB->addChild(cylinder);
//
//	//SoTranslation * X_trans5 = new SoTranslation;
//	//X_trans5->translation.setValue(0, 0, 70);
//	//objectB->addChild(X_trans5);
//	//objectB->addChild(cylinder);
//
//	//SoTranslation * X_trans6 = new SoTranslation;
//	//X_trans6->translation.setValue(-16, 0, 0);
//	//objectB->addChild(X_trans6);
//	//objectB->addChild(cylinder);
//
//// 	SoTranslation * X_trans7 = new SoTranslation;
//// 	X_trans7->translation.setValue(0, 0, -10);
//// 	objectB->addChild(X_trans7);
//// 	objectB->addChild(cylinder);
//	
//// 	SoTranslation * X_trans8 = new SoTranslation;
//// 	X_trans8->translation.setValue(16, 0, 0);
//// 	objectB->addChild(X_trans8);
//// 	objectB->addChild(cylinder);
//	CSGOP->setSceneGraphs(objectA,objectB);
//	SoSeparator *result = CSGOP->getSceneGraph();
//
//	m_Bar->addChild(result);
	//////////////////////////////////////////////////////////////////////////

	//m_Info.setUserData("sddf");
	//m_Info.getUserData();
	return m_Bar;
}

// 
void CStraightBar::InitIStyleBar( void )
{
	
}

SoSeparator * CStraightBar::CreateBar()
{
	//////////////////////////////////////////////////////////////////////////
	//	 新式的布尔运算方法
	InitIStyleBar();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 初始化孔的数据， 暂时将孔分为四等分用于测试
	SetCircleDatad();
	//////////////////////////////////////////////////////////////////////////
	// 设置法向
	SoShapeHints * m_hints = new SoShapeHints;
	m_hints->windingType=SoShapeHints::ODD_TYPE;
	m_hints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	//m_Bar->addChild(m_hints);

 	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	//myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
	//myVertexProperty->normal.setValues(0, 9, norms);
	//myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
	//myVertexProperty->normalBinding = SoNormalBinding::PER_FACE;
	myVertexProperty->normalBinding = SoNormalBinding::OVERALL;
	// 每个孔4个点 共8个孔 加外框4个点
	myVertexProperty->vertex.setValues(0, 300,CutCircleData_8);
	//myVertexProperty->vertex.setValues(0, 7, vertices);
	static int32_t numvertices[9] = {4,37,37,37,37,37,37,37,37};
	// 面对向
	SoFaceSet *UpFace = new SoFaceSet;
	// 绘制顶面
	UpFace->numVertices.setValues(0, 9, numvertices);
	UpFace->vertexProperty.setValue(myVertexProperty);
	//m_Bar->addChild(myFaceSet);
	// 绘制底面
	SoFaceSet *myFaceSet2 = new SoFaceSet;
	myFaceSet2 = UpFace;
	SoTranslation * depthTran = new SoTranslation;
	depthTran->translation.setValue(0,0,-m_Depth);
	SoSeparator * DownFace = new SoSeparator;
	DownFace->addChild(depthTran);
	DownFace->addChild(m_hints);
	DownFace->addChild(myFaceSet2);


	SoSeparator * UpFaceSep = new SoSeparator;
	UpFaceSep->setName("UpData");
	// 加入上下面分支节点
	//UpDownFace->addChild(m_hints);
	//UpDownFace->addChild(UpFace);
	//UpDownFace->addChild(DownFace);
	//m_Bar->addChild(UpDownFace);
	// 上面的
	 SoTranslation * UpFaceTra = new SoTranslation;
	 UpFaceSep->addChild(m_hints);
	 UpFaceSep->addChild(UpFace);
	 m_Bar->addChild(UpFaceSep);

	 m_Bar->addChild(DownFace);
	// 绘制内部
	SoVertexProperty *myVertexPropertyIN = new SoVertexProperty;
	myVertexPropertyIN->normalBinding = SoNormalBinding::PER_FACE;
	//myVertexPropertyIN->vertex.setValues(0, 128, InCircleData_8);//1184
	myVertexPropertyIN->vertex.setValues(0, 1184, InCircleData_8);
	static int32_t IN_numvertices[296];
	for (int tt=0; tt<296;tt++)
	{
		IN_numvertices[tt] = 4;
	}
	//static int32_t IN_numvertices[32] = {
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//};
	SoFaceSet *myIN_FaceSet = new SoFaceSet;
	myIN_FaceSet->numVertices.setValues(0, 296, IN_numvertices);
	myIN_FaceSet->vertexProperty.setValue(myVertexPropertyIN);
	SoSeparator * CylinderFaces = new SoSeparator;
	CylinderFaces->setName("CylinderData");
	SoTranslation * InFaceTra = new SoTranslation;
	CylinderFaces->addChild(InFaceTra);
	SoShapeHints * InHints = new SoShapeHints;
	CylinderFaces->addChild(InHints);
	CylinderFaces->addChild(myIN_FaceSet);
	m_Bar->addChild(CylinderFaces);
	// 绘制外围
	SoVertexProperty *myVertexPropertyOut = new SoVertexProperty;
	myVertexPropertyOut->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyOut->vertex.setValues(0, 16, OutCircleData_8);
	static int32_t Out_numvertices[4] = {4,4,4,4};
	SoFaceSet *myOut_FaceSet = new SoFaceSet;
	myOut_FaceSet->numVertices.setValues(0, 4, Out_numvertices);
	myOut_FaceSet->vertexProperty.setValue(myVertexPropertyOut);
	SoSeparator * OutSkin = new SoSeparator;
	OutSkin->setName("OutSkin");
	SoTranslation * OutFaceTra = new SoTranslation;
	OutSkin->addChild(OutFaceTra);
	SoShapeHints * OutHints = new SoShapeHints;
	OutSkin->addChild(OutHints);
	OutSkin->addChild(myOut_FaceSet);
	m_Bar->addChild(OutSkin);
	return m_Bar;
	//theApp.m_MainRoot->addChild(myFaceSet);
}

AXIS CStraightBar::CirclePt( float RadiusInput, float AngleInput )
{
	float y = sin(AngleInput)*RadiusInput;
	float x = cos(AngleInput)*RadiusInput;
	AXIS res;
	get<0>(res) = x;
	get<1>(res) = y;
	get<2>(res) = 0;
	return res;
}

vector<AXIS> CStraightBar::GetPoints( float RadiusInput )
{
	// 输入半径，以π的一半进行分割圆
	vector<AXIS> res;//坐标集合
	//float unit = PI/2;//4个点
	float unit = PI/18; // 36个点
	AXIS tempPt;
	for (float i=0; i<=2*PI; i = i + unit)
	{
		tempPt = CirclePt(RadiusInput, i);//根据原点和角度计算孔外围点的坐标
		res.push_back(tempPt);
	}
	//vector<AXIS>::iterator itr = res.end();
	//res.erase(itr);//圆取36个会重复
	return res;//总共有36个点
}

void CStraightBar::SetCircleDatad( void )
{
	//////////////////////////////////////////////////////////////////////////
	// 设置表面数据
	//////////////////////////////////////////////////////////////////////////
	// 设置矩形数据
	CutCircleData_8[0][0] = 0;CutCircleData_8[0][1] = 0;CutCircleData_8[0][2] = 0;
	CutCircleData_8[1][0] = m_Width; CutCircleData_8[1][1] = 0; CutCircleData_8[1][2] = 0;
	CutCircleData_8[2][0] = m_Width; CutCircleData_8[2][1] = m_Height; CutCircleData_8[2][2] = 0;
	CutCircleData_8[3][0] =  0; CutCircleData_8[3][1] = m_Height; CutCircleData_8[3][2] = 0;

	// 设置孔数据
	vector<AXIS> Pts36;
	// 计算圆孔周围坐标点的集合
	Pts36 = GetPoints(m_Diameter/2);
	int num = 4;// 因为开始已经加入了4个边框点
	AXIS tempC, tempD;
	float X_Vec;
	float Y_Vec;
	float Z_Vec;
	float tempF;
	static int xnum, xnum2;
	// 针对每个开孔中心进行位移变化
	for(int j=0; j<CircleCenterList.size(); j++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList.at(j);
		X_Vec = float(get<0>(X_Y_Vec));
		Y_Vec = float(get<1>(X_Y_Vec));
		//X_Vec = 2;
		//Y_Vec = 2;
		Z_Vec = 0;
		for(int ro=0; ro<Pts36.size(); ro++)
		{
		tempC = Pts36.at(ro);
		xnum = num;
		CutCircleData_8[xnum][0] =get<0>(tempC) + X_Vec;
		CutCircleData_8[xnum][1] = get<1>(tempC) + Y_Vec;
		CutCircleData_8[xnum][2] = get<2>(tempC) + Z_Vec;
		num = num + 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置内部数据
	//////////////////////////////////////////////////////////////////////////
	num = 0;
	// 对每个圆心坐标点进行处理
	int i = 0;
	for(; i<CircleCenterList.size(); i++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList.at(i);
		X_Vec = float(get<0>(X_Y_Vec));//处理x坐标
		Y_Vec = float(get<1>(X_Y_Vec));//处理y坐标
		Z_Vec = 0;// 因为是平面所以Z坐标为零
		for(int ro=0; ro<Pts36.size(); ro++)
		{
			if (ro < Pts36.size()-1)
			{
				tempC = Pts36.at(ro);
				tempD = Pts36.at(ro+1);

				xnum2 = num;
				// 分别计算出 内圆住的四个点  ，要近似为圆的化还需要更多的点
				InCircleData_8[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_8[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_8[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_8[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_8[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_8[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_8[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_8[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_8[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_8[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_8[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_8[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			// 如果是最后一个点 和第一个点组成矩形
			else if (ro == Pts36.size() - 1)
			{
				tempC = Pts36.at(ro);
				tempD = Pts36.at(0);

				xnum2 = num;
				InCircleData_8[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_8[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_8[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_8[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_8[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_8[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_8[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_8[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_8[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_8[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_8[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_8[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			num = num + 4;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置外框数据
	// 就是矩形 围边的数据
	//////////////////////////////////////////////////////////////////////////
	OutCircleData_8[0][0] = 0;OutCircleData_8[0][1] = 0;OutCircleData_8[0][2] = 0;
	OutCircleData_8[1][0] = 0;OutCircleData_8[1][1] = m_Height;OutCircleData_8[1][2] = 0;
	OutCircleData_8[2][0] = 0;OutCircleData_8[2][1] = m_Height;OutCircleData_8[2][2] = -m_Depth;
	OutCircleData_8[3][0] = 0;OutCircleData_8[3][1] = 0;OutCircleData_8[3][2] = -m_Depth;

	OutCircleData_8[4][0] = 0;OutCircleData_8[4][1] = m_Height;OutCircleData_8[4][2] = 0;
	OutCircleData_8[5][0] = m_Width;OutCircleData_8[5][1] = m_Height;OutCircleData_8[5][2] = 0;
	OutCircleData_8[6][0] = m_Width;OutCircleData_8[6][1] = m_Height;OutCircleData_8[6][2] = -m_Depth;
	OutCircleData_8[7][0] = 0;OutCircleData_8[7][1] = m_Height;OutCircleData_8[7][2] = -m_Depth;

	OutCircleData_8[8][0] = m_Width;OutCircleData_8[8][1] = m_Height;OutCircleData_8[8][2] = 0;
	OutCircleData_8[9][0] = m_Width;OutCircleData_8[9][1] = 0;OutCircleData_8[9][2] = 0;
	OutCircleData_8[10][0] = m_Width;OutCircleData_8[10][1] = 0;OutCircleData_8[10][2] = -m_Depth;
	OutCircleData_8[11][0] = m_Width;OutCircleData_8[11][1] = m_Height;OutCircleData_8[11][2] = -m_Depth;

	OutCircleData_8[12][0] = m_Width;OutCircleData_8[12][1] = 0;OutCircleData_8[12][2] = 0;
	OutCircleData_8[13][0] = 0;OutCircleData_8[13][1] = 0;OutCircleData_8[13][2] = 0;
	OutCircleData_8[14][0] = 0;OutCircleData_8[14][1] = 0;OutCircleData_8[14][2] = -m_Depth;
	OutCircleData_8[15][0] = m_Width;OutCircleData_8[15][1] = 0;OutCircleData_8[15][2] = -m_Depth;
 }

 wchar_t* CStraightBar::Cstring2Wchar( CString cstr )
 {
	 //CString origCString("Hello, World!"); 
	 wchar_t* wCharString = cstr.GetBuffer(cstr.GetLength()+1); 
	 return wCharString;
 }

 void CStraightBar::setHwnd( Hwnd tt )
 {
	 mainHwnd = tt;
 }


