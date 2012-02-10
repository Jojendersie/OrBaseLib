// ******************************************************************************** //
// OrVector3.h																		//
// ===========																		//
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

#pragma once

namespace OrE {
namespace Math {

// ******************************************************************************** //
// Die 3D-Vektor-Klasse
class Vector3
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
			float w;
		};

		float		c[3];		// Array der Koordinaten
	};

	// Konstruktoren
	Vector3()																					{}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z)											{}
	Vector3(const float f) : x(f), y(f), z(f)													{}
	Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)				{}
	Vector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}

	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}

	// Zuweisungsoperatoren
	Vector3& operator = (const Vector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	Vector3& operator = (const float f)		{x = f; y = f; z = f; return *this;}
	Vector3& operator += (const Vector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	Vector3& operator -= (const Vector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vector3& operator *= (const Vector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	Vector3& operator *= (const float f)	{x *= f; y *= f; z *= f; return *this;}
	Vector3& operator /= (const Vector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	Vector3& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; return *this;}
};

// Arithmetische Operatoren
inline Vector3 operator + (const Vector3& a, const Vector3& b)	{return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline Vector3 operator - (const Vector3& a, const Vector3& b)	{return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline Vector3 operator - (const Vector3& v)					{return Vector3(-v.x, -v.y, -v.z);}
inline Vector3 operator * (const Vector3& a, const Vector3& b)	{return Vector3(a.x*b.x, a.y*b.y, a.z*b.z);}
inline Vector3 operator * (const Vector3& v, const float f)		{return Vector3(v.x * f, v.y * f, v.z * f);}
inline Vector3 operator * (const float f, const Vector3& v)		{return Vector3(v.x * f, v.y * f, v.z * f);}
inline Vector3 operator / (const Vector3& a, const Vector3& b)	{return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);}
inline Vector3 operator / (const Vector3& v, float f)			{f = 1/f; return Vector3(v.x * f, v.y * f, v.z * f);}

// ******************************************************************************** //
// Vergleichsoperatoren
inline bool operator == (const Vector3& a, const Vector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; return a.z == b.z;}
inline bool operator != (const Vector3& a, const Vector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; return a.z != b.z;}

// ******************************************************************************** //
// Funktionen deklarieren
inline float	Vec3Length(const Vector3& v)													{return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float	Vec3LengthSq(const Vector3& v)													{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline float	Vec3LengthInv(const Vector3& v)													{return InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline Vector3	Vec3Normalize(const Vector3& v)													{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline Vector3	Vec3NormalizeEx(const Vector3& v)												{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline Vector3	Vec3Cross(const Vector3& v1, const Vector3& v2)									{return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float	Vec3CrossX(const float v1y, const float v1z, const float v2y, const float v2z)	{return v1y * v2z - v1z * v2y;}			// Nur die X Coordinate berechnen
inline float	Vec3CrossY(const float v1x, const float v1z, const float v2x, const float v2z)	{return v1z * v2x - v1x * v2z;}			// Nur die Y Coordinate berechnen
inline float	Vec3CrossZ(const float v1x, const float v1y, const float v2x, const float v2y)	{return v1x * v2y - v1y * v2x;}			// Nur die Z Coordinate berechnen
inline float	Vec3Dot(const Vector3& v1, const Vector3& v2)									{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float	Vec3Angle(const Vector3& v1, const Vector3& v2)									{return Cos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) * InvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
inline Vector3	Vec3LrpNormal(const Vector3& v1, const Vector3& v2, const float p)				{return Vec3NormalizeEx(v1 + p * (v2 - v1));}
inline Vector3	Vec3Min(const Vector3& v1, const Vector3& v2)									{return Vector3(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z));}
inline Vector3	Vec3Max(const Vector3& v1, const Vector3& v2)									{return Vector3(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z));}
inline Vector3	Vec3Lrp(const Vector3& v1, const Vector3& v2, const float f)					{return Vector3(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y), v1.z+f*(v2.z-v1.z));}
Vector3			Vec3Random();

Vector3	Vec3TransformCoords(const Vector3& v, const Matrix& m, float* const pfOutW);																																									// 3D-Vektor mit Matrix multiplizieren
Vector3	Vec3TransformCoords(const Vector3& v, const Matrix& m);																																														// 3D-Vektor mit Matrix multiplizieren
Vector3	Vec3TransformDirection(const Vector3& v, const Matrix& m);																																													// 3D-Richtungsvektor mit Matrix multiplizieren
/*Vector3	Vec3TransformNormal(const Vector3& v, const Matrix& m);																																														// 3D-Normalenvektor mit Matrix multiplizieren
Vector3	Vec3TransformNormal_TranspInv(const Vector3& v, const Matrix& m);																																												// 3D-Normalenvektor mit Matrix multiplizieren
*/

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //