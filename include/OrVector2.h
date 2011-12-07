/*************************************************************************

	Vector2.h
	===========
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

	Autor: Johannes Jendersie
	Verwendete Quellen: TriBase-Engine

*************************************************************************/

#pragma once

namespace OrE {
namespace Math {

// ******************************************************************** //
// Die 2D-Vektor-Klasse
class Vector2
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
	Vector2()																	{}
	Vector2(const Vector2& v) : x(v.x), y(v.y)									{}
	Vector2(const float f) : x(f), y(f)											{}
	Vector2(const float _x, const float _y) : x(_x), y(_y)						{}
	Vector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	Vector2& operator = (const Vector2& v)	{x = v.x; y = v.y; return *this;}
	Vector2& operator = (const float f)		{x = f; y = f; return *this;}
	Vector2& operator += (const Vector2& v)	{x += v.x; y += v.y; return *this;}
	Vector2& operator -= (const Vector2& v)	{x -= v.x; y -= v.y; return *this;}
	Vector2& operator *= (const Vector2& v)	{x *= v.x; y *= v.y; return *this;}
	Vector2& operator *= (const float f)	{x *= f; y *= f; return *this;}
	Vector2& operator /= (const Vector2& v)	{x /= v.x; y /= v.y; return *this;}
	Vector2& operator /= (float f)			{f = 1/f; x *= f; y *= f; return *this;}
};

// Arithmetische Operatoren
inline Vector2 operator + (const Vector2& a, const Vector2& b)	{return Vector2(a.x + b.x, a.y + b.y);}
inline Vector2 operator - (const Vector2& a, const Vector2& b)	{return Vector2(a.x - b.x, a.y - b.y);}
inline Vector2 operator - (const Vector2& v)					{return Vector2(-v.x, -v.y);}
inline Vector2 operator * (const Vector2& a, const Vector2& b)	{return Vector2(a.x * b.x, a.y * b.y);}
inline Vector2 operator * (const Vector2& v, const float f)		{return Vector2(v.x * f, v.y * f);}
inline Vector2 operator * (const float f, const Vector2& v)		{return Vector2(v.x * f, v.y * f);}
inline Vector2 operator / (const Vector2& a, const Vector2& b)	{return Vector2(a.x / b.x, a.y / b.y);}
inline Vector2 operator / (const Vector2& v, float f)			{f = 1/f; return Vector2(v.x * f, v.y * f);}	// Multiplikation schneller

// Vergleichsoperatoren
inline bool operator == (const Vector2& a, const Vector2& b) {if(a.x != b.x) return false; return a.y == b.y;}
inline bool operator != (const Vector2& a, const Vector2& b) {if(a.x != b.x) return true; return a.y != b.y;}

// ******************************************************************** //
// Funktionen deklarieren
inline float	Vec2Length(const Vector2& v)												{return Sqrt(v.x * v.x + v.y * v.y);}
inline float	Vec2LengthSq(const Vector2& v)												{return v.x * v.x + v.y * v.y;}
inline Vector2	Vec2Normalize(const Vector2& v)												{return v * InvSqrt(v.x * v.x + v.y * v.y);}
inline Vector2	Vec2NormalizeEx(const Vector2& v)											{return v * InvSqrt(v.x * v.x + v.y * v.y);}
inline float	Vec2Dot(const Vector2& v1, const Vector2& v2)								{return v1.x * v2.x + v1.y * v2.y;}
inline float	Vec2Cross(const Vector2& v1, const Vector2& v2)								{return v1.x * v2.y - v1.y * v2.x;}
inline Vector2	Vec2Perpendicular(const Vector2& v)											{return Vector2(-v.y, v.x);}		// Senkrechter Vektor
inline float	Vec2Angle(const Vector2& v1, const Vector2& v2)								{return Cos((v1.x * v2.x + v1.y * v2.y) * InvSqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
inline Vector2	Vec2LrpNormal(const Vector2& v1, const Vector2& v2, const float p)			{return Vec2NormalizeEx(v1 + p * (v2 - v1));}
inline Vector2	Vec2Min(const Vector2& v1, const Vector2& v2)								{return Vector2(Minf(v1.x, v2.x), Minf(v1.y, v2.y));}
inline Vector2	Vec2Max(const Vector2& v1, const Vector2& v2)								{return Vector2(Maxf(v1.x, v2.x), Maxf(v1.y, v2.y));}
inline Vector2	Vec2Lrp(const Vector2& v1, const Vector2& v2, const float f)				{return Vector2(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y));}
Vector2			Vec2Random();
Vector2			Vec2Rotate(const Vector2& _v, const float _fAlpha);											// Rotieren eines Punktes im 2D-Raum
Vector2			Vec2Transform(const Vector2& v, const Matrix2x3& m);										// 2D-Vektor mit Matrix multiplizieren

}; // namespace Math
}; // namespace OrE
// ******************************************************************** //