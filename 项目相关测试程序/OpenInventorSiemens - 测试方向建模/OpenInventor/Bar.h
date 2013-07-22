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

class CBar
{
public:
	CBar(void);
	~CBar(void);
};

