/*************************************************************************

	Vector3.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 3D-Vektoren
	
*************************************************************************/

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrMatrix.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// ******************************************************************** //
// Aus Abhängigkeitsgründen separieren
Vector3 OrE::Math::Vec3Random()
{
	return Vec3NormalizeEx(Vector3(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f));
}

// ******************************************************************** //
// 3D-Positionsvektor transformieren
Vector3 OrE::Math::Vec3TransformCoords(const Vector3& v,
									   const Matrix& m,
									   float* const pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	Vector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	if(w != 1.0f) vResult /= (w);
	//!!!/= fabs(w) ??? was passiert mit Koordinaten im negativen z-Bereich->werden Positiv

	// Wenn erwünscht, w kopieren
	if(pfOutW) *pfOutW = w;

	return vResult;
}

// ******************************************************************** //
// 3D-Positionsvektor transformieren
Vector3 OrE::Math::Vec3TransformCoords(const Vector3& v,
									   const Matrix& m)
{
	// Vektor mit Matrix multiplizieren
	Vector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	return vResult / w;
}

// ******************************************************************** //
// 3D-Richtungsvektor transformieren
Vector3 OrE::Math::Vec3TransformDirection(const Vector3& v,
										  const Matrix& m)
{
	// Vektor mit Matrix multiplizieren
	Vector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen entfällt da es sich um Richtungsvektoren handelt

	return vResult;
}

// ******************************************************************** //
// 3D-Richtungsvektor transformieren
/*Vector3 OrE::Math::Vec3TransformNormal(const Vector3& v,
									   const Matrix& m)
{
	// Vektorlänge berechnen
	const float fLength = Vec3Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	const Matrix mTransform(MatrixTranspose(MatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vec3Normalize(Vector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                             v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
									 v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}*/

// ******************************************************************** //
// 3D-Richtungsvektor transformieren (transponierte invertierte Matrix wird übergeben)
/*Vector3 OrE::Math::Vec3TransformNormal_TranspInv(const Vector3& v,
												 const Matrix& m)
{
	// Vektorlänge berechnen
	const float fLength = Vec3Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vec3Normalize(Vector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
									 v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
									 v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}*/

// ******************************************************************** //