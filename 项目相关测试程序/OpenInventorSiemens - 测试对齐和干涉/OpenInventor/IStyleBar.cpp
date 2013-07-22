#include "StdAfx.h"
#include "IStyleBar.h"
#include "Joint4.h"
#include <Inventor\nodes\SoCube.h>
#include <Inventor\nodes\SoFaceSet.h>
#include <Inventor\nodes\SoShapeHints.h>


CIStyleBar::CIStyleBar(void)
{
	// 位移值
	m_Trans.setValue(0,0,0);
	// 初始化I型链接排
	m_Name = "IBar1";
	// 高度 其实就是长
	m_Height = 30;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}

CIStyleBar::CIStyleBar( CString JName, double Height, 
	double Width, double Depth, double Diameter1,
	double Diameter2,
	SbVec3f Tran, double XRot, double YRot, double ZRot, vector<COORDINATE2> ListA )
{
	// 位移值
	m_Trans.setValue(Tran);
	// 初始化I型链接排
	m_Name = JName;
	// 高度 其实就是长
	m_Height = Height;
	// 宽
	m_Width = Width;
	// 深
	m_Depth = Depth;
	// 开孔直径
	m_Diameter = Diameter1;
	////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}

CIStyleBar::CIStyleBar( CString Name )
{
	// 位移值
	m_Trans.setValue(0,0,0);
	// 初始化I型链接排
	m_Name = Name;
	// 高度 其实就是长
	m_Height = 30;
	// 宽
	m_Width = 5;
	// 深
	m_Depth = 1;
	// 开孔直径
	m_Diameter = 0.7;
	////////////////////////////////////////////////////////////////////////
	// 开孔位置
	COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 14;CircleCenterList_B.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 13;CircleCenterList_B.push_back(tempSet);
	// 设置开孔最小和最大值 这个值是遍历得到的
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}


CIStyleBar::~CIStyleBar(void)
{
}

SoSeparator * CIStyleBar::CreateBar()
{
	InitIStyleBar();
	CreatCube();
	//m_IBar->addChild(new SoCube);
	//SoSeparator * ss = new SoSeparator;
	//ss->addChild(new SoCube);
	return m_IBar;
}

void CIStyleBar::InitIStyleBar()
{
	m_IBar = new SoSeparator;

	// 名称节点
	tempStr = m_Name;// + _T("GROUP");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_IBar->setName(tempSbStr);
	//// 颜色节点
	SoMaterial * m_Material = new SoMaterial;
	m_Material->diffuseColor.setValue(1,0,0);
	tempStr = m_Name + _T("COLOR");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Material->setName(tempSbStr);
	m_IBar->addChild(m_Material);

	// 位移节点
	m_Translation = new SoTranslation;
	m_Translation->translation.setValue(m_Trans);
	tempStr = m_Name+_T("TRANSLATION");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Translation->setName(tempSbStr);
	m_IBar->addChild(m_Translation);

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
	m_IBar->addChild(m_Rotation);

	//// 中心点
	//////////////////////////////////////////////////////////////////////////
	// 测试加入坐标
	SoSeparator * Coord = new SoSeparator;
	SoSwitch * CoordSwitch = new SoSwitch;
	tempStr = m_Name+_T("AXIS");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	CoordSwitch->setName(tempSbStr);

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

	//m_IBar->addChild(m_Translation);
	//m_IBar->addChild(m_Rotation);
	//m_IBar->addChild(m_Material);
	m_IBar->addChild(CoordSwitch);

}

void CIStyleBar::CreatCube()
{
	InitFaceData();
	CString connectA_Name;
	connectA_Name = m_Name + _T("connectA");
	SbVec3f Tran; Tran.setValue(0, 0,0);
	CJoint4 connectA(connectA_Name, m_Width, m_Depth, m_Diameter,
		Tran, 3.14, 3.14, 0, CircleCenterList_A);

	
	CString connectB_Name;
	connectB_Name = m_Name + _T("connectB");
	Tran.setValue(0, m_Height - (MinAndMax[0] + MiddleWidth + m_Diameter/2),0);
	CJoint4 connectB(connectB_Name, m_Width, m_Depth, m_Diameter,
		Tran, 3.14, 3.14, 0, CircleCenterList_A);

	m_IBar->addChild(connectA.CreateJoint());
	m_IBar->addChild(connectB.CreateJoint());
	//m_IBar->addChild(new SoCube);
}

void CIStyleBar::InitFaceData()
{
	FaceData[0][0] = 0; FaceData[0][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[0][2] = 0;
	FaceData[1][0] = m_Width; FaceData[1][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[1][2] = 0;
	FaceData[2][0] = m_Width; FaceData[2][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[2][2] = 0;
	FaceData[3][0] = 0; FaceData[3][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[3][2] = 0;
	//
	FaceData[4][0] = 0; FaceData[4][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[4][2] = - m_Depth;
	FaceData[5][0] = 0; FaceData[5][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[5][2] = -m_Depth;
	FaceData[6][0] = m_Width; FaceData[6][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[6][2] = -m_Depth;
	FaceData[7][0] = m_Width; FaceData[7][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[7][2] = -m_Depth;
	//
	FaceData[8][0] = m_Width; FaceData[8][1] =MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[8][2] = 0;
	FaceData[9][0] = m_Width; FaceData[9][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[9][2] = -m_Depth;
	FaceData[10][0] = m_Width; FaceData[10][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[10][2] = -m_Depth;
	FaceData[11][0] = m_Width; FaceData[11][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[11][2] = 0;
	//
	FaceData[12][0] = 0; FaceData[12][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[12][2] = 0;
	FaceData[13][0] = 0; FaceData[13][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[13][2] = 0;
	FaceData[14][0] = 0; FaceData[14][1] = m_Height-(MinAndMax[0] + MiddleWidth + m_Diameter/2); FaceData[14][2] = -m_Depth;
	FaceData[15][0] = 0; FaceData[15][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; FaceData[15][2] = -m_Depth;

	// 绘制内部
	SoVertexProperty *BodyVertexProperty = new SoVertexProperty;
	BodyVertexProperty->normalBinding = SoNormalBinding::PER_FACE;
	BodyVertexProperty->vertex.setValues(0, 16, FaceData);
	static int32_t numvertices[4] = {4,4,4,4};
	SoFaceSet *IBarBodyFace = new SoFaceSet;
	IBarBodyFace->numVertices.setValues(0, 4, numvertices);
	IBarBodyFace->vertexProperty.setValue(BodyVertexProperty);
	SoSeparator * CylinderFaces = new SoSeparator;
	CylinderFaces->setName("Body");
	SoTranslation * InFaceTra = new SoTranslation;
	CylinderFaces->addChild(InFaceTra);
	SoShapeHints * InHints = new SoShapeHints;
	CylinderFaces->addChild(InHints);
	CylinderFaces->addChild(IBarBodyFace);
	m_IBar->addChild(CylinderFaces);
	//m_Joint->addChild(CylinderFaces);
}
