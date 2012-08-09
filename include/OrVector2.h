// ******************************************************************************** //
// OrVector2.h																		//
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
// Die 2D-Vektor-Klasse
class Vec2
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
		};

		float c[2];		// Array der Koordinaten
	};

	// Konstruktoren
	Vec2()																	{}
	Vec2(const Vec2& v) : x(v.x), y(v.y)									{}
	Vec2(const float f) : x(f), y(f)										{}
	Vec2(const float _x, const float _y) : x(_x), y(_y)						{}
	Vec2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}
	operator const float* () const {return (const float*)(c);}

	// Zuweisungsoperatoren
	Vec2& operator = (const Vec2& v)	{x = v.x; y = v.y; return *this;}
	Vec2& operator = (const float f)	{x = f; y = f; return *this;}
	Vec2& operator += (const Vec2& v)	{x += v.x; y += v.y; return *this;}
	Vec2& operator -= (const Vec2& v)	{x -= v.x; y -= v.y; return *this;}
	Vec2& operator *= (const Vec2& v)	{x *= v.x; y *= v.y; return *this;}
	Vec2& operator *= (const float f)	{x *= f; y *= f; return *this;}
	Vec2& operator /= (const Vec2& v)	{x /= v.x; y /= v.y; return *this;}
	Vec2& operator /= (float f)			{f = 1/f; x *= f; y *= f; return *this;}

	// ******************************************************************************** //
	// Arithmetische Operatoren
	inline Vec2 operator + (const Vec2& b) const	{return Vec2(x + b.x, y + b.y);}
	inline Vec2 operator - (const Vec2& b) const	{return Vec2(x - b.x, y - b.y);}
	inline Vec2 operator - () const					{return Vec2(-x, -y);}
	inline Vec2 operator * (const Vec2& b) const	{return Vec2(x * b.x, y * b.y);}
	inline Vec2 operator * (const float f) const	{return Vec2(x * f, y * f);}
	inline Vec2 operator / (const Vec2& b) const	{return Vec2(x / b.x, y / b.y);}
	inline Vec2 operator / (float f) const			{f = 1/f; return Vec2(x * f, y * f);}	// Multiplikation schneller

	// ******************************************************************************** //
	// Vergleichsoperatoren
	inline bool operator == (const Vec2& b) const	{if(x != b.x) return false; return y == b.y;}
	inline bool operator != (const Vec2& b) const	{if(x != b.x) return true; return y != b.y;}

	// ******************************************************************************** //
	// Vector2 functions
	// The non-static and non-const functions always change the calling object
	inline float		Length() const												{return Sqrt(x * x + y * y);}
	inline float		LengthSq() const											{return x * x + y * y;}
	inline float		LengthInv() const											{return InvSqrt(x * x + y * y);}
	inline const Vec2&	Normalize()													{*this *= InvSqrt(x * x + y * y); return *this;}
	inline static Vec2	Normalize(const Vec2& v)									{return v * InvSqrt(v.x * v.x + v.y * v.y);}
	inline const Vec2&	NormalizeEx()												{float fL = Sqrt(x * x + y * y); *this /= OrE::Math::Max(fL, 0.0000000000000000001f); return *this;}
	inline static Vec2	NormalizeEx(const Vec2& v)									{float fL = Sqrt(v.x * v.x + v.y * v.y); return v / OrE::Math::Max(fL, 0.0000000000000000001f);}
	inline float		Dot(const Vec2& v) const									{return x * v.x + y * v.y;}
//	inline static float	Dot(const Vec2& v1, const Vec2& v2)							{return v1.x * v2.x + v1.y * v2.y;}
	inline float		Cross(const Vec2& v) const									{return x * v.y - y * v.x;}
	inline static float Cross(const Vec2& v1, const Vec2& v2)						{return v1.x * v2.y - v1.y * v2.x;}
	inline Vec2			Perpendicular() const										{return Vec2(-y, x);}		// Senkrechter Vektor
//	inline static Vec2	Perpendicular(const Vec2& v)								{return Vec2(-v.y, v.x);}		// Senkrechter Vektor
	inline float		Angle(const Vec2& v) const									{return Cos((x * v.x + y * v.y) * InvSqrt((x * x + y * y) * (v.x * v.x + v.y * v.y)));}
//	inline static float	Angle(const Vec2& v1, const Vec2& v2)						{return Cos((v1.x * v2.x + v1.y * v2.y) * InvSqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
	static Vec2			Random();
	Vec2				Rotate(const float _fAlpha) const;								// Rotieren eines Punktes im 2D-Raum
	//static Vec2			Rotate(const Vec2& _v, const float _fAlpha);				// Rotieren eines Punktes im 2D-Raum
};

// ******************************************************************************** //
// The Operator, which cannot be inside the class
inline Vec2 operator * (const float f, const Vec2& v)			{return Vec2(v.x * f, v.y * f);}

// ******************************************************************************** //
// Lerp, Min, Max and Abs have to be in global scope to override the default template
inline Vec2	Min(const Vec2& v1, const Vec2& v2)					{return Vec2(Min(v1.x, v2.x), Min(v1.y, v2.y));}
inline Vec2	Max(const Vec2& v1, const Vec2& v2)					{return Vec2(Max(v1.x, v2.x), Max(v1.y, v2.y));}
inline Vec2	Lerp(const Vec2& v1, const Vec2& v2, const float f)	{return Vec2(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y));}
inline Vec2	Abs(const Vec2& v1)									{return Vec2(Abs(v1.x), Abs(v1.y));}

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //