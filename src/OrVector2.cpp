// ******************************************************************************** //
// OrVec2.cpp																	//
// =============																	//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project. You can do this by a comment at	https://github.com/Jojendersie/.		//
// Futhermore you have to state this project as a source of your project.			//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrMatrix.h"
#include "..\Include\OrRandom.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// ******************************************************************************** //
inline Vec2 OrE::Math::Vec2Random()
{
	return Vec2NormalizeEx(Vec2(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f));
}

// ******************************************************************************** //
// Rotieren eines Punktes  in 2D-Raum
Vec2 OrE::Math::Vec2Rotate(const Vec2& _v,
							  const float _fAlpha)
{
	float fCos = Cos(_fAlpha);
	float fSin = Sin(_fAlpha);
	return Vec2(_v.x*fCos-_v.y*fSin, _v.x*fSin+_v.y*fCos);
}

// ******************************************************************************** //
// 2D-Vektor mit Matrix multiplizieren
Vec2 OrE::Math::Vec2Transform(const Vec2& v, const Matrix2x3& m)
{
	// Multiply matrix from left: 2x3 * (2+1)x1 = 2x1
	return Vec2(m.m11*v.x + m.m12*v.y + m.m13,
					 m.m21*v.x + m.m22*v.y + m.m23);
}

// *************************************EOF**************************************** //