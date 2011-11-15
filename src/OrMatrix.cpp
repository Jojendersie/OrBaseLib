/*************************************************************************

	OrMatrix.cpp
	============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Matrizenrechnung
	
	Autor: Johannes Jendersie
	Verwendete Quellen: TriBase-Engine

*************************************************************************/

#include <string.h>
#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"
//#include "..\Includes\OrPlane.h"

// ******************************************************************** //
OrMatrix::OrMatrix(const float* pfValue)	{memcpy(n, pfValue, sizeof(float)*16);/*dwMatrixID = Or_MatrixIDCounter++;*/}

// ******************************************************************** //
// Zuweisungsoperatoren
OrMatrix& OrMatrix::operator = (const OrMatrix& m) {memcpy(n, m.n, sizeof(float)*16); return *this;}

// ******************************************************************** //
// Translationsmatrix berechnen
OrMatrix OrMatrixTranslation(const OrVector3& v)
{
	return OrMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************** //
// Translationsmatrix berechnen
OrMatrix OrMatrixTranslation(const float x, const float y, const float z)
{
	return OrMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x,    y,    z,    1.0f);
}

// ******************************************************************** //
// Rotationsmatrix für Rotation um die x-Achse berechnen
OrMatrix OrMatrixRotationX(const float f)
{
	OrMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = OrCos(f);
	mResult.m23 = OrSin(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************** //
// Rotationsmatrix für Rotation um die y-Achse berechnen
OrMatrix OrMatrixRotationY(const float f)
{
	OrMatrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = OrCos(f);
	mResult.m31 = OrSin(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************** //
// Rotationsmatrix für Rotation um die z-Achse berechnen
OrMatrix OrMatrixRotationZ(const float f)
{
	OrMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = OrCos(f);
	mResult.m12 = OrSin(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************** //
// Rotiert um alle drei Achsen
OrMatrix OrMatrixRotation(const float x,
									  const float y,
									  const float z)
{
	float fSinX = OrSin(x), fSinY = OrSin(y), fSinZ = OrSin(z);
	float fCosX = OrCos(x), fCosY = OrCos(y), fCosZ = OrCos(z);
	float fSinXY = fSinX*fSinY;
	float fCosYZ = fCosY*fCosZ;
	float fCosYSinZ = fCosY*fSinZ;
	return OrMatrix(fCosYZ-fSinXY*fSinZ,		fCosX*fSinZ, fSinY*fCosZ+fSinX*fCosYSinZ,	0.0f,
					-fCosYSinZ-fSinXY*fCosZ,	fCosX*fCosZ, fSinX*fCosYZ-fSinY*fSinZ,		0.0f,
					-fCosX*fSinY,				-fSinX,		 fCosX*fCosY,					0.0f,
					0.0f,						0.0f,		 0.0f,							1.0f);
	//return OrMatrixRotationZ(z) *
	//	   OrMatrixRotationX(x) *
	//	   OrMatrixRotationY(y);
}

// ******************************************************************** //
// Rotiert um alle drei Achsen (Winkel in Vektor)
OrMatrix OrMatrixRotation(const OrVector3& v)
{
	return OrMatrixRotation(v.x, v.y, v.z);
//	return OrMatrixRotationZ(v.z) *
//		   OrMatrixRotationX(v.x) *
//		   OrMatrixRotationY(v.y);
}

// ******************************************************************** //
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
OrMatrix OrMatrixRotationAxis(const OrVector3& v,
							  const float f)
{
	// Sinus und Kosinus berechnen
	const float fSin = OrSin(-f);
	const float fCos = OrCos(-f);
	const float fOneMinusCos = 1.0f - fCos;

	// Achsenvektor normalisieren
	const OrVector3 vAxis(OrVec3Normalize(v));

	// Matrix erstellen
	return OrMatrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
		            (vAxis.x * vAxis.y) * fOneMinusCos - (vAxis.z * fSin),
				    (vAxis.x * vAxis.z) * fOneMinusCos + (vAxis.y * fSin),
					0.0f,
					(vAxis.y * vAxis.x) * fOneMinusCos + (vAxis.z * fSin),
					(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
					(vAxis.y * vAxis.z) * fOneMinusCos - (vAxis.x * fSin),
					0.0f,
					(vAxis.z * vAxis.x) * fOneMinusCos - (vAxis.y * fSin),
					(vAxis.z * vAxis.y) * fOneMinusCos + (vAxis.x * fSin),
					(vAxis.z * vAxis.z) * fOneMinusCos + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}

// ******************************************************************** //
// Direkte Berechnung von OrMatrixRotation*OrMatrixTranslation
OrMatrix OrMatrixRotation_Translatation(const OrVector3& vR,
										const OrVector3& vP)
{
	float fSinX = OrSin(vR.x), fSinY = OrSin(vR.y), fSinZ = OrSin(vR.z);
	float fCosX = OrCos(vR.x), fCosY = OrCos(vR.y), fCosZ = OrCos(vR.z);
	float fSinXY = fSinX*fSinY;
	float fCosYZ = fCosY*fCosZ;
	float fCosYSinZ = fCosY*fSinZ;
	return OrMatrix(fCosYZ-fSinXY*fSinZ,		fCosX*fSinZ, fSinY*fCosZ+fSinX*fCosYSinZ,	0.0f,
					-fCosYSinZ-fSinXY*fCosZ,	fCosX*fCosZ, fSinX*fCosYZ-fSinY*fSinZ,		0.0f,
					-fCosX*fSinY,				-fSinX,		 fCosX*fCosY,					0.0f,
					vP.x,						vP.y,		 vP.z,							1.0f);
}

// ******************************************************************** //
// Skalierungsmatrix berechnen
OrMatrix OrMatrixScaling(const OrVector3& v)
{
	// Skalierungsmatrix berechnen
	return OrMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************** //
// Skalierungsmatrix berechnen (float - erhalten der Proportionen)
OrMatrix OrMatrixScaling(const float f)
{
	// Skalierungsmatrix berechnen
	return OrMatrix(f,  0.0f, 0.0f, 0.0f,
					0.0f, f,  0.0f, 0.0f,
					0.0f, 0.0f, f,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************** //
// Liefert eine Achsenmatrix
OrMatrix OrMatrixAxis(const OrVector3& vXAxis,
					  const OrVector3& vYAxis,
					  const OrVector3& vZAxis)
{
	return OrMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************** //
// Determinante einer 3x3 Matrix berechnen
float OrMatrixDet3(const OrMatrix& m)
{
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************** //
// Determinante einer Matrix berechnen (Entwicklungssatz von Laplace)
float OrMatrixDet(const OrMatrix& m)
{
	// Determinante der linken oberen 3x3-Teilmatrix berechnen
	// (also 4. Zeile und 4. Spalte streichen)
	float detA44 = + m.m11 * (m.m22 * m.m33 - m.m23 * m.m32)
				   - m.m12 * (m.m21 * m.m33 - m.m23 * m.m31)
				   + m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
	// (4. Zeile und 3. Spalte streichen)
	float detA43 = + m.m11 * (m.m22 * m.m34 - m.m24 * m.m32)
				   - m.m12 * (m.m21 * m.m34 - m.m24 * m.m31)
				   + m.m14 * (m.m21 * m.m32 - m.m22 * m.m31);
	// (4. Zeile und 2. Spalte streichen)
	float detA42 = + m.m11 * (m.m23 * m.m34 - m.m24 * m.m33)
				   - m.m13 * (m.m21 * m.m34 - m.m24 * m.m31)
				   + m.m14 * (m.m21 * m.m33 - m.m23 * m.m31);
	// (4. Zeile und 1. Spalte streichen)
	float detA41 = + m.m12 * (m.m23 * m.m34 - m.m24 * m.m33)
				   - m.m13 * (m.m22 * m.m34 - m.m24 * m.m32)
				   + m.m14 * (m.m22 * m.m33 - m.m23 * m.m32);

	return - m.m41 * detA41
		   + m.m42 * detA42
		   - m.m43 * detA43
		   + m.m44 * detA44;
}

// ******************************************************************** //
// Funktion für das Tauschen von Variablen (32 bit)
inline void OrSwap32(void* p1, void* p2)
{
	*(__int32*)(p1) = *(__int32*)(p1) ^ *(__int32*)(p2);
	*(__int32*)(p2) = *(__int32*)(p1) ^ *(__int32*)(p2);
	*(__int32*)(p1) = *(__int32*)(p1) ^ *(__int32*)(p2);
}

// ******************************************************************** //
// Invertierte Matrix berechnen
// Wenn nicht invertierbar, so wird die Einheitsmatrix angegeben.
OrMatrix OrMatrixInvert(const OrMatrix& m)
{
	OrMatrix mSolution;
	OrVector4 v, vec[3];
	float fDet;

	fDet = OrMatrixDet(m);

	if(!fDet)
		return OrMatrixIdentity();

	// Tausche Vorzeichen in jedem Durchlauf
	float fSignDet = 1.0f/fDet;

	for(int i=0; i<4; i++)
	{
		// Determinantenentwicklung als Kreuzprodukt
		v = OrVec4Cross(m.m[0<i?0:1], m.m[1<i?1:2], m.m[2<i?2:3]);

		// Vorzeichen * Determinante_i / Determinante
		mSolution.m[0][i] = fSignDet * v.x;
		mSolution.m[1][i] = fSignDet * v.y;
		mSolution.m[2][i] = fSignDet * v.z;
		mSolution.m[3][i] = fSignDet * v.w;
		fSignDet *= -1.0f;
	}

	return mSolution;
}
/*{
	// Zur Beschleunigung wird Loop-entrolling angewendet
	// Zur Kürzung des Quelltexts wird auf Kommentare verzichtet.
	// Siehe OrMatrixSolveEquation für genauere Erklärungen.
	// Allgemein wird mit dem Gauß-Jordan verfahren und 4 parallelen
	// Lösungsvektoren gearbeitet
	// TODO: Pointerarithmetik beschleunigen!!!
	OrMatrix mSolution( 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f );
	OrMatrix mA(m);

	// Pivot-Prüfung
	if(mA.m[0][0]==0.0f)
	{
		dword j=1;
		for(;j<4;++j)
			if(mA.m[0][j]!=0.0f && mA.m[j][0]!=0)
			{
				OrSwap32(&mA.m[0][0],&mA.m[j][0]);
				OrSwap32(&mA.m[0][1],&mA.m[j][1]);
				OrSwap32(&mA.m[0][2],&mA.m[j][2]);
				OrSwap32(&mA.m[0][3],&mA.m[j][3]);
				OrSwap32(&mSolution.m[0][0],&mSolution.m[j][0]);
				OrSwap32(&mSolution.m[0][1],&mSolution.m[j][1]);
				OrSwap32(&mSolution.m[0][2],&mSolution.m[j][2]);
				OrSwap32(&mSolution.m[0][3],&mSolution.m[j][3]);
				break;
			}
		if(j>=4) return OrMatrixIdentity();
	}

	// Das Pivot-Element ist != 0 => teilen der Zeile
	float fInvPivot = 1.0f/mA.m[0][0];
	mA.m[0][0] = 1.0f;
	mA.m[0][1] *= fInvPivot;
	mA.m[0][2] *= fInvPivot;
	mA.m[0][3] *= fInvPivot;
	mSolution.m[0][0] *= fInvPivot;
	mSolution.m[0][1] *= fInvPivot;
	mSolution.m[0][2] *= fInvPivot;
	mSolution.m[0][3] *= fInvPivot;

	// Die neue Zeile von allen anderen subtrahieren
	for(dword j=1;j<4;++j)
	{
		float fFactor = mA.m[j][0];
		mA.m[j][0] = 0.0f;
		mA.m[j][1] -= fFactor*mA.m[0][1];
		mA.m[j][2] -= fFactor*mA.m[0][2];
		mA.m[j][3] -= fFactor*mA.m[0][3];
		mSolution.m[j][0] -= fFactor*mSolution.m[0][0];
		mSolution.m[j][1] -= fFactor*mSolution.m[0][1];
		mSolution.m[j][2] -= fFactor*mSolution.m[0][2];
		mSolution.m[j][3] -= fFactor*mSolution.m[0][3];
	}

	if(mA.m[1][1]==0.0f)
	{
		dword j=2;
		for(;j<4;++j)
			if(mA.m[1][j]!=0.0f && mA.m[j][1]!=0)
			{
				OrSwap32(&mA.m[1][1],&mA.m[j][1]);
				OrSwap32(&mA.m[1][2],&mA.m[j][2]);
				OrSwap32(&mA.m[1][3],&mA.m[j][3]);
				OrSwap32(&mSolution.m[1][0],&mSolution.m[j][0]);
				OrSwap32(&mSolution.m[1][1],&mSolution.m[j][1]);
				OrSwap32(&mSolution.m[1][2],&mSolution.m[j][2]);
				OrSwap32(&mSolution.m[1][3],&mSolution.m[j][3]);
				break;
			}
		if(j>=4) return OrMatrixIdentity();
	}

	// Das Pivot-Element ist != 0 => teilen der Zeile
	fInvPivot = 1.0f/mA.m[1][1];
	mA.m[1][1] = 1.0f;
	mA.m[1][2] *= fInvPivot;
	mA.m[1][3] *= fInvPivot;
	mSolution.m[1][0] *= fInvPivot;
	mSolution.m[1][1] *= fInvPivot;
	mSolution.m[1][2] *= fInvPivot;
	mSolution.m[1][3] *= fInvPivot;

	// Die neue Zeile von allen anderen subtrahieren
	for(dword j=0;j<4;++j)
		if(1!=j)
		{
			float fFactor = mA.m[j][1];
			mA.m[j][1] = 0.0f;
			mA.m[j][2] -= fFactor*mA.m[1][2];
			mA.m[j][3] -= fFactor*mA.m[1][3];
			mSolution.m[j][0] -= fFactor*mSolution.m[1][0];
			mSolution.m[j][1] -= fFactor*mSolution.m[1][1];
			mSolution.m[j][2] -= fFactor*mSolution.m[1][2];
			mSolution.m[j][3] -= fFactor*mSolution.m[1][3];
		}

	if(mA.m[2][2]==0.0f)
	{
		if(mA.m[2][3]!=0.0f && mA.m[3][2]!=0)
		{
			OrSwap32(&mA.m[2][2],&mA.m[3][2]);
			OrSwap32(&mA.m[2][3],&mA.m[3][3]);
			OrSwap32(&mSolution.m[2][0],&mSolution.m[3][0]);
			OrSwap32(&mSolution.m[2][1],&mSolution.m[3][1]);
			OrSwap32(&mSolution.m[2][2],&mSolution.m[3][2]);
			OrSwap32(&mSolution.m[2][3],&mSolution.m[3][3]);
		} else
			return OrMatrixIdentity();
	}

	// Das Pivot-Element ist != 0 => teilen der Zeile
	fInvPivot = 1.0f/mA.m[2][2];
	mA.m[2][2] = 1.0f;
	mA.m[2][3] *= fInvPivot;
	mSolution.m[2][0] *= fInvPivot;
	mSolution.m[2][1] *= fInvPivot;
	mSolution.m[2][2] *= fInvPivot;
	mSolution.m[2][3] *= fInvPivot;

	// Die neue Zeile von allen anderen subtrahieren
	for(dword j=0;j<4;++j)
		if(2!=j)
		{
			float fFactor = mA.m[j][2];
			mA.m[j][2] = 0.0f;
			mA.m[j][3] -= fFactor*mA.m[2][3];
			mSolution.m[j][0] -= fFactor*mSolution.m[2][0];
			mSolution.m[j][1] -= fFactor*mSolution.m[2][1];
			mSolution.m[j][2] -= fFactor*mSolution.m[2][2];
			mSolution.m[j][3] -= fFactor*mSolution.m[2][3];
		}

	if(mA.m[3][3]==0.0f) return OrMatrixIdentity();

	// Das Pivot-Element ist != 0 => teilen der Zeile
	fInvPivot = 1.0f/mA.m[3][3];
	//mA.m[3][3] = 1.0f;
	mSolution.m[3][0] *= fInvPivot;
	mSolution.m[3][1] *= fInvPivot;
	mSolution.m[3][2] *= fInvPivot;
	mSolution.m[3][3] *= fInvPivot;

	// Die neue Zeile von allen anderen subtrahieren
	for(dword j=0;j<3;++j)
	{
		float fFactor = mA.m[j][3];
		//mA.m[j][3] = 0.0f;
		mSolution.m[j][0] -= fFactor*mSolution.m[3][0];
		mSolution.m[j][1] -= fFactor*mSolution.m[3][1];
		mSolution.m[j][2] -= fFactor*mSolution.m[3][2];
		mSolution.m[j][3] -= fFactor*mSolution.m[3][3];
	}

	return mSolution;
}*/

// ******************************************************************** //
// Transponierte Matrix berechnen
OrMatrix OrMatrixTranspose(const OrMatrix& m)
{
	// Matrix transponieren
	return OrMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************** //
// Projektionsmatrix berechnen
OrMatrix OrMatrixProjection(const float fFOV,
							const float fAspect,
							const float fNearPlane,
							const float fFarPlane)
{
	const float s = 1.0f / OrTan(fFOV * 0.5f);
	const float fFrustumLengthInv = 1.0f / (fNearPlane - fFarPlane);

	return OrMatrix(s / fAspect, 0.0f, 0.0f,										0.0f,
		            0.0f,		 s,	   0.0f,										0.0f,
					0.0f,		 0.0f, (fNearPlane + fFarPlane)*fFrustumLengthInv,	(2.0f*fNearPlane*fFarPlane)*fFrustumLengthInv,
					0.0f,		 0.0f, -1.0f,										0.0f);
	// DirectX compatible version:
/*	const float s = 1.0f / OrTan(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return OrMatrix(s / fAspect, 0.0f, 0.0f,			0.0f,
		            0.0f,		 s,	   0.0f,			0.0f,
					0.0f,		 0.0f, Q,				1.0f,
					0.0f,		 0.0f, -Q * fNearPlane, 0.0f);*/
}

// ******************************************************************** //
// Projektionsmatrix berechnen
OrMatrix OrMatrixParallelProjection(const float fWidth,
									const float fHeigh,
									const float fNearPlane,
									const float fFarPlane)
{
	const float s = 2.0f*fNearPlane;
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return OrMatrix(s / fWidth,	0.0f,		0.0f,				0.0f,
		            0.0f,		s / fHeigh,	0.0f,				0.0f,
					0.0f,		0.0f,		Q,					1.0f,
					0.0f,		0.0f,		-Q * fNearPlane,	0.0f);
}

// ******************************************************************** //
// Kameramatrix berechnen
OrMatrix OrMatrixCamera(const OrVector3& vPos,
						const OrVector3& vLookAt,
						const OrVector3& vUp) // = OrVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	OrVector3 vZAxis(OrVec3Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	OrVector3 vXAxis(OrVec3Normalize(OrVec3Cross(vUp, vZAxis)));

	// y-Achse berechnen
	OrVector3 vYAxis(OrVec3Normalize(OrVec3Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mit ihr multiplizieren
	return OrMatrixTranslation(-vPos) *
	       OrMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************** //
// Texturmatrix berechnen
OrMatrix OrMatrixToTex2DMatrix(const OrMatrix& m)
{
	// Texturmatrix berechnen
	return OrMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************** //
// Eine Spiegelmatrix an gegebener Ebene berechnen
/*OrMatrix OrMatrixMirror(const OrPlane& p)
{
	return OrMatrix(-2.0f*p.a*p.a + 1.0f, -2.0f*p.b*p.a,	-2.0f*p.c*p.a,		  0.0f,
					-2.0f*p.a*p.b,		  -2.0f*p.b + 1.0f, -2.0f*p.c*p.b,		  0.0f,
					-2.0f*p.a*p.c,		  -2.0f*p.b*p.c,	-2.0f*p.c*p.c + 1.0f, 0.0f,
					-2.0f*p.a*p.d,		  -2.0f*p.b*p.d,	-2.0f*p.c*p.d,		  1.0f);
}

// Berechnung mit vorhergehender Normalisierung
OrMatrix OrMatrixMirror_Normalize(const OrPlane& p)																													// Eine Spiegelmatrix an gegebener Ebene (wird normalisiert) berechnen
{
	OrPlane n = OrPlaneNormalize(p);
	return OrMatrix(-2.0f*n.a*n.a + 1.0f, -2.0f*n.b*n.a,		-2.0f*n.c*n.a,		  0.0f,
					-2.0f*n.a*n.b,		  -2.0f*n.b*n.b + 1.0f, -2.0f*n.c*n.b,		  0.0f,
					-2.0f*n.a*n.c,		  -2.0f*n.b*p.c,		-2.0f*n.c*n.c + 1.0f, 0.0f,
					-2.0f*n.a*n.d,		  -2.0f*n.b*n.d,		-2.0f*n.c*n.d,		  1.0f);
}*/

// ******************************************************************** //
// Löst das Gleichungssystem Ax=v mit dem Gauß-Jordan verfahren
// gibt zurück, ob es eine Eindeutige Lösung gab.
// Die Lösung wird im eingegebenen Lösungsvektor beschrieben
bool OrMatrixSolveEquation(OrMatrix _A, OrVector4* _pV_X)
{
	for(dword i=0;i<4;++i)
	{
		// Absichern, dass das Pivot-Element != 0
		if(_A.m[i][i]==0.0f)
		{
			// Problem: wir müssen eine gültige Zeile zum Tauschen finden.
			// Diese muss an der gesuchten Stelle etwas stehen haben. Außerdem
			// muss in der aktuellen Zeile das spätere Element != 0 sein.
			dword j=i+1;
			for(;j<4;++j)
			{
				if(_A.m[i][j]!=0.0f && _A.m[j][i]!=0)
				{
					// gesuchte Zeile gefunden -> tauschen
					// !!!Zugriffe Beschleunigen, indem wir die
					// Pointerarithmetik manuell machen.
					OrSwap32(&_A.m[i][0],&_A.m[j][0]);
					OrSwap32(&_A.m[i][1],&_A.m[j][1]);
					OrSwap32(&_A.m[i][2],&_A.m[j][2]);
					OrSwap32(&_A.m[i][3],&_A.m[j][3]);
					OrSwap32(((float*)_pV_X)+i,((float*)_pV_X)+j);
					// weiterprüfen ist unnötig
					break;
				}
			}
			// Wenn es eine solche Zeile nicht gab kann das System nicht
			// eindeutig gelößt werden. Wir brechen einfach ab.
			if(j>=4) return false;
		}

		// Das Pivot-Element ist != 0 => teilen der Zeile
		float fInvPivot = 1.0f/_A.m[i][i];
		// In der ersten Spalte steht entwerden das Pivotelement, oder eine 0=>weglassen
		_A.m[i][1] *= fInvPivot;
		_A.m[i][2] *= fInvPivot;
		_A.m[i][3] *= fInvPivot;
		(*(((float*)_pV_X)+i)) *= fInvPivot;
		_A.m[i][i] = 1.0f;

		// Die neue Zeile von allen anderen subtrahieren
		for(dword j=0;j<4;++j)
			if(i!=j)
			{
				float fFactor = _A.m[j][i];
				// Bench: for(dword k=i+1;k<4;++k) _A.m[j,k] -= fFactor*_A.m[i,k];
				_A.m[j][1] -= fFactor*_A.m[i][1];
				_A.m[j][2] -= fFactor*_A.m[i][2];
				_A.m[j][3] -= fFactor*_A.m[i][3];
				(*(((float*)_pV_X)+j)) -= fFactor*(*(((float*)_pV_X)+i));
				_A.m[j][i] = 0.0f;
			}
	}
	return true;
}

// ******************************************************************** //
OrMatrix OrMatrixOrthonormal(const OrVector3& vNormal)
{
	// Über das Skalarprudukt den 2. Vektor bestimmen.
	OrVector3 v2 = (vNormal.x==1.0f)?OrVector3(0.0f, 1.0f, 0.0f): OrVec3Normalize((vNormal.y != 0.0f)? OrVector3(1.0f, -vNormal.x/vNormal.y, 0.0f) : OrVector3(1.0f, 0.0f, -vNormal.x/vNormal.z));
	// 3. Vektor über Kreuzprodukt ermitteln
	OrVector3 v3 = OrVec3Cross(vNormal, v2);
	return OrMatrix(v2.x,		v2.y,		v2.z,		0.0f,
					v3.x,		v3.y,		v3.z,		0.0f,
					vNormal.x,	vNormal.y,	vNormal.z,	0.0f,
					0.0f,		0.0f,		0.0f,		1.0f);
	//return OrMatrix(vNormal.x,	vNormal.y,	vNormal.z,	0.0f,
	//				v2.x,		v2.y,		v2.z,		0.0f,
	//				v3.x,		v3.y,		v3.z,		0.0f,
	//				0.0f,		0.0f,		0.0f,		1.0f);
}

// ******************************************************************** //