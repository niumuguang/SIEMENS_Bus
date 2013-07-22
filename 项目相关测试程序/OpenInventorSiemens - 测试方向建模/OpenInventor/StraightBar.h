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

	// ����ģ��
	SoGroup * CreateStraightBar(); // ����
	SoSeparator * CreateBar();
	AXIS CirclePt(float RadiusInput, float AngleInput);//���ݰ뾶�Ƕȼ���Բ�ϵ�
	vector<AXIS> GetPoints(float RadiusInput);//�õ��ȷ�Բ����
	void SetCircleDatad(void);//�����������
	void InitIStyleBar(void); // ��ʼ��ֱ������
	void InitCoord(void);
	wchar_t* Cstring2Wchar(CString cstr);// Cstring ���� ת���� wchar_t
	void setHwnd(Hwnd tt);
	// ����԰�ϵ�
	float CutCircleData_8[300][3];	// ���±�������
	//float InCircleData_8[128][3]; // �ڲ�Բ��������
	//float InCircleData_8[432][3]; // �ڲ�Բ��������
	float InCircleData_8[1184][3]; // �ڲ�Բ��������
	float OutCircleData_8[16][3];	// �������
	float MinAndMax[2];
	float MiddleWidth;
	//////////////////////////////////////////////////////////////////////////

private:
	//////////////////////////////////////////////////////////////////////////
	// ��������
	SbVec3d m_Trans;
	CString m_Name;
	SbVec3f m_Straight_Position;
	double m_Height, m_Width, m_Depth;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	wchar_t* tempWCHAR; 
	vector<COORDINATE2> CircleCenterList;//�������ĵ������
	Hwnd mainHwnd;
	//////////////////////////////////////////////////////////////////////////
	// ֱ�����Ų���
	SoBaseColor * m_Color;
	//SoMaterial m_Material;
	// ֱ������λ��
	SoTranslation * m_Translation;
	// ��������
	SoCSGGroup * m_csgGroup;
	// ֱ������ת��
	SoRotationXYZ * m_RotationX;
	SoRotationXYZ * m_RotationY;
	SoRotationXYZ * m_RotationZ;
	float m_xRot, m_yRot, m_zRot;

	// ֱ��������
	SoInfo * m_WhichDirection;
	// ֱ����������
	SoInfo * m_StraightName;
	// �ܽڵ�
	SoSeparator * m_Bar;
	// ��ʱ����
	SbName m_tempName;
	CString tempStr;
	SbString tempSbStr;
	//SoVertexProperty * m_NormalVec;
	SoNormalBinding * m_NormalBind;
	SoNormal * m_NormalVec;
	//// ��ͷ1 ����
	//SoMaterial * m_First_Head_Material;
	//// ��ͷ2 ����
	//SoMaterial * m_Second_Head_Material;
	//SoCoordinate3 * m_s;
};

