#include "StdAfx.h"
#include "Joint4.h"
#include <Inventor\nodes\SoShapeHints.h>
#include <Inventor\nodes\SoFaceSet.h>


CJoint4::CJoint4(void)
{
	// 位移值
	m_Trans.setValue(0,0,0);
	// 初始化I型链接排
	m_Name = "Joint";
	// 高度 其实就是长
	//m_Height = 15;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	//////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);

	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	//MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}

CJoint4::CJoint4( CString JName, double Width, 
	double Depth, double Diameter, SbVec3f Tran, 
	double XRot, double YRot, double ZRot, vector<COORDINATE2> ListA)
{
	// 位移值
	m_Trans.setValue(Tran);
	// 初始化I型链接排
	m_Name = JName;
	// 高度 其实就是长
	//m_Height = 15;
	// 宽
	m_Width = Width;
	// 深
	m_Depth = Depth;
	// 开孔直径
	m_Diameter = Diameter;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	//
	CircleCenterList_A = ListA;
	//COORDINATE2 tempSet;
	//get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	//get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	//get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	//get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	//// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	//MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}


CJoint4::~CJoint4(void)
{
}

COORDINATE3 CJoint4::CirclePt( float RadiusInput, float AngleInput )
{
	float y = sin(AngleInput)*RadiusInput;
	float x = cos(AngleInput)*RadiusInput;
	COORDINATE3 res;
	get<0>(res) = x;
	get<1>(res) = y;
	get<2>(res) = 0;
	return res;
}

vector<COORDINATE3> CJoint4::GetPoints( float RadiusInput )
{
	// 输入半径，以π的一半进行分割圆
	vector<COORDINATE3> res;//坐标集合
	//float unit = PI/2;//4个点
	float unit = PI/18; // 36个点
	COORDINATE3 tempPt;
	for (float i=0; i<=2*PI; i = i + unit)
	{
		tempPt = CirclePt(RadiusInput, i);//根据原点和角度计算孔外围点的坐标
		res.push_back(tempPt);
	}
	//vector<AXIS>::iterator itr = res.end();
	//res.erase(itr);//圆取36个会重复
	return res;//总共有36个点
}

void CJoint4::SetCircleDatad_A( void )
{
	//////////////////////////////////////////////////////////////////////////
	// 设置表面数据
	//////////////////////////////////////////////////////////////////////////
	// 设置 中间 矩形数据
	CutCircleData_A[0][0] = 0;
	CutCircleData_A[0][1] = 0;
	CutCircleData_A[0][2] = 0;

	CutCircleData_A[1][0] = m_Width; 
	CutCircleData_A[1][1] = 0; 
	CutCircleData_A[1][2] = 0;

	CutCircleData_A[2][0] = m_Width; 
	CutCircleData_A[2][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; 
	CutCircleData_A[2][2] = 0;

	CutCircleData_A[3][0] =  0; 
	CutCircleData_A[3][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; 
	CutCircleData_A[3][2] = 0;

	// 设置孔数据
	vector<COORDINATE3> Pts36;
	// 计算圆孔周围坐标点的集合
	Pts36 = GetPoints(m_Diameter/2);		// 0 到 36 总共有37个点
	int num = 4;// 因为开始已经加入了4个边框点

	COORDINATE3 tempC, tempD;
	float X_Vec;
	float Y_Vec;
	float Z_Vec;
	float tempF;
	static int xnum, xnum2;
	// 针对每个开孔中心进行位移变化 示例为4个孔
	// 
	for(int j=0; j<CircleCenterList_A.size(); j++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList_A.at(j);
		X_Vec = float(get<0>(X_Y_Vec));
		Y_Vec = float(get<1>(X_Y_Vec));
		//X_Vec = 2;
		//Y_Vec = 2;
		Z_Vec = 0;
		for(int ro=0; ro<Pts36.size(); ro++)
		{
			tempC = Pts36.at(ro);
			xnum = num;
			CutCircleData_A[xnum][0] = get<0>(tempC) + X_Vec;
			CutCircleData_A[xnum][1] = get<1>(tempC) + Y_Vec;
			CutCircleData_A[xnum][2] = get<2>(tempC) + Z_Vec;
			num = num + 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置内部数据
	//////////////////////////////////////////////////////////////////////////
	num = 0;
	// 对每个圆心坐标点进行处理
	for(int i = 0; i<CircleCenterList_A.size(); i++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList_A.at(i);
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
				InCircleData_A[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_A[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_A[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_A[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			// 如果是最后一个点 和第一个点组成矩形
			else if (ro == Pts36.size() - 1)
			{
				tempC = Pts36.at(ro);
				tempD = Pts36.at(0);

				xnum2 = num;
				InCircleData_A[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_A[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_A[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_A[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			num = num + 4;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置外框数据
	// 就是矩形 围边的数据
	//////////////////////////////////////////////////////////////////////////
	OutCircleData_8[0][0] = 0;OutCircleData_8[0][1] = 0;OutCircleData_8[0][2] = 0;
	OutCircleData_8[1][0] = 0;OutCircleData_8[1][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[1][2] = 0;
	OutCircleData_8[2][0] = 0;OutCircleData_8[2][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[2][2] = -m_Depth;
	OutCircleData_8[3][0] = 0;OutCircleData_8[3][1] = 0;OutCircleData_8[3][2] = -m_Depth;

	OutCircleData_8[4][0] = 0;OutCircleData_8[4][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[4][2] = 0;
	OutCircleData_8[5][0] = m_Width;OutCircleData_8[5][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[5][2] = 0;
	OutCircleData_8[6][0] = m_Width;OutCircleData_8[6][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[6][2] = -m_Depth;
	OutCircleData_8[7][0] = 0;OutCircleData_8[7][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[7][2] = -m_Depth;

	OutCircleData_8[8][0] = m_Width;OutCircleData_8[8][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[8][2] = 0;
	OutCircleData_8[9][0] = m_Width;OutCircleData_8[9][1] = 0;OutCircleData_8[9][2] = 0;
	OutCircleData_8[10][0] = m_Width;OutCircleData_8[10][1] = 0;OutCircleData_8[10][2] = -m_Depth;
	OutCircleData_8[11][0] = m_Width;OutCircleData_8[11][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[11][2] = -m_Depth;

	OutCircleData_8[12][0] = m_Width;OutCircleData_8[12][1] = 0;OutCircleData_8[12][2] = 0;
	OutCircleData_8[13][0] = 0;OutCircleData_8[13][1] = 0;OutCircleData_8[13][2] = 0;
	OutCircleData_8[14][0] = 0;OutCircleData_8[14][1] = 0;OutCircleData_8[14][2] = -m_Depth;
	OutCircleData_8[15][0] = m_Width;OutCircleData_8[15][1] = 0;OutCircleData_8[15][2] = -m_Depth;
}

void CJoint4::InitJoint( void )
{
	m_Joint = new SoSeparator;
	//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
	//blueDepth->test.setValue(TRUE);
	//blueDepth->function.setValue(SoDepthBuffer::ALWAYS);
	//m_Bar->addChild(blueDepth);
	// 名称节点
	tempStr = m_Name;// + _T("GROUP");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Joint->setName(tempSbStr);
	// 颜色节点
	SoMaterial * m_Material = new SoMaterial;
	m_Material->diffuseColor.setValue(1,0,0);
	tempStr = m_Name + _T("COLOR");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Material->setName(tempSbStr);

	//m_Color = new SoBaseColor;
	//m_Color->rgb.setValue(1, 1, 1);
	//tempStr = m_Name+_T("COLOR");
	//tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	//tempSbStr.fromWideChar(tempWCHAR);
	//m_Color->setName(tempSbStr);
	// 位移节点
	m_Translation = new SoTranslation;
	m_Translation->translation.setValue(m_Trans);
	tempStr = m_Name+_T("TRANSLATION");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Translation->setName(tempSbStr);
	// 旋转节点
	m_RotationX = new SoRotationXYZ;
	tempStr = m_Name+_T("ROTATEX");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_RotationX->setName(tempSbStr);

	m_RotationY = new SoRotationXYZ;
	tempStr = m_Name+_T("ROTATEY");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_RotationY->setName(tempSbStr);

	m_RotationZ = new SoRotationXYZ;
	tempStr = m_Name+_T("ROTATEZ");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_RotationZ->setName(tempSbStr);

	m_RotationX->axis.setValue(SoRotationXYZ::X);
	m_RotationX->angle.setValue(m_xRot);
	m_RotationY->axis.setValue(SoRotationXYZ::Y);
	m_RotationY->angle.setValue(m_yRot);
	m_RotationZ->axis.setValue(SoRotationXYZ::Z);
	m_RotationZ->angle.setValue(m_zRot);

	SoGroup * m_Rotation = new SoGroup;
	tempStr = m_Name+_T("ROTATION");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Rotation->setName(tempSbStr);
	m_Rotation->addChild(m_RotationX);m_Rotation->addChild(m_RotationY);m_Rotation->addChild(m_RotationZ);
	//  法向节点
	//SoCoordinate3 * ss = new SoCoordinate3;
	//ss->point.set1Value(0,0,0,0);
	// 中心点
	//////////////////////////////////////////////////////////////////////////
	// 测试加入坐标 soswitch 测试
	SoSeparator * Coord = new SoSeparator;
	SoSwitch * CoordSwitch = new SoSwitch;
	SoInput Coordinate;
	if (Coordinate.openFile("axis.iv") == NULL)
	{
		exit(-1);
	}
	SoSeparator *axis = SoDB::readAll(&Coordinate);
	//theApp.m_MainRoot->addChild(axis);
	CoordSwitch->addChild(axis);
	//SO_SWITCH_NONE
	CoordSwitch->whichChild.setValue(0);
	//
	// 绘图模式节点
	//SoDrawStyle * m_DrawStyle = new SoDrawStyle;
	//m_DrawStyle->style.setValue(SoDrawStyle::LINES);

	m_Joint->addChild(m_Translation);
	m_Joint->addChild(m_Rotation);
	m_Joint->addChild(m_Material);
	m_Joint->addChild(CoordSwitch);
	//m_Bar->addChild(m_DrawStyle);
	//m_Bar->addChild(m_Color);
	//m_Bar->addChild(ss);
}

wchar_t* CJoint4::Cstring2Wchar( CString cstr )
{
	//CString origCString("Hello, World!"); 
	wchar_t* wCharString = cstr.GetBuffer(cstr.GetLength()+1); 
	return wCharString;
}

void CJoint4::setHwnd( Hwnd tt )
{
	mainHwnd = tt;
}

SoSeparator * CJoint4::CreateJoint()
{
	//////////////////////////////////////////////////////////////////////////
	//	 新式的布尔运算方法
	InitJoint();
	//////////////////////////////////////////////////////////////////////////
	// 初始化孔的数据， 暂时将孔分为四等分用于测试
	SetCircleDatad_A();
	SetFourCircleList();

	//////////////////////////////////////////////////////////////////////////
	// 设置法向
	SoShapeHints * m_hints = new SoShapeHints;
	m_hints->windingType=SoShapeHints::ODD_TYPE;
	m_hints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	//m_Bar->addChild(m_hints);

	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	myVertexProperty->normalBinding = SoNormalBinding::OVERALL;
	// 每个孔4个点 共8个孔 加外框4个点
	myVertexProperty->vertex.setValues(0, 300,CutCircleData_A);
	static int32_t numvertices[5] = {4,37,37,37,37};
	// 面对向
	SoFaceSet *UpFace = new SoFaceSet;
	// 绘制顶面
	UpFace->numVertices.setValues(0, 5, numvertices);
	UpFace->vertexProperty.setValue(myVertexProperty);
	//m_Bar->addChild(myFaceSet);
	// 绘制底面
	SoFaceSet *myFaceSet2 = new SoFaceSet;
	myFaceSet2 = UpFace;
	SoTranslation * depthTran = new SoTranslation;
	depthTran->translation.setValue(0,0,-m_Depth);
	SoSeparator * DownFace = new SoSeparator;
	DownFace->setName("DownData");
	DownFace->addChild(depthTran);
	DownFace->addChild(m_hints);
	DownFace->addChild(myFaceSet2);

	SoSeparator * UpFaceSep = new SoSeparator;
	UpFaceSep->setName("UpData");
	// 加入上下面分支节点
	//UpDownFace->addChild(m_hints);
	//m_Bar->addChild(m_hints);
	//
	// 上面的
	SoTranslation *UpFaceTra = new SoTranslation;
	UpFaceSep->addChild(UpFaceTra);
	UpFaceSep->addChild(m_hints);
	UpFaceSep->addChild(UpFace);
	m_Joint->addChild(UpFaceSep);

	m_Joint->addChild(DownFace);
	//UpDownFace->addChild(UpFace);
	//UpDownFace->addChild(DownFace);
	//m_Bar->addChild(UpDownFace);
	// 绘制内部
	SoVertexProperty *myVertexPropertyIN = new SoVertexProperty;
	myVertexPropertyIN->normalBinding = SoNormalBinding::PER_FACE;
	//myVertexPropertyIN->vertex.setValues(0, 128, InCircleData_8);//1184
	myVertexPropertyIN->vertex.setValues(0, 592, InCircleData_A);
	static int32_t IN_numvertices[148];
	for (int tt=0; tt<148;tt++)
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
	myIN_FaceSet->numVertices.setValues(0, 148, IN_numvertices);
	myIN_FaceSet->vertexProperty.setValue(myVertexPropertyIN);
	SoSeparator * CylinderFaces = new SoSeparator;
	CylinderFaces->setName("CylinderData");
	SoTranslation * InFaceTra = new SoTranslation;
	CylinderFaces->addChild(InFaceTra);
	SoShapeHints * InHints = new SoShapeHints;
	CylinderFaces->addChild(InHints);
	CylinderFaces->addChild(myIN_FaceSet);
	m_Joint->addChild(CylinderFaces);
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
	m_Joint->addChild(OutSkin);

	CreateSelCircle();

	return m_Joint;
}

void CJoint4::SetFourCircleList()
{
	for (int i = 4; i<41; i++)
	{
		Circle1Pos[i-4][0] = CutCircleData_A[i][0];
		Circle1Pos[i-4][1] = CutCircleData_A[i][1];
		Circle1Pos[i-4][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=41; i<78; i++)
	{
		Circle2Pos[i-41][0] = CutCircleData_A[i][0];
		Circle2Pos[i-41][1] = CutCircleData_A[i][1];
		Circle2Pos[i-41][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=78; i<115; i++)
	{
		Circle3Pos[i-78][0] = CutCircleData_A[i][0];
		Circle3Pos[i-78][1] = CutCircleData_A[i][1];
		Circle3Pos[i-78][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=115; i<152; i++)
	{
		Circle4Pos[i-115][0] = CutCircleData_A[i][0];
		Circle4Pos[i-115][1] = CutCircleData_A[i][1];
		Circle4Pos[i-115][2] = CutCircleData_A[i][2];
	}
}

void CJoint4::CreateSelCircle()
{
	CalculateNegCircle();

	SoSwitch * SelectCircle = new SoSwitch;
	//SelectCircle->whichChild.setValue(SO_SWITCH_ALL);
	SelectCircle->whichChild.setValue(SO_SWITCH_NONE);
	tempStr= m_Name + _T("HOLE_SWITCH");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);

	SelectCircle->setName(tempSbStr);
	//////////////////////////////////////////////////////////////////////////
	// 绘制测试圆
	// 第一个圆
	SoVertexProperty * CircleProperty1 = new SoVertexProperty;
	CircleProperty1->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty1->vertex.setValues(0, 37, Circle1Pos);
	static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace1 = new SoFaceSet;
	CircleFace1->numVertices.setValues(0, 1, testNum);
	CircleFace1->vertexProperty.setValue(CircleProperty1);
	SoSeparator * CircleFaceSep1 = new SoSeparator;
	tempStr= m_Name + _T("C1");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep1->setName(tempSbStr);

	SoMaterial * CircleMaterial1 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL1");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial1->setName(tempSbStr);

	CircleMaterial1->diffuseColor.setValue(0,0,1);
	CircleFaceSep1->addChild(CircleMaterial1);

	SoTranslation * CircleFaceTran1 = new SoTranslation;
	//tempStr= m_Name + _T("CIRCLE_TRANSLATE");
	//tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	//tempSbStr.fromWideChar(tempWCHAR);
	//CircleFaceTran1->setName(tempSbStr);

	CircleFaceSep1->addChild(CircleFaceTran1);
	SoShapeHints * CircleHints1 = new SoShapeHints;
	//tempStr= m_Name + _T("CIRCLE_HINTS");
	//tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	//tempSbStr.fromWideChar(tempWCHAR);
	//CircleHints1->setName(tempSbStr);

	CircleFaceSep1->addChild(CircleHints1);
	CircleFaceSep1->addChild(CircleFace1);
	//
	SoCoordinate3 * CPt1 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP1");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt1->setName(tempSbStr);

	CPt1->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(0)), 
		get<1>(CircleCenterList_A.at(0)),
		0);
	//SbString dd;
	//testC->point.get1(0,dd);
	//const char *ts = dd.getString();
	//CString go;	go = ts;
	//LPCTSTR lpstr = (LPCTSTR)(LPCTSTR)go.GetBuffer();
	//MessageBox(mainHwnd,lpstr,_T("TestOut"),0);
	//testC->point.get(dd);
	CircleFaceSep1->addChild(CPt1);
	//
	SelectCircle->addChild(CircleFaceSep1);
	//////////////////////////////////////////////////////////////////////////
	// 第二个圆
	SoVertexProperty * CircleProperty2 = new SoVertexProperty;
	CircleProperty2->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty2->vertex.setValues(0, 37, Circle2Pos);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace2 = new SoFaceSet;
	CircleFace2->numVertices.setValues(0, 1, testNum);
	CircleFace2->vertexProperty.setValue(CircleProperty2);
	SoSeparator * CircleFaceSep2 = new SoSeparator;
	tempStr= m_Name + _T("C2");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep2->setName(tempSbStr);

	SoMaterial * CircleMaterial2 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL2");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial2->setName(tempSbStr);

	CircleMaterial2->diffuseColor.setValue(0,0,1);
	CircleFaceSep2->addChild(CircleMaterial2);
	SoTranslation * CircleFaceTran2 = new SoTranslation;
	CircleFaceSep2->addChild(CircleFaceTran2);
	SoShapeHints * CircleHints2 = new SoShapeHints;
	CircleFaceSep2->addChild(CircleHints2);
	CircleFaceSep2->addChild(CircleFace2);
	//
	SoCoordinate3 *CPt2 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP2");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt2->setName(tempSbStr);

	CPt2->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(1)), 
		get<1>(CircleCenterList_A.at(1)),
		0);
	CircleFaceSep2->addChild(CPt2);

	SelectCircle->addChild(CircleFaceSep2);
	//////////////////////////////////////////////////////////////////////////
	// 第三个圆
	SoVertexProperty * CircleProperty3 = new SoVertexProperty;
	CircleProperty3->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty3->vertex.setValues(0, 37, Circle3Pos);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace3 = new SoFaceSet;
	CircleFace3->numVertices.setValues(0, 1, testNum);
	CircleFace3->vertexProperty.setValue(CircleProperty3);
	SoSeparator * CircleFaceSep3 = new SoSeparator;
	tempStr= m_Name + _T("C3");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep3->setName(tempSbStr);

	SoMaterial * CircleMaterial3 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL3");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial3->setName(tempSbStr);

	CircleMaterial3->diffuseColor.setValue(0,0,1);
	CircleFaceSep3->addChild(CircleMaterial3);
	SoTranslation * CircleFaceTran3 = new SoTranslation;
	CircleFaceSep3->addChild(CircleFaceTran3);
	SoShapeHints * CircleHints3 = new SoShapeHints;
	CircleFaceSep3->addChild(CircleHints3);
	CircleFaceSep3->addChild(CircleFace3);
	//
	SoCoordinate3 *CPt3 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP3");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt3->setName(tempSbStr);

	CPt3->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(2)), 
		get<1>(CircleCenterList_A.at(2)),
		0);
	CircleFaceSep3->addChild(CPt3);

	SelectCircle->addChild(CircleFaceSep3);
	//////////////////////////////////////////////////////////////////////////
	// 第四个圆
	SoVertexProperty * CircleProperty4 = new SoVertexProperty;
	CircleProperty4->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty4->vertex.setValues(0, 37, Circle4Pos);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace4 = new SoFaceSet;
	CircleFace4->numVertices.setValues(0, 1, testNum);
	CircleFace4->vertexProperty.setValue(CircleProperty4);
	SoSeparator * CircleFaceSep4 = new SoSeparator;
	tempStr= m_Name + _T("C4");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep4->setName(tempSbStr);

	SoMaterial * CircleMaterial4 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL4");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial4->setName(tempSbStr);

	CircleMaterial4->diffuseColor.setValue(0,0,1);
	CircleFaceSep4->addChild(CircleMaterial4);
	SoTranslation * CircleFaceTran4 = new SoTranslation;
	CircleFaceSep4->addChild(CircleFaceTran4);
	SoShapeHints * CircleHints4 = new SoShapeHints;
	CircleFaceSep4->addChild(CircleHints4);
	CircleFaceSep4->addChild(CircleFace4);
	//
	SoCoordinate3 *CPt4 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP4");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt4->setName(tempSbStr);

	CPt4->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(3)), 
		get<1>(CircleCenterList_A.at(3)),
		0);
	CircleFaceSep4->addChild(CPt4);

	SelectCircle->addChild(CircleFaceSep4);
	//
	// 反面的孔
	//////////////////////////////////////////////////////////////////////////
	// 第五个圆
	SoVertexProperty * CircleProperty5 = new SoVertexProperty;
	
	CircleProperty5->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty5->vertex.setValues(0, 37, Circle1Neg);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace5 = new SoFaceSet;
	CircleFace5->numVertices.setValues(0, 1, testNum);
	CircleFace5->vertexProperty.setValue(CircleProperty5);
	SoSeparator * CircleFaceSep5 = new SoSeparator;
	tempStr= m_Name + _T("C5");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep5->setName(tempSbStr);

	SoMaterial * CircleMaterial5 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL5");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial5->setName(tempSbStr);

	CircleMaterial5->diffuseColor.setValue(0,0,1);
	CircleFaceSep5->addChild(CircleMaterial5);
	SoTranslation * CircleFaceTran5 = new SoTranslation;
	CircleFaceSep5->addChild(CircleFaceTran5);
	SoShapeHints * CircleHints5 = new SoShapeHints;
	CircleHints5->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	CircleFaceSep5->addChild(CircleHints5);
	CircleFaceSep5->addChild(CircleFace5);
	//
	SoCoordinate3 *CPt5 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP5");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt5->setName(tempSbStr);

	CPt5->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(0)), 
		get<1>(CircleCenterList_A.at(0)),
		0 - m_Depth);
	CircleFaceSep5->addChild(CPt5);

	SelectCircle->addChild(CircleFaceSep5);
	//////////////////////////////////////////////////////////////////////////
	// 第六个圆
	SoVertexProperty * CircleProperty6 = new SoVertexProperty;

	CircleProperty6->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty6->vertex.setValues(0, 37, Circle2Neg);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace6 = new SoFaceSet;
	CircleFace6->numVertices.setValues(0, 1, testNum);
	CircleFace6->vertexProperty.setValue(CircleProperty6);
	SoSeparator * CircleFaceSep6 = new SoSeparator;
	tempStr= m_Name + _T("C6");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep6->setName(tempSbStr);

	SoMaterial * CircleMaterial6 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL6");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial6->setName(tempSbStr);

	CircleMaterial6->diffuseColor.setValue(0,0,1);
	CircleFaceSep6->addChild(CircleMaterial6);
	SoTranslation * CircleFaceTran6 = new SoTranslation;
	CircleFaceSep6->addChild(CircleFaceTran6);
	SoShapeHints * CircleHints6 = new SoShapeHints;
	CircleHints6->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	CircleFaceSep6->addChild(CircleHints6);
	CircleFaceSep6->addChild(CircleFace6);
	//
	SoCoordinate3 *CPt6 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP6");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt6->setName(tempSbStr);

	CPt6->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(1)), 
		get<1>(CircleCenterList_A.at(1)),
		0 - m_Depth);
	CircleFaceSep6->addChild(CPt6);

	SelectCircle->addChild(CircleFaceSep6);
	//////////////////////////////////////////////////////////////////////////
	// 第七个圆
	SoVertexProperty * CircleProperty7 = new SoVertexProperty;

	CircleProperty7->normalBinding = SoNormalBinding::PER_FACE;
	CircleProperty7->vertex.setValues(0, 37, Circle3Neg);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace7 = new SoFaceSet;
	CircleFace7->numVertices.setValues(0, 1, testNum);
	CircleFace7->vertexProperty.setValue(CircleProperty7);
	SoSeparator * CircleFaceSep7 = new SoSeparator;
	tempStr= m_Name + _T("C7");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep7->setName(tempSbStr);

	SoMaterial * CircleMaterial7 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL7");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial7->setName(tempSbStr);

	CircleMaterial7->diffuseColor.setValue(0,0,1);
	CircleFaceSep7->addChild(CircleMaterial7);
	SoTranslation * CircleFaceTran7 = new SoTranslation;
	CircleFaceSep7->addChild(CircleFaceTran7);
	SoShapeHints * CircleHints7 = new SoShapeHints;
	CircleHints7->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	CircleFaceSep7->addChild(CircleHints7);
	CircleFaceSep7->addChild(CircleFace7);
	//
	SoCoordinate3 *CPt7 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP7");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt7->setName(tempSbStr);

	CPt7->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(2)), 
		get<1>(CircleCenterList_A.at(2)),
		0 - m_Depth);
	CircleFaceSep7->addChild(CPt7);

	SelectCircle->addChild(CircleFaceSep7);
	//////////////////////////////////////////////////////////////////////////
	// 第八个圆
	SoVertexProperty * CircleProprety8 = new SoVertexProperty;

	CircleProprety8->normalBinding = SoNormalBinding::PER_FACE;
	CircleProprety8->vertex.setValues(0, 37, Circle4Neg);
	//static int32_t testNum[1] = {36};
	SoFaceSet * CircleFace8 = new SoFaceSet;
	CircleFace8->numVertices.setValues(0, 1, testNum);
	CircleFace8->vertexProperty.setValue(CircleProprety8);
	SoSeparator * CircleFaceSep8 = new SoSeparator;
	tempStr= m_Name + _T("C8");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleFaceSep8->setName(tempSbStr);

	SoMaterial * CircleMaterial8 = new SoMaterial;
	tempStr= m_Name + _T("CIRCLE_MATERIAL8");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CircleMaterial8->setName(tempSbStr);

	CircleMaterial8->diffuseColor.setValue(0,0,1);
	CircleFaceSep8->addChild(CircleMaterial8);
	SoTranslation * CircleFaceTran8 = new SoTranslation;
	CircleFaceSep8->addChild(CircleFaceTran8);
	SoShapeHints * CircleHints8 = new SoShapeHints;
	CircleHints8->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	CircleFaceSep8->addChild(CircleHints8);
	CircleFaceSep8->addChild(CircleFace8);
	//
	SoCoordinate3 *CPt8 = new SoCoordinate3;
	tempStr= m_Name + _T("CIRCLE_CP8");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CPt8->setName(tempSbStr);

	CPt8->point.set1Value(0, 
		get<0>(CircleCenterList_A.at(3)), 
		get<1>(CircleCenterList_A.at(3)),
		0 - m_Depth);
	CircleFaceSep8->addChild(CPt8);

	SelectCircle->addChild(CircleFaceSep8);
	//////////////////////////////////////////////////////////////////////////
	m_Joint->addChild(SelectCircle);
}

void CJoint4::CalculateNegCircle()
{
	for (int i = 0; i<37; i++)
	{
		Circle1Neg[i][0] = Circle1Pos[i][0];
		Circle1Neg[i][1] = Circle1Pos[i][1];
		Circle1Neg[i][2] = Circle1Pos[i][2] - m_Depth;
	}
	//
	for (int i = 0; i<37; i++)
	{
		Circle2Neg[i][0] = Circle2Pos[i][0];
		Circle2Neg[i][1] = Circle2Pos[i][1];
		Circle2Neg[i][2] = Circle2Pos[i][2] - m_Depth;
	}
	//
	for (int i = 0; i<37; i++)
	{
		Circle3Neg[i][0] = Circle3Pos[i][0];
		Circle3Neg[i][1] = Circle3Pos[i][1];
		Circle3Neg[i][2] = Circle3Pos[i][2] - m_Depth;
	}
	//
	for (int i = 0; i<37; i++)
	{
		Circle4Neg[i][0] = Circle4Pos[i][0];
		Circle4Neg[i][1] = Circle4Pos[i][1];
		Circle4Neg[i][2] = Circle4Pos[i][2] - m_Depth;
	}
}
