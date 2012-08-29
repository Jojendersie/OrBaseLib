// ******************************************************************************** //
// OrMatrix.cpp																		//
// ============																		//
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

#include <string.h>
#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"
#include "..\Include\OrPlane.h"

using namespace OrE::Math;

// ******************************************************************************** //
Matrix::Matrix(const float* pfValue)	{memcpy(n, pfValue, sizeof(float)*16);/*dwMatrixID = Or_MatrixIDCounter++;*/}

// ******************************************************************************** //
// Operators
Matrix& Matrix::operator = (const Matrix& m) {memcpy(n, m.n, sizeof(float)*16); return *this;}

// ******************************************************************************** //
// Multiply vector from left (interpret v as row vector with a fourth component of one)
Vec3 OrE::Math::operator * (const Vec3& v,
				 const Matrix& m)
{
	// Uniform coordinates in 4D -> division through w
	const double w = 1.0/(v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44);

	return Vec3(float( w * (v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41) ),
				float( w * (v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42) ),
				float( w * (v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43) ));
}

// ******************************************************************************** //
// Multiply vector from right (interpret v as col vector with a fourth component of one)
Vec3 OrE::Math::operator * (const Matrix& m,
				 const Vec3& v)
{
	// Uniform coordinates in 4D -> division through w
	const double w = 1.0/(v.x * m.m41 + v.y * m.m42 + v.z * m.m43 + m.m44);

	return Vec3(float( w * (v.x * m.m11 + v.y * m.m12 + v.z * m.m13 + m.m14) ),
				float( w * (v.x * m.m21 + v.y * m.m22 + v.z * m.m23 + m.m24) ),
				float( w * (v.x * m.m31 + v.y * m.m32 + v.z * m.m33 + m.m34) ));
}

// ******************************************************************************** //
// Multiply 4D-vector from left (interpret v as row vector)
Vec4 OrE::Math::operator * (const Vec4& v,
				 const Matrix& m)
{
	return Vec4( v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41 ,
				 v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42 ,
				 v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43 ,
				 v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44 );
}

// ******************************************************************************** //
// Multiply 4D-vector from right
Vec4 OrE::Math::operator * (const Matrix& m,
				 const Vec4& v)
{
	return Vec4( v.x * m.m11 + v.y * m.m12 + v.z * m.m13 + m.m14 ,
				 v.x * m.m21 + v.y * m.m22 + v.z * m.m23 + m.m24 ,
				 v.x * m.m31 + v.y * m.m32 + v.z * m.m33 + m.m34 ,
				 v.x * m.m41 + v.y * m.m42 + v.z * m.m43 + m.m44 );
}

// ******************************************************************************** //
// Translationsmatrix berechnen
Matrix OrE::Math::MatrixTranslation(const Vec3& v)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
		          0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************************** //
// Translationsmatrix berechnen
Matrix OrE::Math::MatrixTranslation(const float x, const float y, const float z)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
		          0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  x,    y,    z,    1.0f);
}

// ******************************************************************************** //
// Rotationsmatrix für Rotation um die x-Achse berechnen
Matrix OrE::Math::MatrixRotationX(const float f)
{
	Matrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = Cos(f);
	mResult.m23 = Sin(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************************** //
// Rotationsmatrix für Rotation um die y-Achse berechnen
Matrix OrE::Math::MatrixRotationY(const float f)
{
	Matrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = Cos(f);
	mResult.m31 = Sin(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************************** //
// Rotationsmatrix für Rotation um die z-Achse berechnen
Matrix OrE::Math::MatrixRotationZ(const float f)
{
	Matrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = Cos(f);
	mResult.m12 = Sin(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************************** //
// Rotiert um alle drei Achsen
Matrix OrE::Math::MatrixRotation(const float x,
						  const float y,
						  const float z)
{
	float fSinX = Sin(x), fSinY = Sin(y), fSinZ = Sin(z);
	float fCosX = Cos(x), fCosY = Cos(y), fCosZ = Cos(z);
	float fSinXY = fSinX*fSinY;
	float fCosYZ = fCosY*fCosZ;
	float fCosYSinZ = fCosY*fSinZ;
	return Matrix(fCosYZ-fSinXY*fSinZ,		fCosX*fSinZ, fSinY*fCosZ+fSinX*fCosYSinZ,	0.0f,
					-fCosYSinZ-fSinXY*fCosZ,	fCosX*fCosZ, fSinX*fCosYZ-fSinY*fSinZ,		0.0f,
					-fCosX*fSinY,				-fSinX,		 fCosX*fCosY,					0.0f,
					0.0f,						0.0f,		 0.0f,							1.0f);
	//return MatrixRotationZ(z) *
	//	   MatrixRotationX(x) *
	//	   MatrixRotationY(y);
}

// ******************************************************************************** //
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
Matrix OrE::Math::MatrixRotationAxis(const Vec3& vAxis,
									 const float f)
{
	// Sinus und Kosinus berechnen
	const double fSin = sin(double(-f));
	const double fCos = cos(double(-f));
	const double fOneMinusCos = 1.0 - fCos;
	const double AxSin = vAxis.x * fSin;
	const double AySin = vAxis.y * fSin;
	const double AzSin = vAxis.z * fSin;

	// Matrix erstellen
	return Matrix(float((vAxis.x * vAxis.x) * fOneMinusCos + fCos),
		          float((vAxis.x * vAxis.y) * fOneMinusCos - (AzSin)),
				  float((vAxis.x * vAxis.z) * fOneMinusCos + (AySin)),
				  0.0f,
				  float((vAxis.y * vAxis.x) * fOneMinusCos + (AzSin)),
				  float((vAxis.y * vAxis.y) * fOneMinusCos + fCos),
				  float((vAxis.y * vAxis.z) * fOneMinusCos - (AxSin)),
				  0.0f,
				  float((vAxis.z * vAxis.x) * fOneMinusCos - (AySin)),
				  float((vAxis.z * vAxis.y) * fOneMinusCos + (AxSin)),
				  float((vAxis.z * vAxis.z) * fOneMinusCos + fCos),
				  0.0f,
				  0.0f,
				  0.0f,
				  0.0f,
				  1.0f);
}

// ******************************************************************************** //
// Direkte Berechnung von MatrixRotation*MatrixTranslation
Matrix OrE::Math::MatrixRotation_Translatation(const Vec3& vR,
											   const Vec3& vP)
{
	float fSinX = Sin(vR.x), fSinY = Sin(vR.y), fSinZ = Sin(vR.z);
	float fCosX = Cos(vR.x), fCosY = Cos(vR.y), fCosZ = Cos(vR.z);
	float fSinXY = fSinX*fSinY;
	float fCosYZ = fCosY*fCosZ;
	float fCosYSinZ = fCosY*fSinZ;
	return Matrix(fCosYZ-fSinXY*fSinZ,		fCosX*fSinZ, fSinY*fCosZ+fSinX*fCosYSinZ,	0.0f,
				  -fCosYSinZ-fSinXY*fCosZ,	fCosX*fCosZ, fSinX*fCosYZ-fSinY*fSinZ,		0.0f,
				  -fCosX*fSinY,				-fSinX,		 fCosX*fCosY,					0.0f,
				  vP.x,						vP.y,		 vP.z,							1.0f);
}

// ******************************************************************************** //
// Skalierungsmatrix berechnen
Matrix OrE::Math::MatrixScaling(const Vec3& v)
{
	// Skalierungsmatrix berechnen
	return Matrix(v.x,  0.0f, 0.0f, 0.0f,
		          0.0f, v.y,  0.0f, 0.0f,
				  0.0f, 0.0f, v.z,  0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************************** //
// Skalierungsmatrix berechnen (float - erhalten der Proportionen)
Matrix OrE::Math::MatrixScaling(const float f)
{
	// Skalierungsmatrix berechnen
	return Matrix(f,  0.0f, 0.0f, 0.0f,
				  0.0f, f,  0.0f, 0.0f,
				  0.0f, 0.0f, f,  0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************************** //
// Liefert eine Achsenmatrix
Matrix OrE::Math::MatrixAxis(const Vec3& vXAxis,
					  const Vec3& vYAxis,
					  const Vec3& vZAxis)
{
	return Matrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		          vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
				  vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
				  0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************************** //
// Determinante einer 3x3 Matrix berechnen
float OrE::Math::MatrixDet3(const Matrix& m)
{
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************************** //
// Determinante einer Matrix berechnen (Entwicklungssatz von Laplace)
float OrE::Math::MatrixDet(const Matrix& m)
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

// ******************************************************************************** //
// Funktion für das Tauschen von Variablen (32 bit)
/*inline void OrSwap32(void* p1, void* p2)
{
	*(__int32*)(p1) = *(__int32*)(p1) ^ *(__int32*)(p2);
	*(__int32*)(p2) = *(__int32*)(p1) ^ *(__int32*)(p2);
	*(__int32*)(p1) = *(__int32*)(p1) ^ *(__int32*)(p2);
}*/

// ******************************************************************************** //
// Invertierte Matrix berechnen
// Wenn nicht invertierbar, so wird die Einheitsmatrix angegeben.
Matrix OrE::Math::MatrixInvert(const Matrix& m)
{
	Matrix mSolution;
	Vec4 v, vec[3];
	float fDet;

	fDet = MatrixDet(m);

	if(!fDet)
		return MatrixIdentity();

	// Tausche Vorzeichen in jedem Durchlauf
	float fSignDet = 1.0f/fDet;

	for(int i=0; i<4; i++)
	{
		// Determinantenentwicklung als Kreuzprodukt
		v = Vec4::Cross(m.m[0<i?0:1], m.m[1<i?1:2], m.m[2<i?2:3]);

		// Vorzeichen * Determinante_i / Determinante
		mSolution.m[0][i] = fSignDet * v.x;
		mSolution.m[1][i] = fSignDet * v.y;
		mSolution.m[2][i] = fSignDet * v.z;
		mSolution.m[3][i] = fSignDet * v.w;
		fSignDet *= -1.0f;
	}

	return mSolution;
}

// ******************************************************************************** //
// Transponierte Matrix berechnen
Matrix OrE::Math::MatrixTranspose(const Matrix& m)
{
	// Matrix transponieren
	return Matrix(m.m11, m.m21, m.m31, m.m41,
		          m.m12, m.m22, m.m32, m.m42,
				  m.m13, m.m23, m.m33, m.m43,
				  m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************************** //
// Projektionsmatrix berechnen
Matrix OrE::Math::MatrixProjection(const float fFOV,
							const float fAspect,
							const float fNearPlane,
							const float fFarPlane)
{
	const float s = 1.0f / Tan(fFOV * 0.5f);
	const float fFrustumLengthInv = 1.0f / (fNearPlane - fFarPlane);

	return Matrix(s / fAspect,	0.0f,  0.0f,										0.0f,
				  0.0f,			s,	   0.0f,										0.0f,
				  0.0f,			0.0f, (fNearPlane + fFarPlane)*fFrustumLengthInv,	(2.0f*fNearPlane*fFarPlane)*fFrustumLengthInv,
				  0.0f,			0.0f, -1.0f,										0.0f);
	// DirectX compatible version:
/*	const float s = 1.0f / Tan(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return Matrix(s / fAspect, 0.0f, 0.0f,			0.0f,
		            0.0f,		 s,	   0.0f,			0.0f,
					0.0f,		 0.0f, Q,				1.0f,
					0.0f,		 0.0f, -Q * fNearPlane, 0.0f);*/
}

// ******************************************************************************** //
// Projektionsmatrix berechnen
Matrix OrE::Math::MatrixParallelProjection(const float fWidth,
									const float fHeigh,
									const float fNearPlane,
									const float fFarPlane)
{
	const float s = 2.0f*fNearPlane;
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return Matrix(s / fWidth,	0.0f,		0.0f,				0.0f,
		          0.0f,			s / fHeigh,	0.0f,				0.0f,
				  0.0f,			0.0f,		Q,					1.0f,
				  0.0f,			0.0f,		-Q * fNearPlane,	0.0f);
}

// ******************************************************************************** //
Matrix OrE::Math::MatrixParallelProjection(const float l, const float r, const float b, const float t, const float n, const float f)
{
	const float	r_l = 1.0f/(r - l);
	const float t_b = 1.0f/(t - b);
	const float f_n = 1.0f/(n - f);
	return Matrix( 2.0f*r_l,	0.0f,		0.0f,		0.0f,
				   0.0f,		2.0f*t_b,	0.0f,		0.0f,
				   0.0f,		0.0f,		2.0f*f_n,	0.0f,
				   -(l+r)*r_l,	-(b+t)*t_b,	(f+n)*f_n,	1.0f);
}

// ******************************************************************************** //
// Kameramatrix berechnen
Matrix OrE::Math::MatrixCamera(const Vec3& vPos,
						const Vec3& vLookAt,
						const Vec3& vUp) // = Vec3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	Vec3 vZAxis(Vec3::Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	Vec3 vXAxis(Vec3::Normalize(Vec3::Cross(vUp, vZAxis)));

	// y-Achse berechnen
	Vec3 vYAxis(Vec3::Normalize(Vec3::Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mit ihr multiplizieren
	return MatrixTranslation(-vPos) *
	       Matrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		          vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				  vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
				  0.0f,     0.0f,     0.0f,     1.0f);// *
		   //MatrixTranslation(-vPos);
}

// ******************************************************************************** //
// Kameramatrix erzeugen
Matrix OrE::Math::MatrixCamera(const Vec3& vPos, const Vec3& vDir, const Vec3& vUp, const Vec3& vBidir)
{
	// Easy way - all axis already given
	return MatrixTranslation(-vPos) *
	       Matrix(vBidir.x, vUp.x, vDir.x, 0.0f,
		          vBidir.y, vUp.y, vDir.y, 0.0f,
				  vBidir.z, vUp.z, vDir.z, 0.0f,
				  0.0f,     0.0f,  0.0f,   1.0f);// *
		//   MatrixTranslation(-vPos);
}

// ******************************************************************************** //
// Texturmatrix berechnen
Matrix OrE::Math::MatrixToTex2DMatrix(const Matrix& m)
{
	// Texturmatrix berechnen
	return Matrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************************** //
// Eine Spiegelmatrix an gegebener Ebene berechnen
// TODO: transponieren? TEST
Matrix MatrixMirror(const Plane& p)
{
	return Matrix(-2.0f*p.a*p.a + 1.0f,	-2.0f*p.b*p.a,		-2.0f*p.c*p.a,		  0.0f,
					-2.0f*p.a*p.b,		-2.0f*p.b + 1.0f,	-2.0f*p.c*p.b,		  0.0f,
					-2.0f*p.a*p.c,		-2.0f*p.b*p.c,		-2.0f*p.c*p.c + 1.0f, 0.0f,
					-2.0f*p.a*p.d,		-2.0f*p.b*p.d,		-2.0f*p.c*p.d,		  1.0f);
}


// ******************************************************************************** //
// Löst das Gleichungssystem Ax=v mit dem Gauß-Jordan verfahren
// gibt zurück, ob es eine Eindeutige Lösung gab.
// Die Lösung wird im eingegebenen Lösungsvektor beschrieben
bool OrE::Math::MatrixSolveEquation(Matrix _A, Vec4* _pV_X)
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
					float fSwap0 = _A.m[i][0]; _A.m[i][0] = _A.m[j][0]; _A.m[j][0] = fSwap0;
					float fSwap1 = _A.m[i][1]; _A.m[i][1] = _A.m[j][1]; _A.m[j][1] = fSwap1;
					float fSwap2 = _A.m[i][2]; _A.m[i][2] = _A.m[j][2]; _A.m[j][2] = fSwap2;
					float fSwap3 = _A.m[i][3]; _A.m[i][3] = _A.m[j][3]; _A.m[j][3] = fSwap3;
					float fSwap = _pV_X->c[i]; _pV_X->c[i] = _pV_X->c[j]; _pV_X->c[j] = fSwap;
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

// ******************************************************************************** //
Matrix OrE::Math::MatrixOrthonormal(const Vec3& vNormal)
{
	// Über das Skalarprudukt den 2. Vektor bestimmen.
	Vec3 v2 = (vNormal.x==1.0f)?Vec3(0.0f, 1.0f, 0.0f): Vec3::Normalize((vNormal.y != 0.0f)? Vec3(1.0f, -vNormal.x/vNormal.y, 0.0f) : Vec3(1.0f, 0.0f, -vNormal.x/vNormal.z));
	// 3. Vektor über Kreuzprodukt ermitteln
	Vec3 v3 = Vec3::Cross(vNormal, v2);
	return Matrix(v2.x,		v2.y,		v2.z,		0.0f,
					v3.x,		v3.y,		v3.z,		0.0f,
					vNormal.x,	vNormal.y,	vNormal.z,	0.0f,
					0.0f,		0.0f,		0.0f,		1.0f);
	//return Matrix(vNormal.x,	vNormal.y,	vNormal.z,	0.0f,
	//				v2.x,		v2.y,		v2.z,		0.0f,
	//				v3.x,		v3.y,		v3.z,		0.0f,
	//				0.0f,		0.0f,		0.0f,		1.0f);
}



// ******************************************************************************** //
// ******************************************************************************** //
// ******************************************************************************** //
// Die Funktionen simmulieren alle eine 3x3 matrix

// ******************************************************************************** //
// Translationsmatrix (Verschiebungsmatrix) berechnen
Matrix2x3 OrE::Math::Matrix2x3Translation(const Vec2& v)
{
	return Matrix2x3(1.0f, 0.0f, v.x,
					 0.0f, 1.0f, v.y);
}

// ******************************************************************************** //
// Translationsmatrix (Verschiebungsmatrix) berechnen
Matrix2x3 OrE::Math::Matrix2x3Translation(const float x, const float y)
{
	return Matrix2x3(1.0f, 0.0f, x,
					 0.0f, 1.0f, y);
}

// ******************************************************************************** //
// Rotationsmatrix um die "Z-Achse" berechnen
Matrix2x3 OrE::Math::Matrix2x3Rotation(const float f)
{
	float fCos = Cos(f);
	float fSin = Sin(f);
	return Matrix2x3(fCos, -fSin, 0.0f,
					 fSin , fCos, 0.0f);
}

// ******************************************************************************** //
// Skalierungsmatrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Scaling(const Vec2& v)
{
	return Matrix2x3(v.x , 0.0f, 0.0f,
					 0.0f, v.y , 0.0f);
}

// ******************************************************************************** //
// Skalierungsmatrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Scaling(const float x, const float y)
{
	return Matrix2x3(x   , 0.0f, 0.0f,
					 0.0f, y   , 0.0f);
}

// ******************************************************************************** //
// Skalierungsmatrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Scaling(const float f)
{
	return Matrix2x3(f , 0.0f, 0.0f,
					 0.0f, f , 0.0f);
}

// ******************************************************************************** //
// Liefert eine Achsenmatrix
Matrix2x3 OrE::Math::Matrix2x3Axis(const Vec2& vXAxis, const Vec2& vYAxis)
{
	return Matrix2x3(vXAxis.x, vXAxis.y, 0.0f,
					 vYAxis.x, vYAxis.y, 0.0f);
}

// ******************************************************************************** //
// Invertierte (umgekehrte) Matrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Invert(const Matrix2x3& m)
{
	float fDet = 1.0f/Matrix2x3Det(m);
	// Simuliere 3x3 Matrix mit (0,0,1) in der 3. Zeile
	return Matrix2x3(m.m22*fDet, -m.m12*fDet, (m.m12*m.m23-m.m13*m.m22)*fDet,
					 -m.m21*fDet, m.m11*fDet, (m.m13*m.m21-m.m11*m.m23)*fDet);
}

// ******************************************************************************** //
// Scherungs Matrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Transvection(const Vec2& v)
{
	return Matrix2x3(1.0f, v.x , 0.0f,
					 v.y , 1.0f, 0.0f);
}

// ******************************************************************************** //
// Scherungs Matrix berechnen
Matrix2x3 OrE::Math::Matrix2x3Transvection(const float x, const float y)
{
	return Matrix2x3(1.0f, x   , 0.0f,
					 y   , 1.0f, 0.0f);
}

// ******************************************************************************** //
// Determinante berechnen (Laplaceches Entwicklungsschema)
float OrE::Math::Matrix2x3Det(const Matrix2x3& m)
{
	return m.m11 * m.m22 -
           m.m12 * m.m21;
}

// *************************************EOF**************************************** //