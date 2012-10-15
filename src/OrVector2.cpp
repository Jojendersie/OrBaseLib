// ******************************************************************************** //
// OrVec2.cpp																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
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
Vec2 OrE::Math::Vec2::Random()
{
	return Vec2(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f).NormalizeEx();
}

// ******************************************************************************** //
// Rotieren eines Punktes  in 2D-Raum
Vec2 OrE::Math::Vec2::Rotate(const float _fAlpha) const
{
	float fCos = Cos(_fAlpha);
	float fSin = Sin(_fAlpha);
	return Vec2(x*fCos - y*fSin,
				x*fSin + y*fCos);
}

/*Vec2 OrE::Math::Vec2::Rotate(const Vec2& _v,
							 const float _fAlpha)
{
	float fCos = Cos(_fAlpha);
	float fSin = Sin(_fAlpha);
	return Vec2(_v.x*fCos-_v.y*fSin, _v.x*fSin+_v.y*fCos);
}*/

// ******************************************************************************** //
// 2D-Vektor mit Matrix multiplizieren
/*const Vec2& OrE::Math::Vec2::Transform(const Matrix2x3& m)
{
	// Multiply matrix from left: 2x3 * (2+1)x1 = 2x1
	x = m.m11*x + m.m12*y + m.m13;
	y = m.m21*x + m.m22*y + m.m23;
	return *this;
}

Vec2 OrE::Math::Vec2::Transform(const Vec2& v, const Matrix2x3& m)
{
	// Multiply matrix from left: 2x3 * (2+1)x1 = 2x1
	return Vec2(m.m11*v.x + m.m12*v.y + m.m13,
				m.m21*v.x + m.m22*v.y + m.m23);
}*/

// ******************************************************************************** //
// The spherical interpolation applies only to normal vectors
Vec2 OrE::Math::Slerp(const Vec2& v1, const Vec2& v2, const float t)
{
	float fOmega = Arccos( Clamp(v1.Dot(v2), -1.0f, 1.0f) );
	//float fInvSin = 1.0f/Sin( fOmega );
	float f1 = Sin( fOmega * (1.0f-t) );// * fInvSin;
	float f2 = Sin( fOmega * t );// * fInvSin;
	return Vec2( v1.x*f1+v2.x*f2, v1.y*f1+v2.y*f2 ).Normalize();
}

// *************************************EOF**************************************** //