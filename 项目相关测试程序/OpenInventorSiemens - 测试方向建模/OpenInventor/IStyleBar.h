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
	void InitIStyleBar(); // 初始化直排数据
	void CreatCube();
	void InitFaceData();

private:
	float FaceData[24][3];
	double m_Height, m_Width, m_Depth;
	// 总节点
	SoSeparator * m_IBar;
	// 临时变量
	SbName m_tempName;
	CString tempStr;
	SbString tempSbStr;
	// 输入数据
	SbVec3d m_Trans;
	CString m_Name;
	SbVec3f m_Straight_Position;
	double m_Diameter;
	SbRotation x_Rotation, y_Rotation, z_Rotation;
	wchar_t* tempWCHAR; 
	vector<COORDINATE2> CircleCenterList;//开孔中心点的容器
	Hwnd mainHwnd;
	// 直连接排位移
	SoTranslation * m_Translation;
	// 直连接排转动
	SoRotationXYZ * m_RotationX;
	SoRotationXYZ * m_RotationY;
	SoRotationXYZ * m_RotationZ;
	float m_xRot, m_yRot, m_zRot;

	// 计算园上点
	vector<COORDINATE2> CircleCenterList_A;// A端开孔中心点的容器
	vector<COORDINATE2> CircleCenterList_B;// B端开孔中心点的容器
	vector<COORDINATE2> CircleCenterList_C;// C端开孔中心点的容器
	float MinAndMax[2];
	float MiddleWidth;
};

