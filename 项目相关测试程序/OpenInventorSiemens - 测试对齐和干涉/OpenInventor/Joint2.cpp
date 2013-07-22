#include "StdAfx.h"
#include "Joint2.h"
#include <Inventor\nodes\SoShapeHints.h>
#include <Inventor\nodes\SoFaceSet.h>


CJoint2::CJoint2(void)
{
	// λ��ֵ
	m_Trans.setValue(0,0,0);
	// ��ʼ��I��������
	m_Name = "CONN1";
	// �߶� ��ʵ���ǳ�
	m_Height = 15;
	// ��
	m_Width = 5;
	// ��
	m_Depth = 1;
	// ����ֱ��
	m_Diameter = 0.7;
	//////////////////////////////////////////////////////////////////////////
	// ����λ��
	//COORDINATE2 tempSet;
	get<0>(tempSet) = 1;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 1;CircleCenterList_A.push_back(tempSet);

	get<0>(tempSet) = 1;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);
	get<0>(tempSet) = 4;get<1>(tempSet) = 2;CircleCenterList_A.push_back(tempSet);

	// ���ÿ�����С�����ֵ ���ֵ�Ǳ����õ���
	MinAndMax[0] = 2;
	MinAndMax[1] = 13;
	MiddleWidth = 1 - m_Diameter/2;
	m_xRot = 0; m_yRot = 0; m_zRot = 0;
	SoSolidViz::init(); // ֧�ֲ�������
}


CJoint2::~CJoint2(void)
{
}

COORDINATE3 CJoint2::CirclePt( float RadiusInput, float AngleInput )
{
	float y = sin(AngleInput)*RadiusInput;
	float x = cos(AngleInput)*RadiusInput;
	COORDINATE3 res;
	get<0>(res) = x;
	get<1>(res) = y;
	get<2>(res) = 0;
	return res;
}

vector<COORDINATE3> CJoint2::GetPoints( float RadiusInput )
{
	// ����뾶���Ԧе�һ����зָ�Բ
	vector<COORDINATE3> res;//���꼯��
	//float unit = PI/2;//4����
	float unit = PI/18; // 36����
	COORDINATE3 tempPt;
	for (float i=0; i<=2*PI; i = i + unit)
	{
		tempPt = CirclePt(RadiusInput, i);//����ԭ��ͽǶȼ������Χ�������
		res.push_back(tempPt);
	}
	//vector<AXIS>::iterator itr = res.end();
	//res.erase(itr);//Բȡ36�����ظ�
	return res;//�ܹ���36����
}

void CJoint2::SetCircleDatad_A( void )
{
	//////////////////////////////////////////////////////////////////////////
	// ���ñ�������
	//////////////////////////////////////////////////////////////////////////
	// ���� �м� ��������
	CutCircleData_A[0][0] = 0;
	CutCircleData_A[0][1] = 0;
	CutCircleData_A[0][2] = 0;

	CutCircleData_A[1][0] = m_Width; 
	CutCircleData_A[1][1] = 0; 
	CutCircleData_A[1][2] = 0;

	CutCircleData_A[2][0] = m_Width; 
	CutCircleData_A[2][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; 
	CutCircleData_A[2][2] = 0;

	CutCircleData_A[3][0] =  0; 
	CutCircleData_A[3][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2; 
	CutCircleData_A[3][2] = 0;

	// ���ÿ�����
	vector<COORDINATE3> Pts36;
	// ����Բ����Χ�����ļ���
	Pts36 = GetPoints(m_Diameter/2);		// 0 �� 36 �ܹ���37����
	int num = 4;// ��Ϊ��ʼ�Ѿ�������4���߿��

	COORDINATE3 tempC, tempD;
	float X_Vec;
	float Y_Vec;
	float Z_Vec;
	float tempF;
	static int xnum, xnum2;
	// ���ÿ���������Ľ���λ�Ʊ仯 ʾ��Ϊ4����
	// 
	for(int j=0; j<CircleCenterList_A.size(); j++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList_A.at(j);
		X_Vec = float(get<0>(X_Y_Vec));
		Y_Vec = float(get<1>(X_Y_Vec));
		//X_Vec = 2;
		//Y_Vec = 2;
		Z_Vec = 0;
		for(int ro=0; ro<Pts36.size(); ro++)
		{
			tempC = Pts36.at(ro);
			xnum = num;
			CutCircleData_A[xnum][0] = get<0>(tempC) + X_Vec;
			CutCircleData_A[xnum][1] = get<1>(tempC) + Y_Vec;
			CutCircleData_A[xnum][2] = get<2>(tempC) + Z_Vec;
			num = num + 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// �����ڲ�����
	//////////////////////////////////////////////////////////////////////////
	num = 0;
	// ��ÿ��Բ���������д���
	for(int i = 0; i<CircleCenterList_A.size(); i++)
	{
		COORDINATE2 X_Y_Vec = CircleCenterList_A.at(i);
		X_Vec = float(get<0>(X_Y_Vec));//����x����
		Y_Vec = float(get<1>(X_Y_Vec));//����y����
		Z_Vec = 0;// ��Ϊ��ƽ������Z����Ϊ��
		for(int ro=0; ro<Pts36.size(); ro++)
		{
			if (ro < Pts36.size()-1)
			{
				tempC = Pts36.at(ro);
				tempD = Pts36.at(ro+1);

				xnum2 = num;
				// �ֱ����� ��Բס���ĸ���  ��Ҫ����ΪԲ�Ļ�����Ҫ����ĵ�
				InCircleData_A[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_A[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_A[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_A[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			// ��������һ���� �͵�һ������ɾ���
			else if (ro == Pts36.size() - 1)
			{
				tempC = Pts36.at(ro);
				tempD = Pts36.at(0);

				xnum2 = num;
				InCircleData_A[xnum2][0] = get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2][2] = get<2>(tempC) + Z_Vec;

				InCircleData_A[xnum2+1][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+1][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+1][2] = get<2>(tempD)+Z_Vec;

				InCircleData_A[xnum2+2][0] = get<0>(tempD)+X_Vec;
				InCircleData_A[xnum2+2][1] = get<1>(tempD)+Y_Vec;
				InCircleData_A[xnum2+2][2] = get<2>(tempD)+Z_Vec-m_Depth;

				InCircleData_A[xnum2+3][0] =get<0>(tempC) + X_Vec;
				InCircleData_A[xnum2+3][1] = get<1>(tempC) + Y_Vec;
				InCircleData_A[xnum2+3][2] = get<2>(tempC) + Z_Vec-m_Depth;
			}
			num = num + 4;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// �����������
	// ���Ǿ��� Χ�ߵ�����
	//////////////////////////////////////////////////////////////////////////
	OutCircleData_8[0][0] = 0;OutCircleData_8[0][1] = 0;OutCircleData_8[0][2] = 0;
	OutCircleData_8[1][0] = 0;OutCircleData_8[1][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[1][2] = 0;
	OutCircleData_8[2][0] = 0;OutCircleData_8[2][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[2][2] = -m_Depth;
	OutCircleData_8[3][0] = 0;OutCircleData_8[3][1] = 0;OutCircleData_8[3][2] = -m_Depth;

	OutCircleData_8[4][0] = 0;OutCircleData_8[4][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[4][2] = 0;
	OutCircleData_8[5][0] = m_Width;OutCircleData_8[5][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[5][2] = 0;
	OutCircleData_8[6][0] = m_Width;OutCircleData_8[6][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[6][2] = -m_Depth;
	OutCircleData_8[7][0] = 0;OutCircleData_8[7][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[7][2] = -m_Depth;

	OutCircleData_8[8][0] = m_Width;OutCircleData_8[8][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[8][2] = 0;
	OutCircleData_8[9][0] = m_Width;OutCircleData_8[9][1] = 0;OutCircleData_8[9][2] = 0;
	OutCircleData_8[10][0] = m_Width;OutCircleData_8[10][1] = 0;OutCircleData_8[10][2] = -m_Depth;
	OutCircleData_8[11][0] = m_Width;OutCircleData_8[11][1] = MinAndMax[0] + MiddleWidth + m_Diameter/2;OutCircleData_8[11][2] = -m_Depth;

	OutCircleData_8[12][0] = m_Width;OutCircleData_8[12][1] = 0;OutCircleData_8[12][2] = 0;
	OutCircleData_8[13][0] = 0;OutCircleData_8[13][1] = 0;OutCircleData_8[13][2] = 0;
	OutCircleData_8[14][0] = 0;OutCircleData_8[14][1] = 0;OutCircleData_8[14][2] = -m_Depth;
	OutCircleData_8[15][0] = m_Width;OutCircleData_8[15][1] = 0;OutCircleData_8[15][2] = -m_Depth;
}

void CJoint2::InitJoint( void )
{
	m_Joint = new SoSeparator;
	//SoDepthBuffer *blueDepth =  new SoDepthBuffer;
	//blueDepth->test.setValue(TRUE);
	//blueDepth->function.setValue(SoDepthBuffer::ALWAYS);
	//m_Bar->addChild(blueDepth);
	// ���ƽڵ�
	tempStr = m_Name;// + _T("GROUP");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Joint->setName(tempSbStr);
	// ��ɫ�ڵ�
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
	// λ�ƽڵ�
	m_Translation = new SoTranslation;
	m_Translation->translation.setValue(m_Trans);
	tempStr = m_Name+_T("TRANSLATION");
	tempWCHAR = tempStr.GetBuffer(tempStr.GetLength()+1);
	tempSbStr.fromWideChar(tempWCHAR);
	m_Translation->setName(tempSbStr);
	// ��ת�ڵ�
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
	//  ����ڵ�
	//SoCoordinate3 * ss = new SoCoordinate3;
	//ss->point.set1Value(0,0,0,0);
	// ���ĵ�
	//////////////////////////////////////////////////////////////////////////
	// ���Լ������� soswitch ����
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
	//
	// ��ͼģʽ�ڵ�
	//SoDrawStyle * m_DrawStyle = new SoDrawStyle;
	//m_DrawStyle->style.setValue(SoDrawStyle::LINES);

	m_Joint->addChild(m_Translation);
	m_Joint->addChild(m_Rotation);
	m_Joint->addChild(m_Material);
	m_Joint->addChild(CoordSwitch);
	//m_Bar->addChild(m_DrawStyle);
	//m_Bar->addChild(m_Color);
	//m_Bar->addChild(ss);
}

wchar_t* CJoint2::Cstring2Wchar( CString cstr )
{
	//CString origCString("Hello, World!"); 
	wchar_t* wCharString = cstr.GetBuffer(cstr.GetLength()+1); 
	return wCharString;
}

void CJoint2::setHwnd( Hwnd tt )
{
	mainHwnd = tt;
}

SoSeparator * CJoint2::CreateJoint()
{
	//////////////////////////////////////////////////////////////////////////
	//	 ��ʽ�Ĳ������㷽��
	InitJoint();
	//////////////////////////////////////////////////////////////////////////
	// ��ʼ���׵����ݣ� ��ʱ���׷�Ϊ�ĵȷ����ڲ���
	SetCircleDatad_A();
	SetFourCircleList();

	//////////////////////////////////////////////////////////////////////////
	// ���÷���
	SoShapeHints * m_hints = new SoShapeHints;
	m_hints->windingType=SoShapeHints::ODD_TYPE;
	m_hints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	//m_Bar->addChild(m_hints);

	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	//myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
	//myVertexProperty->normal.setValues(0, 9, norms);
	//myVertexProperty->normal.set1Value(0, SbVec3f(0, 0, 1));
	//myVertexProperty->normalBinding = SoNormalBinding::PER_FACE;
	myVertexProperty->normalBinding = SoNormalBinding::OVERALL;
	// ÿ����4���� ��8���� �����4����
	myVertexProperty->vertex.setValues(0, 300,CutCircleData_A);
	//myVertexProperty->vertex.setValues(0, 7, vertices);
	static int32_t numvertices[5] = {4,37,37,37,37};
	// �����
	SoFaceSet *UpFace = new SoFaceSet;
	// ���ƶ���
	UpFace->numVertices.setValues(0, 5, numvertices);
	UpFace->vertexProperty.setValue(myVertexProperty);
	//m_Bar->addChild(myFaceSet);
	// ���Ƶ���
	SoFaceSet *myFaceSet2 = new SoFaceSet;
	myFaceSet2 = UpFace;
	SoTranslation * depthTran = new SoTranslation;
	depthTran->translation.setValue(0,0,-m_Depth);
	SoSeparator * DownFace = new SoSeparator;
	DownFace->setName("DownData");
	DownFace->addChild(depthTran);
	DownFace->addChild(m_hints);
	DownFace->addChild(myFaceSet2);

	SoSeparator * UpFaceSep = new SoSeparator;
	UpFaceSep->setName("UpData");
	// �����������֧�ڵ�
	//UpDownFace->addChild(m_hints);
	//m_Bar->addChild(m_hints);
	//
	// �����
	SoTranslation *UpFaceTra = new SoTranslation;
	UpFaceSep->addChild(UpFaceTra);
	UpFaceSep->addChild(m_hints);
	UpFaceSep->addChild(UpFace);
	m_Joint->addChild(UpFaceSep);

	m_Joint->addChild(DownFace);
	//UpDownFace->addChild(UpFace);
	//UpDownFace->addChild(DownFace);
	//m_Bar->addChild(UpDownFace);
	// �����ڲ�
	SoVertexProperty *myVertexPropertyIN = new SoVertexProperty;
	myVertexPropertyIN->normalBinding = SoNormalBinding::PER_FACE;
	//myVertexPropertyIN->vertex.setValues(0, 128, InCircleData_8);//1184
	myVertexPropertyIN->vertex.setValues(0, 592, InCircleData_A);
	static int32_t IN_numvertices[148];
	for (int tt=0; tt<148;tt++)
	{
		IN_numvertices[tt] = 4;
	}
	//static int32_t IN_numvertices[32] = {
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//4,4,4,4,4,4,4,4,
	//};
	SoFaceSet *myIN_FaceSet = new SoFaceSet;
	myIN_FaceSet->numVertices.setValues(0, 148, IN_numvertices);
	myIN_FaceSet->vertexProperty.setValue(myVertexPropertyIN);
	SoSeparator * CylinderFaces = new SoSeparator;
	CylinderFaces->setName("CylinderData");
	SoTranslation * InFaceTra = new SoTranslation;
	CylinderFaces->addChild(InFaceTra);
	SoShapeHints * InHints = new SoShapeHints;
	CylinderFaces->addChild(InHints);
	CylinderFaces->addChild(myIN_FaceSet);
	m_Joint->addChild(CylinderFaces);
	// ������Χ
	SoVertexProperty *myVertexPropertyOut = new SoVertexProperty;
	myVertexPropertyOut->normalBinding = SoNormalBinding::PER_FACE;
	myVertexPropertyOut->vertex.setValues(0, 16, OutCircleData_8);
	static int32_t Out_numvertices[4] = {4,4,4,4};
	SoFaceSet *myOut_FaceSet = new SoFaceSet;
	myOut_FaceSet->numVertices.setValues(0, 4, Out_numvertices);
	myOut_FaceSet->vertexProperty.setValue(myVertexPropertyOut);
	SoSeparator * OutSkin = new SoSeparator;
	OutSkin->setName("OutSkin");
	SoTranslation * OutFaceTra = new SoTranslation;
	OutSkin->addChild(OutFaceTra);
	SoShapeHints * OutHints = new SoShapeHints;
	OutSkin->addChild(OutHints);
	OutSkin->addChild(myOut_FaceSet);
	m_Joint->addChild(OutSkin);

	//////////////////////////////////////////////////////////////////////////
	// ������Χ
	SoVertexProperty *FirstCircleProperty = new SoVertexProperty;
	FirstCircleProperty->normalBinding = SoNormalBinding::PER_FACE;
	FirstCircleProperty->vertex.setValues(0, 37, Circle1Pos);
	static int32_t testNum[1] = {36};
	SoFaceSet *testFace = new SoFaceSet;
	testFace->numVertices.setValues(0, 1, testNum);
	testFace->vertexProperty.setValue(FirstCircleProperty);
	SoSeparator * testFaceSep = new SoSeparator;
	testFaceSep->setName("testSkin");
	SoTranslation * testFaceTra = new SoTranslation;
	testFaceSep->addChild(testFaceTra);
	SoShapeHints * testHints = new SoShapeHints;
	testFaceSep->addChild(testHints);
	testFaceSep->addChild(testFace);
	m_Joint->addChild(testFaceSep);

	//////////////////////////////////////////////////////////////////////////
	//// ���Ի���һ��Բ
	//SoVertexProperty *FirstCircleVertexProperty = new SoVertexProperty;
	////FirstCircleVertexProperty->normalBinding = SoNormalBinding::OVERALL;
	//FirstCircleVertexProperty->vertex.setValues(0, 100,Circle1Pos);
	//FirstCircleVertexProperty->vertex.setValues(0, 100,OutCircleData_8);
	//static int32_t testNumVertices[1] = {36};
	//SoFaceSet * testFace = new SoFaceSet;
	//testFace->numVertices.setValues(0, 1, testNumVertices);
	//testFace->vertexProperty.setValue(FirstCircleVertexProperty);
	//SoSeparator * testFaceSep = new SoSeparator;
	//testFaceSep->setName("testData");
	//// ��ɫ�ڵ�
	//SoMaterial * testMaterial = new SoMaterial;
	//testMaterial->diffuseColor.setValue(0,0,1);

	//testFaceSep->addChild(testMaterial);
	//testFaceSep->addChild(new SoTranslation);
	//testFaceSep->addChild(new SoShapeHints);
	//testFaceSep->addChild(testFace);
	////////////////////////////////////////////////////////////////////////
	//SoPointSet * dd = new SoPointSet;
	//SoVertexProperty * testPointVertexProperty = new SoVertexProperty;
	////FirstCircleVertexProperty->normalBinding = SoNormalBinding::OVERALL;
	//float testPointPro[1][3]; testPointPro[0][0] = 1; testPointPro[0][1] = 1; testPointPro[0][2] = 0;
	//testPointVertexProperty->vertex.setValues(0, 1,testPointPro);
	//dd->vertexProperty.setValue(testPointVertexProperty);
	//dd->numPoints.setValue(1);

	//SoCoordinate3 * testPoint = new SoCoordinate3;
	//float ss[3];
	//testPoint->point.set1Value(0,50,50,0);
	//testPoint->
	//testFaceSep->addChild(testPoint);
	//m_Joint->addChild(testFaceSep);
	return m_Joint;
}

void CJoint2::SetFourCircleList()
{
	for (int i = 4; i<40; i++)
	{
		Circle1Pos[i-4][0] = CutCircleData_A[i][0];
		Circle1Pos[i-4][1] = CutCircleData_A[i][1];
		Circle1Pos[i-4][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=40; i<76; i++)
	{
		Circle2Pos[i-40][0] = CutCircleData_A[i][0];
		Circle2Pos[i-40][1] = CutCircleData_A[i][1];
		Circle2Pos[i-40][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=76; i<112; i++)
	{
		Circle3Pos[i-76][0] = CutCircleData_A[i][0];
		Circle3Pos[i-76][1] = CutCircleData_A[i][1];
		Circle3Pos[i-76][2] = CutCircleData_A[i][2];
	}
	//
	for (int i=112; i<148; i++)
	{
		Circle4Pos[i-112][0] = CutCircleData_A[i][0];
		Circle4Pos[i-112][1] = CutCircleData_A[i][1];
		Circle4Pos[i-112][2] = CutCircleData_A[i][2];
	}
}
