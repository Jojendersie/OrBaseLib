// ******************************************************************************** //
// OrVector4.cpp																	//
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
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

// ******************************************************************************** //
Vector4 OrE::Math::Vec4Random()
{
	return Vec4NormalizeEx(Vector4(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f));
}

// ******************************************************************************** //
// 4D-Vektor mit Matrix multiplizieren
Vector4 OrE::Math::Vec4Transform(const Vector4& v,
								 const Matrix& m)
{
	return Vector4(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
					 v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
					 v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
					 v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44);
}

// ******************************************************************************** //
// Kreuzprodukt 3 4D - vektoren
Vector4 OrE::Math::Vec4Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3)
{
    return Vector4(v1.y * (v2.z * v3.w - v2.w * v3.z) -
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