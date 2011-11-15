/*************************************************************************

	OrMatrix.h
	==========
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Matrizenrechnung

	Autor: Johannes Jendersie
	Verwendete Quellen: TriBase-Engine

*************************************************************************/

#pragma once

// Verwendete Symbole vordefinieren
class OrMatrix;
OrMatrix OrMatrixInvert(const OrMatrix& m);
inline OrMatrix operator * (const OrMatrix& a, const OrMatrix& b);

// ******************************************************************** //
// Idee: zur Renderbeschleunigung
// Matriezen werden beim Erzeugen mit IDs ausgestattet. Das hat zur Folge,
// dass wahrscheinlich identische Matrizen als solche ermittelt werden
// können.
// Die Vergleichsoperatoren beziehen sich jedoch nur auf die
// mathematische Gleichheit.
//static dword Or_MatrixIDCounter = 1;

//struct float4 {float x,y,z,w;};

// ******************************************************************** //
// Die Matrixklasse
class OrMatrix
{
public:
	// Variablen
	union
	{
		struct
		{
			float m11, m12, m13, m14,	// Elemente der Matrix
				  m21, m22, m23, m24,
				  m31, m32, m33, m34,
				  m41, m42, m43, m44;
		};

		float		m[4][4];			// Zweidimensionales Array der Elemente
		float		n[16];				// Eindimensionales Array der Elemente
		//float4		v[4];				// Array mit 4 4D-Vektoren (Zeilen)
	};

//	dword dwMatrixID;

	// Konstruktoren
	OrMatrix() {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	OrMatrix(const OrMatrix& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
                                  m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
								  m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
								  m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	OrMatrix(float _m11, float _m12, float _m13, float _m14,
			 float _m21, float _m22, float _m23, float _m24,
			 float _m31, float _m32, float _m33, float _m34,
			 float _m41, float _m42, float _m43, float _m44) : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
			                                                   m21(_m21), m22(_m22), m23(_m23), m24(_m24),
															   m31(_m31), m32(_m32), m33(_m33), m34(_m34),
															   m41(_m41), m42(_m42), m43(_m43), m44(_m44) {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	OrMatrix(const float* pfValue);

	// Casting-Opeatoren
	operator float* ()					{return (float*)(n);}
	operator const float* () const		{return (float*)(n);}

	// Zugriffsoperatoren
	float& operator () (int iRow, int iColumn) {return m[iRow][iColumn];}
	float operator () (int iRow, int iColumn) const {return m[iRow][iColumn];}

	// Zuweisungsoperatoren
	OrMatrix& operator = (const OrMatrix& m);
	
	OrMatrix& operator += (const OrMatrix& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	OrMatrix& operator -= (const OrMatrix& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	OrMatrix& operator *= (const OrMatrix& m)
	{
		return *this = *this*m;/*OrMatrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
								m.m12 * m11 + m.m22 * m12 + m.m32 * m13 + m.m42 * m14,
								m.m13 * m11 + m.m23 * m12 + m.m33 * m13 + m.m43 * m14,
								m.m14 * m11 + m.m24 * m12 + m.m34 * m13 + m.m44 * m14,
								m.m11 * m21 + m.m21 * m22 + m.m31 * m23 + m.m41 * m24,
								m.m12 * m21 + m.m22 * m22 + m.m32 * m23 + m.m42 * m24,
								m.m13 * m21 + m.m23 * m22 + m.m33 * m23 + m.m43 * m24,
								m.m14 * m21 + m.m24 * m22 + m.m34 * m23 + m.m44 * m24,
								m.m11 * m31 + m.m21 * m32 + m.m31 * m33 + m.m41 * m34,
								m.m12 * m31 + m.m22 * m32 + m.m32 * m33 + m.m42 * m34,
								m.m13 * m31 + m.m23 * m32 + m.m33 * m33 + m.m43 * m34,
								m.m14 * m31 + m.m24 * m32 + m.m34 * m33 + m.m44 * m34,
								m.m11 * m41 + m.m21 * m42 + m.m31 * m43 + m.m41 * m44,
								m.m12 * m41 + m.m22 * m42 + m.m32 * m43 + m.m42 * m44,
								m.m13 * m41 + m.m23 * m42 + m.m33 * m43 + m.m43 * m44,
								m.m14 * m41 + m.m24 * m42 + m.m34 * m43 + m.m44 * m44);*/
	}

	OrMatrix& operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}
	
	OrMatrix& operator /= (const OrMatrix& m)
	{
		return *this *= OrMatrixInvert(m);
	}

	OrMatrix& operator /= (float f)
	{
		f = 1/f;
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}

	// Unäre Operatoren
	OrMatrix operator + () const
	{
		return *this;
	}

	OrMatrix operator - () const
	{
		return OrMatrix(-m11, -m12, -m13, -m14,
						-m21, -m22, -m23, -m24,
						-m31, -m32, -m33, -m34,
						-m41, -m42, -m43, -m44);
	}
};

typedef OrMatrix* OrMatrixP;

// Arithmetische Operatoren
inline OrMatrix operator + (const OrMatrix& a, const OrMatrix& b)	{return OrMatrix(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34, a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);}
inline OrMatrix operator - (const OrMatrix& a, const OrMatrix& b)	{return OrMatrix(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34, a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);}
inline OrMatrix operator - (const OrMatrix& m)						{return OrMatrix(-m.m11, -m.m12, -m.m13, -m.m14, -m.m21, -m.m22, -m.m23, -m.m24, -m.m31, -m.m32, -m.m33, -m.m34, -m.m41, -m.m42, -m.m43, -m.m44);}

inline OrMatrix operator * (const OrMatrix& a,
							const OrMatrix& b)
{
	return OrMatrix(b.m11 * a.m11 + b.m21 * a.m12 + b.m31 * a.m13 + b.m41 * a.m14,
					b.m12 * a.m11 + b.m22 * a.m12 + b.m32 * a.m13 + b.m42 * a.m14,
					b.m13 * a.m11 + b.m23 * a.m12 + b.m33 * a.m13 + b.m43 * a.m14,
					b.m14 * a.m11 + b.m24 * a.m12 + b.m34 * a.m13 + b.m44 * a.m14,
					b.m11 * a.m21 + b.m21 * a.m22 + b.m31 * a.m23 + b.m41 * a.m24,
					b.m12 * a.m21 + b.m22 * a.m22 + b.m32 * a.m23 + b.m42 * a.m24,
					b.m13 * a.m21 + b.m23 * a.m22 + b.m33 * a.m23 + b.m43 * a.m24,
					b.m14 * a.m21 + b.m24 * a.m22 + b.m34 * a.m23 + b.m44 * a.m24,
					b.m11 * a.m31 + b.m21 * a.m32 + b.m31 * a.m33 + b.m41 * a.m34,
					b.m12 * a.m31 + b.m22 * a.m32 + b.m32 * a.m33 + b.m42 * a.m34,
					b.m13 * a.m31 + b.m23 * a.m32 + b.m33 * a.m33 + b.m43 * a.m34,
					b.m14 * a.m31 + b.m24 * a.m32 + b.m34 * a.m33 + b.m44 * a.m34,
					b.m11 * a.m41 + b.m21 * a.m42 + b.m31 * a.m43 + b.m41 * a.m44,
					b.m12 * a.m41 + b.m22 * a.m42 + b.m32 * a.m43 + b.m42 * a.m44,
					b.m13 * a.m41 + b.m23 * a.m42 + b.m33 * a.m43 + b.m43 * a.m44,
					b.m14 * a.m41 + b.m24 * a.m42 + b.m34 * a.m43 + b.m44 * a.m44);
}

inline OrMatrix operator * (const OrMatrix& m,
							const float f)
{
	return OrMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline OrMatrix operator * (const float f,
							const OrMatrix& m)
{
	return OrMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline OrVector3 operator * (const OrVector3& v,
							const OrMatrix& m)
{
	return OrVec3TransformCoords(v, m);
}

inline OrVector3 operator * (const OrMatrix& m,
							const OrVector3& v)
{
	return OrVec3TransformCoords(v, m);
}

inline OrMatrix operator / (const OrMatrix& a, const OrMatrix& b) {return a * OrMatrixInvert(b);}

inline OrMatrix operator / (const OrMatrix& m,
							float f)
{
	f = 1/f;
	return OrMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

// Vergleichsoperatoren
#define ORMATRIX_EPSILON	0.001f
inline bool operator == (const OrMatrix& a,
						 const OrMatrix& b)
{
	if(OrAbsf(a.m11 - b.m11) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m12 - b.m12) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m13 - b.m13) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m14 - b.m14) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m21 - b.m21) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m22 - b.m22) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m23 - b.m23) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m24 - b.m24) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m31 - b.m31) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m32 - b.m32) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m33 - b.m33) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m34 - b.m34) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m41 - b.m41) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m42 - b.m42) > ORMATRIX_EPSILON) return false;
	if(OrAbsf(a.m43 - b.m43) > ORMATRIX_EPSILON) return false;
	return (OrAbsf(a.m44 - b.m44) <= ORMATRIX_EPSILON);
}

// Vergleichsoperatoren
inline bool operator != (const OrMatrix& a,
						 const OrMatrix& b)
{
	if(OrAbsf(a.m11 - b.m11) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m12 - b.m12) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m13 - b.m13) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m14 - b.m14) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m21 - b.m21) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m22 - b.m22) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m23 - b.m23) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m24 - b.m24) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m31 - b.m31) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m32 - b.m32) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m33 - b.m33) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m34 - b.m34) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m41 - b.m41) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m42 - b.m42) > ORMATRIX_EPSILON) return true;
	if(OrAbsf(a.m43 - b.m43) > ORMATRIX_EPSILON) return true;
	return (OrAbsf(a.m44 - b.m44) > ORMATRIX_EPSILON);
}

// ******************************************************************** //
// Funktionen deklarieren
inline	OrMatrix	OrMatrixIdentity() {return OrMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}						// Identitätsmatrix liefern
		OrMatrix	OrMatrixTranslation(const OrVector3& v);																													// Translationsmatrix (Verschiebungsmatrix) berechnen
		OrMatrix	OrMatrixTranslation(const float x, const float y, const float z);																							// Translationsmatrix (Verschiebungsmatrix) berechnen
		OrMatrix	OrMatrixRotationX(const float f);																															// Rotationsmatrix um die X-Achse berechnen
		OrMatrix	OrMatrixRotationY(const float f);																															// Rotationsmatrix um die Y-Achse berechnen
		OrMatrix	OrMatrixRotationZ(const float f);																															// Rotationsmatrix um die Z-Achse berechnen
		OrMatrix	OrMatrixRotation(const float x, const float y, const float z);																								// Rotiert um alle drei Achsen
		OrMatrix	OrMatrixRotation(const OrVector3& v);																														// Rotiert um alle drei Achsen
		OrMatrix	OrMatrixRotation_Translatation(const OrVector3& vR, const OrVector3& vP);																					// Direkte Berechnung von OrMatrixRotation*OrMatrixTranslation
		OrMatrix	OrMatrixRotationAxis(const OrVector3& v, const float f);																									// Rotationsmatrix um eine beliebige Achse berechnen
		OrMatrix	OrMatrixScaling(const OrVector3& v);																														// Skalierungsmatrix berechnen
		OrMatrix	OrMatrixScaling(const float f);																																// Skalierungsmatrix berechnen
		OrMatrix	OrMatrixAxis(const OrVector3& vXAxis, const OrVector3& vYAxis, const OrVector3& vZAxis);																	// Liefert eine Achsenmatrix
		float		OrMatrixDet3(const OrMatrix& m);																															// Determinante berechnen (Obere linke 3x3 Matrix)
		float		OrMatrixDet(const OrMatrix& m);																																// Determinante berechnen (Laplaceches Entwicklungsschema)
		OrMatrix	OrMatrixInvert(const OrMatrix& m);																															// Invertierte (umgekehrte) Matrix berechnen
		OrMatrix	OrMatrixTranspose(const OrMatrix& m);																														// Transponierte Matrix berechnen
		OrMatrix	OrMatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);													// Projektionsmatrix berechnen
		OrMatrix	OrMatrixParallelProjection(const float fWidth, const float fHeigh, const float fNearPlane, const float fFarPlane);											// Projektionsmatrix berechnen
		OrMatrix	OrMatrixCamera(const OrVector3& vPos, const OrVector3& vLookAt, const OrVector3& vUp = OrVector3(0.0f, 1.0f, 0.0f));										// Kameramatrix erzeugen
		OrMatrix	OrMatrixToTex2DMatrix(const OrMatrix& m);																													// In Texturmatrix umwandeln
//		OrMatrix	OrMatrixMirror(const OrPlane& p);																															// Eine Spiegelmatrix an gegebener Ebene berechnen
//		OrMatrix	OrMatrixMirror_Normalize(const OrPlane& p);																													// Eine Spiegelmatrix an gegebener Ebene (wird normalisiert) berechnen
		bool		OrMatrixSolveEquation(OrMatrix _A, OrVector4* _pV_X);																										// Löst das Gleichungssystem Ax=v mit dem Gauß-Jordan verfahren
		OrMatrix	OrMatrixOrthonormal(const OrVector3& vNormal);

// ******************************************************************** //