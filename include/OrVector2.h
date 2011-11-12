/*************************************************************************

	OrVector2.h
	===========
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

	Autor: Johannes Jendersie
	Verwendete Quellen: TriBase-Engine

*************************************************************************/

#pragma once

// ******************************************************************** //
// Die 2D-Vektor-Klasse
class OrVector2
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
	OrVector2()																	{}
	OrVector2(const OrVector2& v) : x(v.x), y(v.y)								{}
	OrVector2(const float f) : x(f), y(f)										{}
	OrVector2(const float _x, const float _y) : x(_x), y(_y)					{}
	OrVector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	OrVector2& operator = (const OrVector2& v)	{x = v.x; y = v.y; return *this;}
	OrVector2& operator = (const float f)		{x = f; y = f; return *this;}
	OrVector2& operator += (const OrVector2& v)	{x += v.x; y += v.y; return *this;}
	OrVector2& operator -= (const OrVector2& v)	{x -= v.x; y -= v.y; return *this;}
	OrVector2& operator *= (const OrVector2& v)	{x *= v.x; y *= v.y; return *this;}
	OrVector2& operator *= (const float f)		{x *= f; y *= f; return *this;}
	OrVector2& operator /= (const OrVector2& v)	{x /= v.x; y /= v.y; return *this;}
	OrVector2& operator /= (float f)			{f = 1/f; x *= f; y *= f; return *this;}
};

// Arithmetische Operatoren
inline OrVector2 operator + (const OrVector2& a, const OrVector2& b)	{return OrVector2(a.x + b.x, a.y + b.y);}
inline OrVector2 operator - (const OrVector2& a, const OrVector2& b)	{return OrVector2(a.x - b.x, a.y - b.y);}
inline OrVector2 operator - (const OrVector2& v)						{return OrVector2(-v.x, -v.y);}
inline OrVector2 operator * (const OrVector2& a, const OrVector2& b)	{return OrVector2(a.x * b.x, a.y * b.y);}
inline OrVector2 operator * (const OrVector2& v, const float f)			{return OrVector2(v.x * f, v.y * f);}
inline OrVector2 operator * (const float f, const OrVector2& v)			{return OrVector2(v.x * f, v.y * f);}
inline OrVector2 operator / (const OrVector2& a, const OrVector2& b)	{return OrVector2(a.x / b.x, a.y / b.y);}
inline OrVector2 operator / (const OrVector2& v, float f)				{f = 1/f; return OrVector2(v.x * f, v.y * f);}	// Multiplikation schneller

// Vergleichsoperatoren
inline bool operator == (const OrVector2& a, const OrVector2& b) {if(a.x != b.x) return false; return a.y == b.y;}
inline bool operator != (const OrVector2& a, const OrVector2& b) {if(a.x != b.x) return true; return a.y != b.y;}

// ******************************************************************** //
// Funktionen deklarieren
inline float		OrVec2Length(const OrVector2& v)													{return OrSqrt(v.x * v.x + v.y * v.y);}
inline float		OrVec2LengthSq(const OrVector2& v)													{return v.x * v.x + v.y * v.y;}
inline OrVector2	OrVec2Normalize(const OrVector2& v)													{return v * OrInvSqrt(v.x * v.x + v.y * v.y);}
inline OrVector2	OrVec2NormalizeEx(const OrVector2& v)												{return v * OrInvSqrtEx(v.x * v.x + v.y * v.y);}
inline float		OrVec2Dot(const OrVector2& v1, const OrVector2& v2)									{return v1.x * v2.x + v1.y * v2.y;}
inline float		OrVec2Cross(const OrVector2& v1, const OrVector2& v2)								{return v1.x * v2.y - v1.y * v2.x;}
inline OrVector2	OrVec2Perpendicular(const OrVector2& v)												{return OrVector2(-v.y, v.x);}		// Senkrechter Vektor
inline float		OrVec2Angle(const OrVector2& v1, const OrVector2& v2)								{return OrCos((v1.x * v2.x + v1.y * v2.y) * OrInvSqrt((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
inline OrVector2	OrVec2LrpNormal(const OrVector2& v1, const OrVector2& v2, const float p)			{return OrVec2NormalizeEx(v1 + p * (v2 - v1));}
inline OrVector2	OrVec2Min(const OrVector2& v1, const OrVector2& v2)									{return OrVector2(OrMinf(v1.x, v2.x), OrMinf(v1.y, v2.y));}
inline OrVector2	OrVec2Max(const OrVector2& v1, const OrVector2& v2)									{return OrVector2(OrMaxf(v1.x, v2.x), OrMaxf(v1.y, v2.y));}
inline OrVector2	OrVec2Lrp(const OrVector2& v1, const OrVector2& v2, const float f)					{return OrVector2(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y));}
OrVector2			OrVec2Random();
OrVector2			OrVec2Rotate(const OrVector2& _v, const float _fAlpha);		// Rotieren eines Punktes im 2D-Raum

// ******************************************************************** //