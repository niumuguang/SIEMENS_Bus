#pragma once
#include "joint.h"
#ifndef PI
#define  PI 3.1415926535897932384626433832795
#endif

#include "stdafx.h"
#include <Inventor/Win/SoWinDef.h>
//#include <OpenInventor.h>
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
#include <Inventor\nodes\SoDrawStyle.h>

#include <math.h>
#include <tuple>
#include <vector>
using namespace std;
typedef tuple<float, float, float> COORDINATE3;

//#ifndef COOR
typedef tuple<float, float> COORDINATE2;
//#endif

class CJoint4 :
	public CJoint
{
public:
	CJoint4(void);
	CJoint4(CString JName, double Width, 
		double Depth, double Diameter, SbVec3f Tran, 
		double XRot, double YRot, double ZRot, vector<COORDINATE2> ListA);
	~CJoint4(void);

	// 创建模型
	SoSeparator * CreateJoint();
	COORDINATE3 CirclePt(float RadiusInput, float AngleInput);//根据半径角度计算圆上点
	vector<COORDINATE3> GetPoints(float RadiusInput);//得到等分圆数据
	void SetCircleDatad_A(void);//处理表面数据

	void InitJoint(void); // 初始化直排数据
	void InitCoord(void);
	wchar_t* Cstring2Wchar(CString cstr);// Cstring 类型 转换成 wchar_t
	void setHwnd(Hwnd tt);
	void SetFourCircleList();
	void CreateSelCircle();// 建立选择圆
	void CalculateNegCircle();

	// 计算园上点
	float CutCircleData_A[152][3];	// 上下表面数据
	//float InCircleData_8[128][3]; // 内部圆柱体数据
	//float InCircleData_8[432][3]; // 内部圆柱体数据
	float InCircleData_A[592][3]; // 内部圆柱体数据
	float OutCircleData_8[16][3];	// 外框数据
	float MinAndMax[2];
	float MiddleWidth;

	float Circle1Pos[37][3], Circle1Neg[37][3];
	float Circle2Pos[37][3], Circle2Neg[37][3];
	float Circle3Pos[37][3], Circle3Neg[37][3];
	float Circle4Pos[37][3], Circle4Neg[37][3];
	//////////////////////////////////////////////////////////////////////////
	// 输入数据


private:
	SbVec3d m_Trans;
	CString m_Name, m_Material, m_UserData;
	SbVec3f m_Straight_Position;
	double m_Height, m_Width, m_Depth;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	wchar_t* tempWCHAR; 
	vector<COORDINATE2> CircleCenterList_A;// A端开孔中心点的容器
	vector<COORDINATE2> CircleCenterList_B;// B端开孔中心点的容器
	vector<COORDINATE2> CircleCenterList_C;// C端开孔中心点的容器
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
	SoSeparator * m_Joint;
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

