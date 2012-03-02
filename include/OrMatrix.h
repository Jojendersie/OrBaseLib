// ******************************************************************************** //
// OrMatrix.h																		//
// ==========																		//
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
// Source: DirectX 9.0, some wikis													//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Math {

// Verwendete Symbole vordefinieren
class Matrix;
class Matrix2x3;
Matrix MatrixInvert(const Matrix& m);
Matrix2x3 Matrix2x3Invert(const Matrix2x3& m);
inline Matrix operator * (const Matrix& a, const Matrix& b);

// ******************************************************************************** //
// Idee: zur Renderbeschleunigung
// Matriezen werden beim Erzeugen mit IDs ausgestattet. Das hat zur Folge,
// dass wahrscheinlich identische Matrizen als solche ermittelt werden
// können.
// Die Vergleichsoperatoren beziehen sich jedoch nur auf die
// mathematische Gleichheit.
//static dword Or_MatrixIDCounter = 1;

// ******************************************************************************** //
// Die 4D - Matrixklasse (Für 3D Transformationen)
class Matrix
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
	};

//	dword dwMatrixID;

	// Konstruktoren
	Matrix() {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	Matrix(const Matrix& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
                                  m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
								  m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
								  m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	Matrix(float _m11, float _m12, float _m13, float _m14,
			 float _m21, float _m22, float _m23, float _m24,
			 float _m31, float _m32, float _m33, float _m34,
			 float _m41, float _m42, float _m43, float _m44) : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
			                                                   m21(_m21), m22(_m22), m23(_m23), m24(_m24),
															   m31(_m31), m32(_m32), m33(_m33), m34(_m34),
															   m41(_m41), m42(_m42), m43(_m43), m44(_m44) {/*dwMatrixID = Or_MatrixIDCounter++;*/}

	Matrix(const float* pfValue);

	// Casting-Opeatoren
	operator float* ()					{return (float*)(n);}
	operator const float* () const		{return (float*)(n);}

	// Zugriffsoperatoren
	float& operator () (int iRow, int iColumn) {return m[iRow][iColumn];}
	float operator () (int iRow, int iColumn) const {return m[iRow][iColumn];}

	// Zuweisungsoperatoren
	Matrix& operator = (const Matrix& m);
	
	Matrix& operator += (const Matrix& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	Matrix& operator -= (const Matrix& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	Matrix& operator *= (const Matrix& m)
	{
		return *this = *this*m;/*Matrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
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

	Matrix& operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}
	
	Matrix& operator /= (const Matrix& m)
	{
		return *this *= MatrixInvert(m);
	}

	Matrix& operator /= (float f)
	{
		f = 1/f;
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}

	// Unäre Operatoren
	Matrix operator + () const
	{
		return *this;
	}

	Matrix operator - () const
	{
		return Matrix(-m11, -m12, -m13, -m14,
						-m21, -m22, -m23, -m24,
						-m31, -m32, -m33, -m34,
						-m41, -m42, -m43, -m44);
	}
};

typedef Matrix* MatrixP;

// Arithmetische Operatoren
inline Matrix operator + (const Matrix& a, const Matrix& b)	{return Matrix(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34, a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);}
inline Matrix operator - (const Matrix& a, const Matrix& b)	{return Matrix(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34, a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);}
inline Matrix operator - (const Matrix& m)						{return Matrix(-m.m11, -m.m12, -m.m13, -m.m14, -m.m21, -m.m22, -m.m23, -m.m24, -m.m31, -m.m32, -m.m33, -m.m34, -m.m41, -m.m42, -m.m43, -m.m44);}

inline Matrix operator * (const Matrix& a,
							const Matrix& b)
{
	return Matrix(b.m11 * a.m11 + b.m21 * a.m12 + b.m31 * a.m13 + b.m41 * a.m14,
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

inline Matrix operator * (const Matrix& m,
							const float f)
{
	return Matrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline Matrix operator * (const float f,
							const Matrix& m)
{
	return Matrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline Vec3 operator * (const Vec3& v,
							const Matrix& m)
{
	return Vec3TransformCoords(v, m);
}

inline Vec3 operator * (const Matrix& m,
							const Vec3& v)
{
	return Vec3TransformCoords(v, m);
}

inline Matrix operator / (const Matrix& a, const Matrix& b) {return a * MatrixInvert(b);}

inline Matrix operator / (const Matrix& m,
							float f)
{
	f = 1/f;
	return Matrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

// ******************************************************************************** //
// Vergleichsoperatoren
#define Matrix_EPSILON	0.001f
inline bool operator == (const Matrix& a,
						 const Matrix& b)
{
	if(Abs(a.m11 - b.m11) > Matrix_EPSILON) return false;
	if(Abs(a.m12 - b.m12) > Matrix_EPSILON) return false;
	if(Abs(a.m13 - b.m13) > Matrix_EPSILON) return false;
	if(Abs(a.m14 - b.m14) > Matrix_EPSILON) return false;
	if(Abs(a.m21 - b.m21) > Matrix_EPSILON) return false;
	if(Abs(a.m22 - b.m22) > Matrix_EPSILON) return false;
	if(Abs(a.m23 - b.m23) > Matrix_EPSILON) return false;
	if(Abs(a.m24 - b.m24) > Matrix_EPSILON) return false;
	if(Abs(a.m31 - b.m31) > Matrix_EPSILON) return false;
	if(Abs(a.m32 - b.m32) > Matrix_EPSILON) return false;
	if(Abs(a.m33 - b.m33) > Matrix_EPSILON) return false;
	if(Abs(a.m34 - b.m34) > Matrix_EPSILON) return false;
	if(Abs(a.m41 - b.m41) > Matrix_EPSILON) return false;
	if(Abs(a.m42 - b.m42) > Matrix_EPSILON) return false;
	if(Abs(a.m43 - b.m43) > Matrix_EPSILON) return false;
	return (Abs(a.m44 - b.m44) <= Matrix_EPSILON);
}

inline bool operator != (const Matrix& a,
						 const Matrix& b)
{
	if(Abs(a.m11 - b.m11) > Matrix_EPSILON) return true;
	if(Abs(a.m12 - b.m12) > Matrix_EPSILON) return true;
	if(Abs(a.m13 - b.m13) > Matrix_EPSILON) return true;
	if(Abs(a.m14 - b.m14) > Matrix_EPSILON) return true;
	if(Abs(a.m21 - b.m21) > Matrix_EPSILON) return true;
	if(Abs(a.m22 - b.m22) > Matrix_EPSILON) return true;
	if(Abs(a.m23 - b.m23) > Matrix_EPSILON) return true;
	if(Abs(a.m24 - b.m24) > Matrix_EPSILON) return true;
	if(Abs(a.m31 - b.m31) > Matrix_EPSILON) return true;
	if(Abs(a.m32 - b.m32) > Matrix_EPSILON) return true;
	if(Abs(a.m33 - b.m33) > Matrix_EPSILON) return true;
	if(Abs(a.m34 - b.m34) > Matrix_EPSILON) return true;
	if(Abs(a.m41 - b.m41) > Matrix_EPSILON) return true;
	if(Abs(a.m42 - b.m42) > Matrix_EPSILON) return true;
	if(Abs(a.m43 - b.m43) > Matrix_EPSILON) return true;
	return (Abs(a.m44 - b.m44) > Matrix_EPSILON);
}

// ******************************************************************************** //
// Funktionen deklarieren
inline	Matrix	MatrixIdentity() {return Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}				// Identitätsmatrix liefern
		Matrix	MatrixTranslation(const Vec3& v);																											// Translationsmatrix (Verschiebungsmatrix) berechnen
		Matrix	MatrixTranslation(const float x, const float y, const float z);																					// Translationsmatrix (Verschiebungsmatrix) berechnen
		Matrix	MatrixRotationX(const float f);																													// Rotationsmatrix um die X-Achse berechnen
		Matrix	MatrixRotationY(const float f);																													// Rotationsmatrix um die Y-Achse berechnen
		Matrix	MatrixRotationZ(const float f);																													// Rotationsmatrix um die Z-Achse berechnen
		Matrix	MatrixRotation(const float x, const float y, const float z);																					// Rotiert um alle drei Achsen
		Matrix	MatrixRotation(const Vec3& v);																												// Rotiert um alle drei Achsen
		Matrix	MatrixRotation_Translatation(const Vec3& vR, const Vec3& vP);																				// Direkte Berechnung von MatrixRotation*MatrixTranslation
		Matrix	MatrixRotationAxis(const Vec3& v, const float f);																							// Rotationsmatrix um eine beliebige Achse berechnen
		Matrix	MatrixScaling(const Vec3& v);																												// Skalierungsmatrix berechnen
		Matrix	MatrixScaling(const float f);																													// Skalierungsmatrix berechnen
		Matrix	MatrixAxis(const Vec3& vXAxis, const Vec3& vYAxis, const Vec3& vZAxis);																// Liefert eine Achsenmatrix
		float	MatrixDet3(const Matrix& m);																													// Determinante berechnen (Obere linke 3x3 Matrix)
		float	MatrixDet(const Matrix& m);																														// Determinante berechnen (Laplaceches Entwicklungsschema)
		Matrix	MatrixInvert(const Matrix& m);																													// Invertierte (umgekehrte) Matrix berechnen
		Matrix	MatrixTranspose(const Matrix& m);																												// Transponierte Matrix berechnen
		Matrix	MatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);											// Projektionsmatrix berechnen
		Matrix	MatrixParallelProjection(const float fWidth, const float fHeigh, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
		Matrix	MatrixCamera(const Vec3& vPos, const Vec3& vLookAt, const Vec3& vUp = Vec3(0.0f, 1.0f, 0.0f));										// Kameramatrix erzeugen
		Matrix	MatrixCamera(const Vec3& vPos, const Vec3& vDir, const Vec3& vUp, const Vec3& vBidir);												// Kameramatrix erzeugen
		Matrix	MatrixToTex2DMatrix(const Matrix& m);																											// In Texturmatrix umwandeln
//		Matrix	MatrixMirror(const OrPlane& p);																													// Eine Spiegelmatrix an gegebener Ebene berechnen
//		Matrix	MatrixMirror_Normalize(const OrPlane& p);																										// Eine Spiegelmatrix an gegebener Ebene (wird normalisiert) berechnen
		bool	MatrixSolveEquation(Matrix _A, Vec4* _pV_X);																									// Löst das Gleichungssystem Ax=v mit dem Gauß-Jordan verfahren
		Matrix	MatrixOrthonormal(const Vec3& vNormal);
		Matrix	MatrixTransvection(const Vec3& v);																											// Scherungsmatrix berechnen TODO


// ******************************************************************************** //
// 2x3 Matrixklasse für 2D Transformationen
// Die Transfornation erfolgt durch Matrix*(u,v,1)' (2x3 * 3x1 = 2x1)
class Matrix2x3
{
public:
	// Variablen
	union
	{
		struct
		{
			float m11, m12, m13,		// Elemente der Matrix
				  m21, m22, m23;
		};

		float		m[2][3];			// Zweidimensionales Array der Elemente
		float		n[6];				// Eindimensionales Array der Elemente
	};

	// Konstruktoren
	Matrix2x3() {}

	Matrix2x3(const Matrix2x3& m) : m11(m.m11), m12(m.m12), m13(m.m13),
										m21(m.m21), m22(m.m22), m23(m.m23) {}

	Matrix2x3(float _m11, float _m12, float _m13,
			    float _m21, float _m22, float _m23) : m11(_m11), m12(_m12), m13(_m13),
													  m21(_m21), m22(_m22), m23(_m23) {}

	Matrix2x3(const float* pfValue) : m11(pfValue[0]), m12(pfValue[1]), m13(pfValue[2]),
									    m21(pfValue[3]), m22(pfValue[4]), m23(pfValue[5])	{}

	// Casting-Opeatoren
	operator float* ()					{return (float*)(n);}
	operator const float* () const		{return (float*)(n);}

	// Zugriffsoperatoren
	float& operator () (int iRow, int iColumn) {return m[iRow][iColumn];}
	float operator () (int iRow, int iColumn) const {return m[iRow][iColumn];}

	// Zuweisungsoperatoren
	Matrix2x3& operator = (const Matrix2x3& m)
	{
		m11 = m.m11; m12 = m.m12; m13 = m.m13;
		m21 = m.m21; m22 = m.m22; m23 = m.m23;
		return *this;
	}
	
	Matrix2x3& operator += (const Matrix2x3& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13;
		m21 += m.m21; m22 += m.m22; m23 += m.m23;
		return *this;
	}

	Matrix2x3& operator -= (const Matrix2x3& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23;
		return *this;
	}

	Matrix2x3& operator *= (const Matrix2x3& m)
	{
		return *this = Matrix2x3(m.m11 * m11 + m.m21 * m12,
								   m.m12 * m11 + m.m22 * m12,
								   m.m13 * m11 + m.m23 * m12 + m13,
								   m.m11 * m21 + m.m21 * m22,
								   m.m12 * m21 + m.m22 * m22,
								   m.m13 * m21 + m.m23 * m22 + m23);
	}

	Matrix2x3& operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f;
		m21 *= f; m22 *= f; m23 *= f;
		return *this;
	}
	
	Matrix2x3& operator /= (const Matrix2x3& m)
	{
		return *this *= Matrix2x3Invert(m);
	}

	Matrix2x3& operator /= (float f)
	{
		f = 1/f;
		m11 *= f; m12 *= f; m13 *= f;
		m21 *= f; m22 *= f; m23 *= f;
		return *this;
	}

	// Unäre Operatoren
	Matrix2x3 operator + () const
	{
		return *this;
	}

	Matrix2x3 operator - () const
	{
		return Matrix2x3(-m11, -m12, -m13,
						   -m21, -m22, -m23);
	}
};

typedef Matrix2x3* Matrix2x3P;


// ******************************************************************************** //
// Arithmetische Operatoren
inline Matrix2x3 operator + (const Matrix2x3& a, const Matrix2x3& b)	{return Matrix2x3(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23);}
inline Matrix2x3 operator - (const Matrix2x3& a, const Matrix2x3& b)	{return Matrix2x3(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23);}
inline Matrix2x3 operator - (const Matrix2x3& m)						{return Matrix2x3(-m.m11, -m.m12, -m.m13, -m.m21, -m.m22, -m.m23);}

inline Matrix2x3 operator * (const Matrix2x3& a,
							   const Matrix2x3& b)
{
	return Matrix2x3(b.m11 * a.m11 + b.m21 * a.m12,
					   b.m12 * a.m11 + b.m22 * a.m12,
					   b.m13 * a.m11 + b.m23 * a.m12 + a.m13,
					   b.m11 * a.m21 + b.m21 * a.m22,
					   b.m12 * a.m21 + b.m22 * a.m22,
					   b.m13 * a.m21 + b.m23 * a.m22 + a.m23);
}

inline Matrix2x3 operator * (const Matrix2x3& m,
							   const float f)
{
	return Matrix2x3(m.m11 * f, m.m12 * f, m.m13 * f,
			           m.m21 * f, m.m22 * f, m.m23 * f);
}

inline Matrix2x3 operator * (const float f,
							   const Matrix2x3& m)
{
	return Matrix2x3(m.m11 * f, m.m12 * f, m.m13 * f,
			           m.m21 * f, m.m22 * f, m.m23 * f);
}

inline Vec2 operator * (const Vec2& v,
							 const Matrix2x3& m)
{
	return Vec2Transform(v, m);
}

inline Vec2 operator * (const Matrix2x3& m,
							 const Vec2& v)
{
	return Vec2Transform(v, m);
}

inline Matrix2x3 operator / (const Matrix2x3& a, const Matrix2x3& b) {return a * Matrix2x3Invert(b);}

inline Matrix2x3 operator / (const Matrix2x3& m,
							   float f)
{
	f = 1/f;
	return Matrix2x3(m.m11 * f, m.m12 * f, m.m13 * f,
			           m.m21 * f, m.m22 * f, m.m23 * f);
}

// ******************************************************************************** //
// Vergleichsoperatoren
#define Matrix_EPSILON	0.001f
inline bool operator == (const Matrix2x3& a,
						 const Matrix2x3& b)
{
	if(Abs(a.m11 - b.m11) > Matrix_EPSILON) return false;
	if(Abs(a.m12 - b.m12) > Matrix_EPSILON) return false;
	if(Abs(a.m13 - b.m13) > Matrix_EPSILON) return false;
	if(Abs(a.m21 - b.m21) > Matrix_EPSILON) return false;
	if(Abs(a.m22 - b.m22) > Matrix_EPSILON) return false;
	if(Abs(a.m23 - b.m23) > Matrix_EPSILON) return false;
	return (Abs(a.m23 - b.m23) <= Matrix_EPSILON);
}

inline bool operator != (const Matrix2x3& a,
						 const Matrix2x3& b)
{
	if(Abs(a.m11 - b.m11) > Matrix_EPSILON) return true;
	if(Abs(a.m12 - b.m12) > Matrix_EPSILON) return true;
	if(Abs(a.m13 - b.m13) > Matrix_EPSILON) return true;
	if(Abs(a.m21 - b.m21) > Matrix_EPSILON) return true;
	if(Abs(a.m22 - b.m22) > Matrix_EPSILON) return true;
	if(Abs(a.m23 - b.m23) > Matrix_EPSILON) return true;
	return (Abs(a.m23 - b.m23) > Matrix_EPSILON);
}

// ******************************************************************************** //
// Die Funktionen simmulieren alle eine 3x3 matrix
inline	Matrix2x3	Matrix2x3Identity() {return Matrix2x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);}																				// Identitätsmatrix liefern
		Matrix2x3	Matrix2x3Translation(const Vec2& v);																													// Translationsmatrix (Verschiebungsmatrix) berechnen
		Matrix2x3	Matrix2x3Translation(const float x, const float y);																										// Translationsmatrix (Verschiebungsmatrix) berechnen
		Matrix2x3	Matrix2x3Rotation(const float f);																															// Rotationsmatrix um die "Z-Achse" berechnen
		Matrix2x3	Matrix2x3Scaling(const Vec2& v);																														// Skalierungsmatrix berechnen
		Matrix2x3	Matrix2x3Scaling(const float x, const float y);																											// Skalierungsmatrix berechnen
		Matrix2x3	Matrix2x3Scaling(const float f);																															// Skalierungsmatrix berechnen
		Matrix2x3	Matrix2x3Axis(const Vec2& vXAxis, const Vec2& vYAxis);																							// Liefert eine Achsenmatrix
		Matrix2x3	Matrix2x3Invert(const Matrix2x3& m);																													// Invertierte (umgekehrte) Matrix berechnen
		Matrix2x3	Matrix2x3Transvection(const Vec2& v);																												// Scherungs Matrix berechnen
		Matrix2x3	Matrix2x3Transvection(const float x, const float y);																										// Scherungs Matrix berechnen
		float		Matrix2x3Det(const Matrix2x3& m);																														// Determinante berechnen (Laplaceches Entwicklungsschema)


}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //