// ******************************************************************************** //
// OrVector3.h																		//
// ===========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quite easy licensing as open source:									//
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
// The class for 3D vectors (element of R^3)
class Vec3
{
public:
	// Variables
	union
	{
		struct
		{
			float x;	// Coordinates
			float y;
			float z;
		};

		struct
		{
			float u;	// Nomenclature for texture coordinates
			float v;
			float w;
		};

		float c[3];		// Array interpretation from the vector
	};

	// Constructors
	Vec3()																						{}
	Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z)												{}
	Vec3(const Vec2& v, float f) : x(v.x), y(v.y), z(f)											{}
	Vec3(float f, const Vec2& v) : x(f), y(v.x), z(v.y)											{}
	Vec3(const float f) : x(f), y(f), z(f)														{}
	Vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	Vec3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}

	// Casting-operators
	operator float* ()												{return (float*)(c);}
	operator const float* () const									{return (const float*)(c);}

	// Casting to "color" with alpha channel==255
	operator uint32 () const											{return (byte(Clamp(x*255.0, 0.0, 255.0))<<24) | (byte(Clamp(y*255.0, 0.0, 255.0))<<16) | (byte(Clamp(z*255.0, 0.0, 255.0))<<8) | 255;}

	// Assignment operators
	Vec3& operator = (const Vec3& v)								{x = v.x; y = v.y; z = v.z; return *this;}
	Vec3& operator = (const float f)								{x = f; y = f; z = f; return *this;}
	Vec3& operator += (const Vec3& v)								{x += v.x; y += v.y; z += v.z; return *this;}
	Vec3& operator -= (const Vec3& v)								{x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vec3& operator *= (const Vec3& v)								{x *= v.x; y *= v.y; z *= v.z; return *this;}
	Vec3& operator *= (const float f)								{x *= f; y *= f; z *= f; return *this;}
	Vec3& operator /= (const Vec3& v)								{x /= v.x; y /= v.y; z /= v.z; return *this;}
	Vec3& operator /= (float f)										{f = 1/f; x *= f; y *= f; z *= f; return *this;}

	// ******************************************************************************** //
	// Arithmetic operators
	inline Vec3 operator + (const Vec3& b) const					{return Vec3(x + b.x, y + b.y, z + b.z);}
	inline Vec3 operator - (const Vec3& b) const					{return Vec3(x - b.x, y - b.y, z - b.z);}
	inline Vec3 operator - () const									{return Vec3(-x, -y, -z);}
	inline Vec3 operator * (const Vec3& b) const					{return Vec3(x*b.x, y*b.y, z*b.z);}
	inline Vec3 operator * (const float f) const					{return Vec3(x * f, y * f, z * f);}
	inline Vec3 operator / (const Vec3& b) const					{return Vec3(x / b.x, y / b.y, z / b.z);}
	inline Vec3 operator / (float f) const							{f = 1/f; return Vec3(x * f, y * f, z * f);}

	// ******************************************************************************** //
	// Comparison operators
	#define Vector_EPSILON	0.00001f
	inline bool operator == (const Vec3& b) const					{if(Abs(x - b.x) > Vector_EPSILON) return false; if(Abs(y - b.y) > Vector_EPSILON) return false; return Abs(z - b.z) < Vector_EPSILON;}
	inline bool operator != (const Vec3& b) const					{if(Abs(x - b.x) > Vector_EPSILON) return true; if(Abs(y - b.y) > Vector_EPSILON) return true; return Abs(z - b.z) > Vector_EPSILON;}

	// ******************************************************************************** //
	// Vector3 functions
	// The non-static and non-const functions always change the calling object
	inline float		Length() const								{return Sqrt(x * x + y * y + z * z);}
	inline float		LengthSq() const							{return x * x + y * y + z * z;}
	inline float		LengthInv() const							{return InvSqrt(x * x + y * y + z * z);}
	inline const Vec3&	Normalize()									{*this *= InvSqrt(x * x + y * y + z * z); return *this;}
	inline static Vec3	Normalize(const Vec3& v)					{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
	inline const Vec3&	NormalizeEx()								{float fL = InvSqrt(x * x + y * y + z * z); *this *= OrE::Math::Min(fL, 100000000000000000.0f); return *this;}
	inline static Vec3	NormalizeEx(const Vec3& v)					{float fL = InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z); return v * OrE::Math::Min(fL, 100000000000000000.0f);}
	inline Vec3			Cross(const Vec3& v) const					{return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}
	inline static Vec3	Cross(const Vec3& v1, const Vec3& v2)		{return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
	inline float		Dot(const Vec3& v) const					{return x * v.x + y * v.y + z * v.z;}
	inline float		Angle(const Vec3& v) const					{return Arccos((x * v.x + y * v.y + z * v.z) * InvSqrt((x * x + y * y + z * z) * (v.x * v.x + v.y * v.y + v.z * v.z)));}
	
	// Create a uniformly distributed random direction vector ( length == 1 ).
	static Vec3			RandomDirection();

	// Create a random point uniformly distributed in the [-1,1]^3 cube.
	static Vec3			RandomPosition();

	// Multiply a directional vector (left) with the matrix (from right).
	// The w component is ignored (9mul + 9add).
	// Use a transposed inverse to transform directions correct
	// (if transformation is affine and not rigged)!
	Vec3			TransformDirection(const Matrix& m) const;

	// Rotate a vector around some axis. This transformation uses quaternion
	// math. Use this if you want to rotate only one vector. For the rotation
	// of multiple vectors a matrix is more efficient!
	// Speed: Debug 480, Release 176
	Vec3			Rotate(const Vec3& vAxis, float fAngle) const;
};

// ******************************************************************************** //
inline Vec3 operator * (const float f, const Vec3& v)				{return Vec3(v.x * f, v.y * f, v.z * f);}

// ******************************************************************************** //
// Lerp, Abs, Min and Max have to be in global scope to override the default template
inline Vec3	Min(const Vec3& v1, const Vec3& v2)						{return Vec3(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z));}
inline Vec3	Max(const Vec3& v1, const Vec3& v2)						{return Vec3(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z));}
inline Vec3	Abs(const Vec3& v1)										{return Vec3(Abs(v1.x), Abs(v1.y), Abs(v1.z));}
inline Vec3	Lerp(const Vec3& v1, const Vec3& v2, const float t)		{return Vec3(v1.x+t*(v2.x-v1.x), v1.y+t*(v2.y-v1.y), v1.z+t*(v2.z-v1.z));}
// The spherical interpolation applies only to normal vectors
Vec3 Slerp(const Vec3& v1, const Vec3& v2, const float t);




// ******************************************************************************** //
// The same class again, but with integer numbers (element of Z^3).
// ******************************************************************************** //
class IVec3
{
public:
	// Variables
	union
	{
		struct
		{
			int x;	// Coordinates
			int y;
			int z;
		};

		struct
		{
			int u;	// Nomenclature for texture coordinates
			int v;
			int w;
		};

		int c[3];	// Array interpretation from the vector
	};

	// Constructors
	IVec3()																						{}
	IVec3(const IVec3& v) : x(v.x), y(v.y), z(v.z)												{}
	IVec3(const Vec3& v) : x(int(v.x)), y(int(v.y)), z(int(v.z))								{}
	IVec3(const int i) : x(i), y(i), z(i)														{}
	IVec3(const int _x, const int _y, const int _z) : x(_x), y(_y), z(_z)						{}
	IVec3(const int* piComponent) : x(piComponent[0]), y(piComponent[1]), z(piComponent[2])		{}

	// Casting-Operators
	operator int* ()												{return (int*)(c);}
	operator const int* () const									{return (const int*)(c);}

	// Casting to "color" with alpha channel==255
	operator uint32 () const											{return (Clamp(x, 0, 255)<<24) | (Clamp(y, 0, 255)<<16) | (Clamp(z, 0, 255)<<8) | 255;}

	// Assignment operators
	IVec3& operator = (const IVec3& v)								{x = v.x; y = v.y; z = v.z; return *this;}
	IVec3& operator = (const int i)									{x = i; y = i; z = i; return *this;}
	IVec3& operator += (const IVec3& v)								{x += v.x; y += v.y; z += v.z; return *this;}
	IVec3& operator -= (const IVec3& v)								{x -= v.x; y -= v.y; z -= v.z; return *this;}
	IVec3& operator *= (const IVec3& v)								{x *= v.x; y *= v.y; z *= v.z; return *this;}
	IVec3& operator *= (const int i)								{x *= i; y *= i; z *= i; return *this;}
	IVec3& operator /= (const IVec3& v)								{x /= v.x; y /= v.y; z /= v.z; return *this;}
	IVec3& operator /= (int i)										{x /= i; y /= i; z /= i; return *this;}

	// ******************************************************************************** //
	// Arithmetic operators
	inline IVec3 operator + (const IVec3& b) const					{return IVec3(x + b.x, y + b.y, z + b.z);}
	inline IVec3 operator - (const IVec3& b) const					{return IVec3(x - b.x, y - b.y, z - b.z);}
	inline IVec3 operator - () const								{return IVec3(-x, -y, -z);}
	inline IVec3 operator * (const IVec3& b) const					{return IVec3(x*b.x, y*b.y, z*b.z);}
	inline IVec3 operator * (const int i) const						{return IVec3(x * i, y * i, z * i);}
	inline IVec3 operator / (const IVec3& b) const					{return IVec3(x / b.x, y / b.y, z / b.z);}
	inline IVec3 operator / (int i) const							{return IVec3(x * i, y * i, z * i);}

	// ******************************************************************************** //
	// Comparison operators
	inline bool operator == (const IVec3& b) const					{if(x != b.x) return false; if(y != b.y) return false; return z == b.z;}
	inline bool operator != (const IVec3& b) const					{if(x != b.x) return true; if(y != b.y) return true; return z != b.z;}

	// ******************************************************************************** //
	// Vector3 functions
	// The non-static and non-const functions always change the calling object
	inline int			Length() const								{return int(Sqrt(float(x * x + y * y + z * z)));}
	inline int			LengthSq() const							{return x * x + y * y + z * z;}
	inline const IVec3&	Normalize(int iL)							{float fL = iL*InvSqrt(float(x * x + y * y + z * z)); x = int(x*fL); y = int(y*fL); z = int(z*fL); return *this;}
	inline static IVec3	Normalize(const IVec3& v, int iL)			{float fL = iL*InvSqrt(float(v.x * v.x + v.y * v.y + v.z * v.z)); return IVec3(int(v.x*fL), int(v.y*fL), int(v.z*fL)); }
	inline const IVec3&	NormalizeEx(int iL)							{int i = x * x + y * y + z * z; float fL = (i!=0)?iL*InvSqrt(float(i)):0.0f; x = int(x*fL); y = int(y*fL); z = int(z*fL); return *this;}
	inline static IVec3	NormalizeEx(const IVec3& v, int iL)			{int i = v.x * v.x + v.y * v.y + v.z * v.z; float fL = (i!=0)?iL*InvSqrt(float(i)):0.0f; return IVec3(int(v.x*fL), int(v.y*fL), int(v.z*fL)); }
	inline IVec3		Cross(const IVec3& v) const					{return IVec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}
	inline static IVec3	Cross(const IVec3& v1, const IVec3& v2)		{return IVec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
	inline int			Dot(const IVec3& v) const					{return x * v.x + y * v.y + z * v.z;}
	inline float		Angle(const IVec3& v) const					{return Cos((x * v.x + y * v.y + z * v.z) * InvSqrt(float(x * x + y * y + z * z) * float(v.x * v.x + v.y * v.y + v.z * v.z)));}

	// Creates an vector with length 1000 and random direction
	static IVec3		Random();
};

// ******************************************************************************** //
inline IVec3 operator * (const int i, const IVec3& v)				{return IVec3(v.x * i, v.y * i, v.z * i);}

// ******************************************************************************** //
// Lerp, Abs, Min and Max have to be in global scope to override the default template
inline IVec3 Min(const IVec3& v1, const IVec3& v2)					{return IVec3(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z));}
inline IVec3 Max(const IVec3& v1, const IVec3& v2)					{return IVec3(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z));}
inline IVec3 Lerp(const IVec3& v1, const IVec3& v2, const int i)	{return IVec3(v1.x+i*(v2.x-v1.x), v1.y+i*(v2.y-v1.y), v1.z+i*(v2.z-v1.z));}
inline IVec3 Abs(const IVec3& v1)									{return IVec3(Abs(v1.x), Abs(v1.y), Abs(v1.z));}



// ******************************************************************************** //
// Combined operators between integer and float vectors
inline Vec3 operator + (const Vec3& v1, const IVec3& v2)			{return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);}
inline Vec3 operator + (const IVec3& v1, const Vec3& v2)			{return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);}
inline Vec3 operator - (const Vec3& v1, const IVec3& v2)			{return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);}
inline Vec3 operator - (const IVec3& v1, const Vec3& v2)			{return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);}
inline Vec3 operator * (const Vec3& v1, const IVec3& v2)			{return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);}
inline Vec3 operator * (const IVec3& v1, const Vec3& v2)			{return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);}
inline Vec3 operator / (const Vec3& v1, const IVec3& v2)			{return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);}
inline Vec3 operator / (const IVec3& v1, const Vec3& v2)			{return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);}


}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //