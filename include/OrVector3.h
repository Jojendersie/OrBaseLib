// ******************************************************************************** //
// OrVec3.h																		//
// ===========																		//
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

#pragma once

namespace OrE {
namespace Math {

// ******************************************************************************** //
// Die 3D-Vektor-Klasse
class Vec3
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
	Vec3()																						{}
	Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z)												{}
	Vec3(const Vec2& v, float f) : x(v.x), y(v.y), z(f)											{}
	Vec3(float f, const Vec2& v) : x(f), y(v.x), z(v.y)											{}
	Vec3(const float f) : x(f), y(f), z(f)														{}
	Vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	Vec3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}

	// Casting-Operatoren
	operator float* () const			{return (float*)(c);}

	// Casting to "color" with alpha chanel==1/255
	operator dword () const				{return (byte(Clamp(x*255.0, 0.0, 255.0))<<24) | (byte(Clamp(y*255.0, 0.0, 255.0))<<16) | (byte(Clamp(z*255.0, 0.0, 255.0))<<8) | 255;}

	// Zuweisungsoperatoren
	Vec3& operator = (const Vec3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	Vec3& operator = (const float f)	{x = f; y = f; z = f; return *this;}
	Vec3& operator += (const Vec3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	Vec3& operator -= (const Vec3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vec3& operator *= (const Vec3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	Vec3& operator *= (const float f)	{x *= f; y *= f; z *= f; return *this;}
	Vec3& operator /= (const Vec3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	Vec3& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; return *this;}

	// ******************************************************************************** //
	// Arithmetische Operatoren
	inline Vec3 operator + (const Vec3& b) const	{return Vec3(x + b.x, y + b.y, z + b.z);}
	inline Vec3 operator - (const Vec3& b) const	{return Vec3(x - b.x, y - b.y, z - b.z);}
	inline Vec3 operator - () const					{return Vec3(-x, -y, -z);}
	inline Vec3 operator * (const Vec3& b) const	{return Vec3(x*b.x, y*b.y, z*b.z);}
	inline Vec3 operator * (const float f) const	{return Vec3(x * f, y * f, z * f);}
	inline Vec3 operator / (const Vec3& b) const	{return Vec3(x / b.x, y / b.y, z / b.z);}
	inline Vec3 operator / (float f) const			{f = 1/f; return Vec3(x * f, y * f, z * f);}

	// ******************************************************************************** //
	// Vergleichsoperatoren
	inline bool operator == (const Vec3& b) const	{if(x != b.x) return false; if(y != b.y) return false; return z == b.z;}
	inline bool operator != (const Vec3& b) const	{if(x != b.x) return true; if(y != b.y) return true; return z != b.z;}

	// ******************************************************************************** //
	// Vector3 Functions
	// The non-static and non-const functions always change the calling object
	inline float		Length() const																{return Sqrt(x * x + y * y + z * z);}
	inline float		LengthSq() const															{return x * x + y * y + z * z;}
	inline float		LengthInv() const															{return InvSqrt(x * x + y * y + z * z);}
	inline const Vec3&	Normalize()																	{*this *= InvSqrt(x * x + y * y + z * z); return *this;}
	inline static Vec3	Normalize(const Vec3& v)													{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
	inline const Vec3&	NormalizeEx()																{float fL = InvSqrt(x * x + y * y + z * z); *this *= OrE::Math::Min(fL, 100000000000000000.0f); return *this;}
	inline static Vec3	NormalizeEx(const Vec3& v)													{float fL = InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z); return v * OrE::Math::Min(fL, 100000000000000000.0f);}
	inline Vec3			Cross(const Vec3& v) const													{return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}
	inline static Vec3	Cross(const Vec3& v1, const Vec3& v2)										{return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
/*	inline static float CrossX(const float v1y, const float v1z, const float v2y, const float v2z)	{return v1y * v2z - v1z * v2y;}			// Nur die X Coordinate berechnen
	inline static float CrossY(const float v1x, const float v1z, const float v2x, const float v2z)	{return v1z * v2x - v1x * v2z;}			// Nur die Y Coordinate berechnen
	inline static float CrossZ(const float v1x, const float v1y, const float v2x, const float v2y)	{return v1x * v2y - v1y * v2x;}			// Nur die Z Coordinate berechnen*/
	inline float		Dot(const Vec3& v) const													{return x * v.x + y * v.y + z * v.z;}
//	inline static float Dot(const Vec3& v1, const Vec3& v2)											{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
	inline float		Angle(const Vec3& v) const													{return Cos((x * v.x + y * v.y + z * v.z) * InvSqrt((x * x + y * y + z * z) * (v.x * v.x + v.y * v.y + v.z * v.z)));}
//	inline static float Angle(const Vec3& v1, const Vec3& v2)										{return Cos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) * InvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
	static Vec3			Random();

/*	const Vec3&			TransformCoords(const Matrix& m, float& fOutW);								// 3D-Vektor mit Matrix multiplizieren
	static Vec3			TransformCoords(const Vec3& v, const Matrix& m, float& fOutW);				// 3D-Vektor mit Matrix multiplizieren
	const Vec3&			TransformCoords(const Matrix& m);											// 3D-Vektor mit Matrix multiplizieren
	static Vec3			TransformCoords(const Vec3& v, const Matrix& m);							// 3D-Vektor mit Matrix multiplizieren*/


	// Multiply a directional vector (left) with the matrix (from right).
	// The w component is ignored (9mul + 9add).
	// Use a transposed inverse to transform directions correct
	// (if transformation is affine and not riged)!
	Vec3			TransformDirection(const Matrix& m) const;
	//static Vec3			TransformDirection(const Vec3& v, const Matrix& m);

	// Rotate a vector around some axis. This transformation uses quaternion
	// math. Use this if you want to rotate onle one vector. For the rotation
	// of multiple vectors a matrix is more efficient!
	// Speed: Debug 480, Release 176
	Vec3			Rotate(const Vec3& vAxis, float fAngle) const;
	//static Vec3			Rotate(const Vec3& v, const Vec3& vAxis, float fAngle);
};

// ******************************************************************************** //
inline Vec3 operator * (const float f, const Vec3& v)				{return Vec3(v.x * f, v.y * f, v.z * f);}

// ******************************************************************************** //
// Lerp, Abs, Min and Max have to be in global scope to override the default template
inline Vec3	Vec3Min(const Vec3& v1, const Vec3& v2)					{return Vec3(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z));}
inline Vec3	Vec3Max(const Vec3& v1, const Vec3& v2)					{return Vec3(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z));}
inline Vec3	Lerp(const Vec3& v1, const Vec3& v2, const float f)		{return Vec3(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y), v1.z+f*(v2.z-v1.z));}
inline Vec3	Abs(const Vec3& v1)										{return Vec3(Abs(v1.x), Abs(v1.y), Abs(v1.z));}

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //