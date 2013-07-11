// ******************************************************************************** //
// OrVec2.cpp																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quite easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "../include/OrTypeDef.h"
#include "../include/OrFastMath.h"
#include "../include/OrVector2.h"
#include "../include/OrVector3.h"
#include "../include/OrMatrix.h"
#include "../include/OrRandom.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// The most operators and functions are implemented inline in the header

// ******************************************************************************** //
// Create a uniformly distributed random direction vector ( length == 1 ).
// No inline because of Rand-dependency (-> more includes)
Vec2 Vec2::RandomDirection()
{
	float fAngle = Rand()*f2Pi;
	return Vec2( cos(fAngle), sin(fAngle) );
}

// ******************************************************************************** //
// Create a random point uniformly distributed in the [-1,1]^3 cube.
Vec2 Vec2::RandomPosition()
{
	return Vec2(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f);
}

// ******************************************************************************** //
// Rotate a point in 2D space (Cartesian coordinates)
Vec2 OrE::Math::Vec2::Rotate(const float _fAlpha) const
{
	float fCos = Cos(_fAlpha);
	float fSin = Sin(_fAlpha);
	return Vec2(x*fCos - y*fSin,
				x*fSin + y*fCos);
}

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