#pragma once
#include "bar.h"


class CIStyleBar :
	public CBar
{
public:
	CIStyleBar(void);
	CIStyleBar(CString JName, double Height, double Width, 
		double Depth, double Diameter1,
		double Diameter2, SbVec3f Tran, 
		double XRot, double YRot, double ZRot, vector<COORDINATE2> ListA);
	CIStyleBar(CString Name);
	~CIStyleBar(void);

	

	SoSeparator * CreateBar();
	void InitIStyleBar(); // ��ʼ��ֱ������
	void CreatCube();
	void InitFaceData();

private:
	float FaceData[24][3];
	double m_Height, m_Width, m_Depth;
	// �ܽڵ�
	SoSeparator * m_IBar;
	// ��ʱ����
	SbName m_tempName;
	CString tempStr;
	SbString tempSbStr;
	// ��������
	SbVec3d m_Trans;
	CString m_Name;
	SbVec3f m_Straight_Position;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	wchar_t* tempWCHAR; 
	vector<COORDINATE2> CircleCenterList;//�������ĵ������
	Hwnd mainHwnd;
	// ֱ������λ��
	SoTranslation * m_Translation;
	// ֱ������ת��
	SoRotationXYZ * m_RotationX;
	SoRotationXYZ * m_RotationY;
	SoRotationXYZ * m_RotationZ;
	float m_xRot, m_yRot, m_zRot;

	// ����԰�ϵ�
	vector<COORDINATE2> CircleCenterList_A;// A�˿������ĵ������
	vector<COORDINATE2> CircleCenterList_B;// B�˿������ĵ������
	vector<COORDINATE2> CircleCenterList_C;// C�˿������ĵ������
	float MinAndMax[2];
	float MiddleWidth;
};

