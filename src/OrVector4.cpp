// ******************************************************************************** //
// OrVec4.cpp																	//
// =============																	//
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
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

// ******************************************************************************** //
Vec4 OrE::Math::Vec4::Random()
{
	return Vec4(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f).NormalizeEx();
}

// ******************************************************************************** //
// 4D-Vektor mit Matrix multiplizieren
/*const Vec4& OrE::Math::Vec4::Transform(const Matrix& m)
{
	x = x * m.m11 + y * m.m21 + z * m.m31 + w * m.m41;
	y = x * m.m12 + y * m.m22 + z * m.m32 + w * m.m42;
	z = x * m.m13 + y * m.m23 + z * m.m33 + w * m.m43;
	w = x * m.m14 + y * m.m24 + z * m.m34 + w * m.m44;
	return *this;
}

Vec4 OrE::Math::Vec4::Transform(const Vec4& v,
								const Matrix& m)
{
	return Vec4(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
				v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
				v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
				v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44);
}*/

// ******************************************************************************** //
// Kreuzprodukt 3 4D - vektoren
const Vec4& OrE::Math::Vec4::Cross(const Vec4& v2, const Vec4& v3)
{
    x = y * (v2.z * v3.w - v2.w * v3.z) -
		z * (v2.y * v3.w - v2.w * v3.y) +
		w * (v2.y * v3.z - v2.z * v3.y),

	y =-(x * (v2.z * v3.w - v3.z * v2.w) -
		 z * (v2.x * v3.w - v3.x * v2.w) +
		 w * (v2.x * v3.z - v3.x * v2.z)),

	z = x * (v2.y * v3.w - v3.y * v2.w) -
		y * (v2.x * v3.w - v3.x * v2.w) +
		w * (v2.x * v3.y - v3.x * v2.y),

	w =-(x * (v2.y * v3.z - v3.y * v2.z) -
		 y * (v2.x * v3.z - v3.x * v2.z) +
		 z * (v2.x * v3.y - v3.x * v2.y));

	return *this;
}

Vec4 OrE::Math::Vec4::Cross(const Vec4& v1, const Vec4& v2, const Vec4& v3)
{
    return Vec4(v1.y * (v2.z * v3.w - v2.w * v3.z) -
				v1.z * (v2.y * v3.w - v2.w * v3.y) +
				v1.w * (v2.y * v3.z - v2.z * v3.y),

			  -(v1.x * (v2.z * v3.w - v3.z * v2.w) -
				v1.z * (v2.x * v3.w - v3.x * v2.w) +
				v1.w * (v2.x * v3.z - v3.x * v2.z)),

				v1.x * (v2.y * v3.w - v3.y * v2.w) -
				v1.y * (v2.x * v3.w - v3.x * v2.w) +
				v1.w * (v2.x * v3.y - v3.x * v2.y),

			  -(v1.x * (v2.y * v3.z - v3.y * v2.z) -
				v1.y * (v2.x * v3.z - v3.x * v2.z) +
				v1.z * (v2.x * v3.y - v3.x * v2.y)) );
}

// *************************************EOF**************************************** //