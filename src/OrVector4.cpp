/*************************************************************************

	OrVector3.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 4D-Vektoren
	
	Statistik:
	In Arbeit: Nein
	Bugs: Keine
	Speicherlecks: Keine
	Unvertig: ---

*************************************************************************/

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

// ******************************************************************** //
OrVector4 OrVec4Random()
{
	return OrVec4NormalizeEx(OrVector4(OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f));
}

// ******************************************************************** //
// 4D-Vektor mit Matrix multiplizieren
OrVector4 OrVec4Transform(const OrVector4& v,
						  const OrMatrix& m)
{
	return OrVector4(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
					 v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
					 v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
					 v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44);
}

// ******************************************************************** //
// Kreuzprodukt 3 4D - vektoren
OrVector4 OrVec4Cross(const OrVector4& v1, const OrVector4& v2, const OrVector4& v3)
{
    return OrVector4(v1.y * (v2.z * v3.w - v2.w * v3.z) -
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

// ******************************************************************** //