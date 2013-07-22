#pragma once
#include "bar.h"

#ifndef PI
#define  PI 3.1415926535897932384626433832795
#endif

#include "bar.h"
#include "stdafx.h"
#include <Inventor/Win/SoWinDef.h>
#include <Inventor\nodes\SoSeparator.h>
#include <Inventor\nodes\SoMaterial.h>
#include <Inventor\nodes\SoTranslation.h>
#include <Inventor\nodes\SoInfo.h>
#include <Inventor\nodes\SoVertexProperty.h>
#include <Inventor\nodes\SoCoordinate3.h>
#include <Inventor\nodes\SoBaseColor.h>
#include <Inventor\nodes\SoRotationXYZ.h>
#include <Inventor\nodes\SoRotation.h>
#include <SolidViz\nodes\SoCSGGroup.h>
#include <Inventor\nodes\SoIndexedFaceSet.h>
#include <Inventor\nodes\SoNormal.h>
#include <Inventor\nodes\SoSwitch.h>

#include <math.h>
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<float, float, float> AXIS;

//#ifndef COOR
typedef tuple<float, float> COORDINATE2;
typedef tuple<float, float, float> COORDINATE3;
//#endif

class CLStyleBar :
	public CBar
{
public:
	CLStyleBar(void);
	CLStyleBar(CString barName, SbVec3d trans, float Height1, float Height2, float Depth, float Width, float xRot, float yRot, float zRot);
	~CLStyleBar(void);

	void InitIStyleBar(void); // 初始化直排数据
	SoSeparator * CreateBar();
	void CreatUpFace1();
	void InitUpFace1Data();
	void CreatUpFace2();
	void InitUpFace2Data();
	void CreatDownFace1();
	void InitDownFace1Data();
	void CreatDownFace2();
	void InitDownFace2Data();
	void CreatSideFace();
	void InitSideFaceData();

	float LStyleUpFaceData1[4][3];
	float LStyleUpFaceData2[4][3];
	float LStyleDownFaceData1[4][3];
	float LStyleDownFaceData2[4][3];
	//
	float LStyleSideFaceData[24][3];

public:
	CString m_Name;
	SoTranslation * m_Translation;
	double m_Height1, m_Height2,  m_Width, m_Depth;
	Hwnd mainHwnd;
	// 直连接排转动
	SoRotationXYZ * m_RotationX;
	SoRotationXYZ * m_RotationY;
	SoRotationXYZ * m_RotationZ;
	float m_xRot, m_yRot, m_zRot;
	// 总节点
	SoSeparator * m_Bar;
	// 临时变量
	SbName m_tempName;
	CString tempStr;
	SbString tempSbStr;
	// 输入数据
	SbVec3d m_Trans;
	double m_Diameter;
	wchar_t* tempWCHAR; 
};

