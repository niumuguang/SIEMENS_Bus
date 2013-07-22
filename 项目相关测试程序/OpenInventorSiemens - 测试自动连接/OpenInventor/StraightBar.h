#pragma once

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
//#endif


class CStraightBar :public CBar
{
public:
	CStraightBar(void);
	CStraightBar(CString barName, SbVec3d trans);
	CStraightBar(CString barName, SbVec3d trans, float xRot, float yRot, float zRot);
	~CStraightBar();
	//SoMaterial * m_Material;

	// 创建模型
	SoGroup * CreateStraightBar(); // 废弃
	SoSeparator * CreateBar();
	AXIS CirclePt(float RadiusInput, float AngleInput);//根据半径角度计算圆上点
	vector<AXIS> GetPoints(float RadiusInput);//得到等分圆数据
	void SetCircleDatad(void);//处理表面数据
	void InitIStyleBar(void); // 初始化直排数据
	void InitCoord(void);
	wchar_t* Cstring2Wchar(CString cstr);// Cstring 类型 转换成 wchar_t
	void setHwnd(Hwnd tt);
	// 计算园上点
	float CutCircleData_8[300][3];	// 上下表面数据
	//float InCircleData_8[128][3]; // 内部圆柱体数据
	//float InCircleData_8[432][3]; // 内部圆柱体数据
	float InCircleData_8[1184][3]; // 内部圆柱体数据
	float OutCircleData_8[16][3];	// 外框数据
	float MinAndMax[2];
	float MiddleWidth;
	//////////////////////////////////////////////////////////////////////////

private:
	//////////////////////////////////////////////////////////////////////////
	// 输入数据
	SbVec3d m_Trans;
	CString m_Name;
	SbVec3f m_Straight_Position;
	double m_Height, m_Width, m_Depth;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	wchar_t* tempWCHAR; 
	vector<COORDINATE2> CircleCenterList;//开孔中心点的容器
	Hwnd mainHwnd;
	//////////////////////////////////////////////////////////////////////////
	// 直连结排材料
	SoBaseColor * m_Color;
	//SoMaterial m_Material;
	// 直连接排位移
	SoTranslation * m_Translation;
	// 布尔运算
	SoCSGGroup * m_csgGroup;
	// 直连接排转动
	SoRotationXYZ * m_RotationX;
	SoRotationXYZ * m_RotationY;
	SoRotationXYZ * m_RotationZ;
	float m_xRot, m_yRot, m_zRot;

	// 直连接排相
	SoInfo * m_WhichDirection;
	// 直连结排名称
	SoInfo * m_StraightName;
	// 总节点
	SoSeparator * m_Bar;
	// 临时变量
	SbName m_tempName;
	CString tempStr;
	SbString tempSbStr;
	//SoVertexProperty * m_NormalVec;
	SoNormalBinding * m_NormalBind;
	SoNormal * m_NormalVec;
	//// 接头1 材质
	//SoMaterial * m_First_Head_Material;
	//// 接头2 材质
	//SoMaterial * m_Second_Head_Material;
	//SoCoordinate3 * m_s;
};

