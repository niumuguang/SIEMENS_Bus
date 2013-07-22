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

	// ����ģ��
	SoGroup * CreateStraightBar();
	//
	SoGroup * CreateBar();
	// ��������
	SoSeparator * BarMaterial();
	// ����ƽ��
	SoSeparator * BarTransfrom();
	// ����԰�ϵ�
	float CutCircleData_8[300][3];
	float InCircleData_8[128][3];
	float OutCircleData_8[16][3];
	AXIS CirclePt(float RadiusInput, float AngleInput);
	vector<AXIS> GetPoints(float RadiusInput);
	void SetCircleDatad(void);
	//////////////////////////////////////////////////////////////////////////

private:
	//////////////////////////////////////////////////////////////////////////
	// ��������
	CString m_Name, m_Material, m_UserData;
	SbVec3f m_Straight_Position;
	double m_Height, m_Width, m_Depth;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	vector<COORDINATE2> CircleCenterList;
	//////////////////////////////////////////////////////////////////////////
	// ֱ�����Ų���
	SoBaseColor * m_Color;
	//SoMaterial m_Material;
	// ֱ������λ��
	SoTranslation * m_Translation;
	// ��������
	SoCSGGroup * m_csgGroup;
	// ֱ������ת��
	SoRotation * m_Rotation;
	// ֱ��������
	SoInfo * m_WhichDirection;
	// ֱ����������
	SoInfo * m_StraightName;
	// �ܽڵ�
	SoGroup * m_Bar;
	// ��ʱ����
	SbName m_tempName;
	CString tempStr;

	SoVertexProperty * m_NormalVec;
	SoNormalBinding * m_NormalBind;
	// ��ͷ1 ����
	SoMaterial * m_First_Head_Material;
	// ��ͷ2 ����
	SoMaterial * m_Second_Head_Material;
	SoCoordinate3 * m_s;
};

