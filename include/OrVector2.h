// ******************************************************************************** //
// OrVec2.h																		//
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
	Vec2(const float f) : x(f), y(f)											{}
	Vec2(const float _x, const float _y) : x(_x), y(_y)						{}
	Vec2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	Vec2& operator = (const Vec2& v)	{x = v.x; y = v.y; return *this;}
	Vec2& operator = (const float f)		{x = f; y = f; return *this;}
	Vec2& operator += (const Vec2& v)	{x += v.x; y += v.y; return *this;}
	Vec2& operator -= (const Vec2& v)	{x -= v.x; y -= v.y; return *this;}
	Vec2& operator *= (const Vec2& v)	{x *= v.x; y *= v.y; return *this;}
	Vec2& operator *= (const float f)	{x *= f; y *= f; return *this;}
	Vec2& operator /= (const Vec2& v)	{x /= v.x; y /= v.y; return *this;}
	Vec2& operator /= (float f)			{f = 1/f; x *= f; y *= f; return *this;}
};

// Arithmetische Operatoren
inline Vec2 operator + (const Vec2& a, const Vec2& b)	{return Vec2(a.x + b.x, a.y + b.y);}
inline Vec2 operator - (const Vec2& a, const Vec2& b)	{return Vec2(a.x - b.x, a.y - b.y);}
inline Vec2 operator - (const Vec2& v)					{return Vec2(-v.x, -v.y);}
inline Vec2 operator * (const Vec2& a, const Vec2& b)	{return Vec2(a.x * b.x, a.y * b.y);}
inline Vec2 operator * (const Vec2& v, const float f)		{return Vec2(v.x * f, v.y * f);}
inline Vec2 operator * (const float f, const Vec2& v)		{return Vec2(v.x * f, v.y * f);}
inline Vec2 operator / (const Vec2& a, const Vec2& b)	{return Vec2(a.x / b.x, a.y / b.y);}
inline Vec2 operator / (const Vec2& v, float f)			{f = 1/f; return Vec2(v.x * f, v.y * f);}	// Multiplikation schneller

// ******************************************************************************** //
// Vergleichsoperatoren
inline bool operator == (const Vec2& a, const Vec2& b) {if(a.x != b.x) return false; return a.y == b.y;}
inline bool operator != (const Vec2& a, const Vec2& b) {if(a.x != b.x) return true; return a.y != b.y;}

// ******************************************************************************** //
// Funktionen deklarieren
inline float	Vec2Length(const Vec2& v)												{return Sqrt(v.x * v.x + v.y * v.y);}
inline float	Vec2LengthSq(const Vec2& v)												{return v.x * v.x + v.y * v.y;}
inline Vec2	Vec2Normalize(const Vec2& v)												{return v * InvSqrt(v.x * v.x + v.y * v.y);}
inline Vec2	Vec2NormalizeEx(const Vec2& v)											{return v * InvSqrt(v.x * v.x + v.y * v.y);}
inline float	Vec2Dot(const Vec2& v1, const Vec2& v2)								{return v1.x * v2.x + v1.y * v2.y;}
inline float	Vec2Cross(const Vec2& v1, const Vec2& v2)								{return v1.x * v2.y - v1.y * v2.x;}
inline Vec2	Vec2Perpendicular(const Vec2& v)											{return Vec2(-v.y, v.x);}		// Senkrechter Vektor
inline float	Vec2Angle(const Vec2& v1, const Vec2& v2)								{return Cos((v1.x * v2.x + v1.y * v2.y) * InvSqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
inline Vec2	Vec2LrpNormal(const Vec2& v1, const Vec2& v2, const float p)			{return Vec2NormalizeEx(v1 + p * (v2 - v1));}
inline Vec2	Vec2Min(const Vec2& v1, const Vec2& v2)								{return Vec2(Min(v1.x, v2.x), Min(v1.y, v2.y));}
inline Vec2	Vec2Max(const Vec2& v1, const Vec2& v2)								{return Vec2(Max(v1.x, v2.x), Max(v1.y, v2.y));}
inline Vec2	Vec2Lrp(const Vec2& v1, const Vec2& v2, const float f)				{return Vec2(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y));}
Vec2			Vec2Random();
Vec2			Vec2Rotate(const Vec2& _v, const float _fAlpha);											// Rotieren eines Punktes im 2D-Raum
Vec2			Vec2Transform(const Vec2& v, const Matrix2x3& m);										// 2D-Vektor mit Matrix multiplizieren

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //