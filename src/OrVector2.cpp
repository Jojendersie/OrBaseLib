/*************************************************************************

	Vector2.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

*************************************************************************/

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrMatrix.h"
#include "..\Include\OrRandom.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// ******************************************************************** //
inline Vector2 OrE::Math::Vec2Random()
{
	return Vec2NormalizeEx(Vector2(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f));
}

// ******************************************************************** //
// Rotieren eines Punktes  in 2D-Raum
Vector2 OrE::Math::Vec2Rotate(const Vector2& _v,
							  const float _fAlpha)
{
	float fCos = Cos(_fAlpha);
	float fSin = Sin(_fAlpha);
	return Vector2(_v.x*fCos-_v.y*fSin, _v.x*fSin+_v.y*fCos);
}

// ******************************************************************** //
// 2D-Vektor mit Matrix multiplizieren
Vector2 OrE::Math::Vec2Transform(const Vector2& v, const Matrix2x3& m)
{
	// Multiply matrix from left: 2x3 * (2+1)x1 = 2x1
	return Vector2(m.m11*v.x + m.m12*v.y + m.m13,
					 m.m21*v.x + m.m22*v.y + m.m23);
}

// ******************************************************************** //