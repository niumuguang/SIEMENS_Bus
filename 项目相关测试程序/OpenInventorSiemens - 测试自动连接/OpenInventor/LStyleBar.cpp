#include "StdAfx.h"
#include "LStyleBar.h"
#include <Inventor\nodes\SoFaceSet.h>
#include <Inventor\nodes\SoShapeHints.h>
#include <Inventor\nodes\SoCube.h>


CLStyleBar::CLStyleBar(void)
{
	//UpFaceData[0] = 1.0;
	m_Trans.setValue(0,0,0);
	m_Name = "LBar1";
	// 长度1
	m_Height1 = 8.0;
	// 长度2
	m_Height2 = 10.0;
	// 宽
	m_Width = 5.0;
	// 深
	m_Depth = 1.0;
	// 开孔直径
	m_Diameter = 0.7;
	//
	InitUpFace1Data();
	//
	InitUpFace2Data();
	//
	InitDownFace1Data();
	//
	InitDownFace2Data();
	//
	InitSideFaceData();
	//
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // 支持布尔运算
}

CLStyleBar::CLStyleBar( CString barName, SbVec3d trans, 
	float Height1, float Height2, float Depth, float Width, 
	float xRot, float yRot, float zRot )
{
	// 位移
	m_Trans = trans;
	// 初始化I型链接排
	m_Name = barName;
	//
	m_Height1 = Height1; m_Height2 = Height2;
	m_Width = Width;
	m_Depth = Depth;
	// 开孔直径
	m_Diameter = 0.7;
	//
	InitUpFace1Data();
	//
	InitUpFace2Data();
	//
	InitDownFace1Data();
	//
	InitDownFace2Data();
	//
	InitSideFaceData();
	//
	m_xRot = xRot; m_yRot = yRot; m_zRot = zRot;
	SoSolidViz::init(); // 支持布尔运算
}

CLStyleBar::~CLStyleBar(void)
{
}

SoSeparator * CLStyleBar::CreateBar()
{
	InitIStyleBar();
	CreatUpFace1();
	CreatUpFace2();
	CreatDownFace1();
	CreatDownFace2();
	CreatSideFace();
// 	float Uptest[3][3];
// 	Uptest[0][0] = 1;
// 	UpFaceData[0][0] = 0;	
// 	UpFaceData[0][1] = 1;	
// 	UpFaceData[0][2] = 0;
	
	//printf("ddd");
	//SoSolidViz::init(); // 支持布尔运算

	return m_Bar;
}

void CLStyleBar::CreatUpFace1()
{
 	SoVertexProperty *myVertexPropertyInUpFace = new SoVertexProperty;
 	myVertexPropertyInUpFace->normalBinding = SoNormalBinding::PER_FACE;
 	myVertexPropertyInUpFace->vertex.setValues(0, 4, LStyleUpFaceData1);
 	static int32_t numvertices[1] = {4};
// 	// 面对向
 	SoFaceSet *UpFace = new SoFaceSet;
	UpFace->numVertices.setValues(0, 1, numvertices);
	UpFace->vertexProperty.setValue(myVertexPropertyInUpFace);
	SoSeparator * UpFaceSep = new SoSeparator;
	UpFaceSep->setName("UpData1");
	SoTranslation * UpFaceTra = new SoTranslation;
	SoShapeHints * UpHints = new SoShapeHints;
	UpHints->windingType=SoShapeHints::ODD_TYPE;
	UpHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	UpFaceSep->addChild(UpFaceTra);
	UpFaceSep->addChild(UpHints);
	UpFaceSep->addChild(UpFace);
	m_Bar->addChild(UpFaceSep);
	//m_Bar->addChild(new SoCube);
}

void CLStyleBar::InitIStyleBar( void )
{
	m_Bar = new SoSeparator;
	//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
	//blueDepth->test.setValue(TRUE);
	//blueDepth->function.setValue(SoDepthBuffer::ALWAYS);
	//m_Bar->addChild(blueDepth);
	// 名称节点
	tempStr = m_Name;// + _T("GROUP");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Bar->setName(tempSbStr);
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
	m_RotationY = new SoRotationXYZ;
	m_RotationZ = new SoRotationXYZ;
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
	// 测试加入坐标
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

	m_Bar->addChild(m_Translation);
	m_Bar->addChild(m_Rotation);
	m_Bar->addChild(m_Material);
	m_Bar->addChild(CoordSwitch);
	//m_Bar->addChild(m_Color);
	//m_Bar->addChild(ss);
}

void CLStyleBar::CreatUpFace2()
{
	SoVertexProperty *myVertexPropertyInUpFace = new SoVertexProperty;
	myVertexPropertyInUpFace->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyInUpFace->vertex.setValues(0, 4, LStyleUpFaceData2);
	static int32_t numvertices[1] = {4};
	// 	// 面对向
	SoFaceSet *UpFace = new SoFaceSet;
	UpFace->numVertices.setValues(0, 1, numvertices);
	UpFace->vertexProperty.setValue(myVertexPropertyInUpFace);
	SoSeparator * UpFaceSep = new SoSeparator;
	UpFaceSep->setName("UpData2");
	SoTranslation * UpFaceTra = new SoTranslation;
	SoShapeHints * UpHints = new SoShapeHints;
	UpHints->windingType=SoShapeHints::ODD_TYPE;
	UpHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	UpFaceSep->addChild(UpFaceTra);
	UpFaceSep->addChild(UpHints);
	UpFaceSep->addChild(UpFace);
	m_Bar->addChild(UpFaceSep);
	//m_Bar->addChild(new SoCube);
}

void CLStyleBar::CreatDownFace1()
{
	SoVertexProperty *myVertexPropertyInUpFace = new SoVertexProperty;
	myVertexPropertyInUpFace->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyInUpFace->vertex.setValues(0, 4, LStyleDownFaceData1);
	static int32_t numvertices[1] = {4};
	// 	// 面对向
	SoFaceSet *DownFace = new SoFaceSet;
	DownFace->numVertices.setValues(0, 1, numvertices);
	DownFace->vertexProperty.setValue(myVertexPropertyInUpFace);
	SoSeparator * DownFaceSep = new SoSeparator;
	DownFaceSep->setName("DownData1");
	SoTranslation * DownFaceTra = new SoTranslation;
	SoShapeHints * UpHints = new SoShapeHints;
	//UpHints->windingType=SoShapeHints::ODD_TYPE;
	//UpHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	DownFaceSep->addChild(DownFaceTra);
	DownFaceSep->addChild(UpHints);
	DownFaceSep->addChild(DownFace);
	m_Bar->addChild(DownFaceSep);
	//m_Bar->addChild(new SoCube);
}

void CLStyleBar::CreatDownFace2()
{
	SoVertexProperty *myVertexPropertyInUpFace = new SoVertexProperty;
	myVertexPropertyInUpFace->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyInUpFace->vertex.setValues(0, 4, LStyleDownFaceData2);
	static int32_t numvertices[1] = {4};
	// 	// 面对向
	SoFaceSet *DownFace = new SoFaceSet;
	DownFace->numVertices.setValues(0, 1, numvertices);
	DownFace->vertexProperty.setValue(myVertexPropertyInUpFace);
	SoSeparator * DownFaceSep = new SoSeparator;
	DownFaceSep->setName("DownData2");
	SoTranslation * DownFaceTra = new SoTranslation;
	SoShapeHints * UpHints = new SoShapeHints;
	//UpHints->windingType=SoShapeHints::ODD_TYPE;
	//UpHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	DownFaceSep->addChild(DownFaceTra);
	DownFaceSep->addChild(UpHints);
	DownFaceSep->addChild(DownFace);
	m_Bar->addChild(DownFaceSep);
	//m_Bar->addChild(new SoCube);
}

void CLStyleBar::InitUpFace1Data()
{
	LStyleUpFaceData1[0][0] = 0.00;	
	LStyleUpFaceData1[0][1] = 0.00;	
	LStyleUpFaceData1[0][2] = 0.00;

	LStyleUpFaceData1[1][0] = m_Width;
	LStyleUpFaceData1[1][1] = 0.00;
	LStyleUpFaceData1[1][2] = 0.00;

	LStyleUpFaceData1[2][0] = m_Width;
	LStyleUpFaceData1[2][1] = m_Height1;
	LStyleUpFaceData1[2][2] = 0;

	LStyleUpFaceData1[3][0] = 0;
	LStyleUpFaceData1[3][1] = m_Height1;
	LStyleUpFaceData1[3][2] = -0;
}

void CLStyleBar::CreatSideFace()
{
	SoVertexProperty *myVertexPropertyInUpFace = new SoVertexProperty;
	myVertexPropertyInUpFace->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyInUpFace->vertex.setValues(0, 24, LStyleSideFaceData);
	static int32_t numvertices[6] = {4,4,4,4,4,4};
	// 	// 面对向
	SoFaceSet *SideFace = new SoFaceSet;
	SideFace->numVertices.setValues(0, 6, numvertices);
	SideFace->vertexProperty.setValue(myVertexPropertyInUpFace);
	SoSeparator * SideFaceSep = new SoSeparator;
	SideFaceSep->setName("SideData");
	SoTranslation * SideFaceTra = new SoTranslation;
	SoShapeHints * UpHints = new SoShapeHints;
	//UpHints->windingType=SoShapeHints::ODD_TYPE;
	//UpHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	SideFaceSep->addChild(SideFaceTra);
	SideFaceSep->addChild(UpHints);
	SideFaceSep->addChild(SideFace);
	m_Bar->addChild(SideFaceSep);
}

void CLStyleBar::InitUpFace2Data()
{
	LStyleUpFaceData2[0][0] = m_Width;
	LStyleUpFaceData2[0][1] = m_Height1;
	LStyleUpFaceData2[0][2] = 0;

	LStyleUpFaceData2[1][0] = m_Width;
	LStyleUpFaceData2[1][1] = m_Height1;
	LStyleUpFaceData2[1][2] = -m_Height2;

	LStyleUpFaceData2[2][0] = 0;
	LStyleUpFaceData2[2][1] = m_Height1;
	LStyleUpFaceData2[2][2] = -m_Height2;

	LStyleUpFaceData2[3][0] = 0;
	LStyleUpFaceData2[3][1] = m_Height1;
	LStyleUpFaceData2[3][2] = 0;
}

void CLStyleBar::InitDownFace1Data()
{
	//
	LStyleDownFaceData1[0][0] = 0;
	LStyleDownFaceData1[0][1] = 0;
	LStyleDownFaceData1[0][2] = m_Depth;

	LStyleDownFaceData1[1][0] = m_Width;
	LStyleDownFaceData1[1][1] = 0;
	LStyleDownFaceData1[1][2] = m_Depth;

	LStyleDownFaceData1[2][0] = m_Width;
	LStyleDownFaceData1[2][1] = m_Height1 + m_Depth;
	LStyleDownFaceData1[2][2] = m_Depth;

	LStyleDownFaceData1[3][0] = 0;
	LStyleDownFaceData1[3][1] = m_Height1 + m_Depth;
	LStyleDownFaceData1[3][2] = m_Depth;
}

void CLStyleBar::InitDownFace2Data()
{
	LStyleDownFaceData2[0][0] = m_Width;
	LStyleDownFaceData2[0][1] = m_Height1 + m_Depth;
	LStyleDownFaceData2[0][2] = m_Depth;

	LStyleDownFaceData2[1][0] = m_Width;
	LStyleDownFaceData2[1][1] = m_Height1 + m_Depth;
	LStyleDownFaceData2[1][2] = -m_Height2;

	LStyleDownFaceData2[2][0] = 0;
	LStyleDownFaceData2[2][1] = m_Height1 + m_Depth;
	LStyleDownFaceData2[2][2] = -m_Height2;

	LStyleDownFaceData2[3][0] = 0;
	LStyleDownFaceData2[3][1] = m_Height1 + m_Depth;
	LStyleDownFaceData2[3][2] = m_Depth;
}

void CLStyleBar::InitSideFaceData()
{
	LStyleSideFaceData[0][0] = 0;
	LStyleSideFaceData[0][1] = 0;
	LStyleSideFaceData[0][2] = 0;

	LStyleSideFaceData[1][0] = m_Width;
	LStyleSideFaceData[1][1] = 0;
	LStyleSideFaceData[1][2] = 0;

	LStyleSideFaceData[2][0] = m_Width;
	LStyleSideFaceData[2][1] = 0;
	LStyleSideFaceData[2][2] = m_Depth;

	LStyleSideFaceData[3][0] = 0;
	LStyleSideFaceData[3][1] = 0;
	LStyleSideFaceData[3][2] = m_Depth;
	//
	LStyleSideFaceData[4][0] = m_Width;
	LStyleSideFaceData[4][1] = 0;
	LStyleSideFaceData[4][2] = 0;

	LStyleSideFaceData[5][0] = m_Width;
	LStyleSideFaceData[5][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[5][2] = 0;

	LStyleSideFaceData[6][0] = m_Width;
	LStyleSideFaceData[6][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[6][2] = m_Depth;

	LStyleSideFaceData[7][0] = m_Width;
	LStyleSideFaceData[7][1] = 0;
	LStyleSideFaceData[7][2] = m_Depth;
	//
	LStyleSideFaceData[8][0] = m_Width;
	LStyleSideFaceData[8][1] = m_Height1;
	LStyleSideFaceData[8][2] = 0;

	LStyleSideFaceData[9][0] = m_Width;
	LStyleSideFaceData[9][1] = m_Height1;
	LStyleSideFaceData[9][2] = -m_Height2;

	LStyleSideFaceData[10][0] = m_Width;
	LStyleSideFaceData[10][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[10][2] = -m_Height2;

	LStyleSideFaceData[11][0] = m_Width;
	LStyleSideFaceData[11][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[11][2] = 0;
	//
	LStyleSideFaceData[12][0] = m_Width;
	LStyleSideFaceData[12][1] = m_Height1;
	LStyleSideFaceData[12][2] = -m_Height2;

	LStyleSideFaceData[13][0] = 0;
	LStyleSideFaceData[13][1] = m_Height1;
	LStyleSideFaceData[13][2] = -m_Height2;

	LStyleSideFaceData[14][0] = 0;
	LStyleSideFaceData[14][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[14][2] = -m_Height2;

	LStyleSideFaceData[15][0] = m_Width;
	LStyleSideFaceData[15][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[15][2] = -m_Height2;
	//
	LStyleSideFaceData[16][0] = 0;
	LStyleSideFaceData[16][1] = m_Height1;
	LStyleSideFaceData[16][2] = -m_Height2;

	LStyleSideFaceData[17][0] = 0;
	LStyleSideFaceData[17][1] = m_Height1;
	LStyleSideFaceData[17][2] = 0;

	LStyleSideFaceData[18][0] = 0;
	LStyleSideFaceData[18][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[18][2] = 0;

	LStyleSideFaceData[19][0] = 0;
	LStyleSideFaceData[19][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[19][2] = -m_Height2;
	//
	LStyleSideFaceData[20][0] = 0;
	LStyleSideFaceData[20][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[20][2] = 0;

	LStyleSideFaceData[21][0] = 0;
	LStyleSideFaceData[21][1] = 0;
	LStyleSideFaceData[21][2] = 0;

	LStyleSideFaceData[22][0] = 0;
	LStyleSideFaceData[22][1] = 0;
	LStyleSideFaceData[22][2] = m_Depth;

	LStyleSideFaceData[23][0] = 0;
	LStyleSideFaceData[23][1] = m_Height1 + m_Depth;
	LStyleSideFaceData[23][2] = m_Depth;
}
