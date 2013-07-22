#pragma once

#ifndef PI
#define  PI 3.1415926535897932384626433832795
#endif



#include "bar.h"
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
	~CStraightBar();

	// 创建模型
	SoGroup * CreateStraightBar();
	//
	SoGroup * CreateBar();
	// 创建材质
	SoSeparator * BarMaterial();
	// 创建平移
	SoSeparator * BarTransfrom();
	// 计算园上点
	float CutCircleData_8[300][3];
	float InCircleData_8[128][3];
	float OutCircleData_8[16][3];
	AXIS CirclePt(float RadiusInput, float AngleInput);
	vector<AXIS> GetPoints(float RadiusInput);
	void SetCircleDatad(void);
	//////////////////////////////////////////////////////////////////////////

private:
	//////////////////////////////////////////////////////////////////////////
	// 输入数据
	CString m_Name, m_Material, m_UserData;
	SbVec3f m_Straight_Position;
	double m_Height, m_Width, m_Depth;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	vector<COORDINATE2> CircleCenterList;
	//////////////////////////////////////////////////////////////////////////
	// 直连结排材料
	SoBaseColor * m_Color;
	//SoMaterial m_Material;
	// 直连接排位移
	SoTranslation * m_Translation;
	// 布尔运算
	SoCSGGroup * m_csgGroup;
	// 直连接排转动
	SoRotation * m_Rotation;
	// 直连接排向
	SoInfo * m_WhichDirection;
	// 直连结排名称
	SoInfo * m_StraightName;
	// 总节点
	SoGroup * m_Bar;
	// 临时变量
	SbName m_tempName;
	CString tempStr;

	SoVertexProperty * m_NormalVec;
	SoNormalBinding * m_NormalBind;
	// 接头1 材质
	SoMaterial * m_First_Head_Material;
	// 接头2 材质
	SoMaterial * m_Second_Head_Material;
	SoCoordinate3 * m_s;
};

