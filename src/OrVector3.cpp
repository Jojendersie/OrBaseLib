/*************************************************************************

	OrVector3.cpp
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
#include "..\Include\OrMatrix.h"

// ******************************************************************** //
// Aus Abhängigkeitsgründen separieren
OrVector3 OrVec3Random()
{
	return OrVec3NormalizeEx(OrVector3(OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f));
}

// ******************************************************************** //
// 3D-Positionsvektor transformieren
OrVector3 OrVector3TransformCoords(const OrVector3& v,
								   const OrMatrix& m,
								   float* const pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	OrVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
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
OrVector3 OrVector3TransformCoords(const OrVector3& v,
								   const OrMatrix& m)
{
	// Vektor mit Matrix multiplizieren
	OrVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
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
OrVector3 OrVector3TransformDirection(const OrVector3& v,
								      const OrMatrix& m)
{
	// Vektor mit Matrix multiplizieren
	OrVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen entfällt da es sich um Richtungsvektoren handelt

	return vResult;
}

// ******************************************************************** //
// 3D-Richtungsvektor transformieren
OrVector3 OrVector3TransformNormal(const OrVector3& v,
								   const OrMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = OrVec3Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	const OrMatrix mTransform(OrMatrixTranspose(OrMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return OrVec3Normalize(OrVector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                             v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
									 v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}

// ******************************************************************** //
// 3D-Richtungsvektor transformieren (transponierte invertierte Matrix wird übergeben)
OrVector3 OrVector3TransformNormal_TranspInv(const OrVector3& v,
											 const OrMatrix& m)
{
	// Vektorlänge berechnen
	const float fLength = OrVec3Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return OrVec3Normalize(OrVector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
									 v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
									 v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}

// ******************************************************************** //