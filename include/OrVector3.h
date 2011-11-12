/*************************************************************************

	OrVector3.h
	===========
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 3D-Vektoren

	Autor: Johannes Jendersie
	Verwendete Quellen: TriBase-Engine

*************************************************************************/

#pragma once


// ******************************************************************** //
// Die 3D-Vektor-Klasse
class OrVector3
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
	OrVector3()																						{}
	OrVector3(const OrVector3& v) : x(v.x), y(v.y), z(v.z)											{}
	OrVector3(const float f) : x(f), y(f), z(f)														{}
	OrVector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	OrVector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}

	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}

	// Zuweisungsoperatoren
	OrVector3& operator = (const OrVector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	OrVector3& operator = (const float f)		{x = f; y = f; z = f; return *this;}
	OrVector3& operator += (const OrVector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	OrVector3& operator -= (const OrVector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	OrVector3& operator *= (const OrVector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	OrVector3& operator *= (const float f)		{x *= f; y *= f; z *= f; return *this;}
	OrVector3& operator /= (const OrVector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	OrVector3& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; return *this;}
};

// Arithmetische Operatoren
inline OrVector3 operator + (const OrVector3& a, const OrVector3& b)	{return OrVector3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline OrVector3 operator - (const OrVector3& a, const OrVector3& b)	{return OrVector3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline OrVector3 operator - (const OrVector3& v)						{return OrVector3(-v.x, -v.y, -v.z);}
inline OrVector3 operator * (const OrVector3& a, const OrVector3& b)	{return OrVector3(a.x*b.x, a.y*b.y, a.z*b.z);}
inline OrVector3 operator * (const OrVector3& v, const float f)			{return OrVector3(v.x * f, v.y * f, v.z * f);}
inline OrVector3 operator * (const float f, const OrVector3& v)			{return OrVector3(v.x * f, v.y * f, v.z * f);}
inline OrVector3 operator / (const OrVector3& a, const OrVector3& b)	{return OrVector3(a.x / b.x, a.y / b.y, a.z / b.z);}
inline OrVector3 operator / (const OrVector3& v, float f)				{f = 1/f; return OrVector3(v.x * f, v.y * f, v.z * f);}

// Vergleichsoperatoren
inline bool operator == (const OrVector3& a, const OrVector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; return a.z == b.z;}
inline bool operator != (const OrVector3& a, const OrVector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; return a.z != b.z;}

// ******************************************************************** //
// Funktionen deklarieren
inline float		OrVec3Length(const OrVector3& v)													{return OrSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float		OrVec3LengthSq(const OrVector3& v)													{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline float		OrVec3LengthInv(const OrVector3& v)													{return OrInvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline OrVector3	OrVec3Normalize(const OrVector3& v)													{return v * OrInvSqrt(v.x * v.x + v.y * v.y + v.z * v.z);}
inline OrVector3	OrVec3NormalizeEx(const OrVector3& v)												{return v * OrInvSqrtEx(v.x * v.x + v.y * v.y + v.z * v.z);}
inline OrVector3	OrVec3Cross(const OrVector3& v1, const OrVector3& v2)								{return OrVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float		OrVec3CrossX(const float v1y, const float v1z, const float v2y, const float v2z)	{return v1y * v2z - v1z * v2y;}			// Nur die X Coordinate berechnen
inline float		OrVec3CrossY(const float v1x, const float v1z, const float v2x, const float v2z)	{return v1z * v2x - v1x * v2z;}			// Nur die Y Coordinate berechnen
inline float		OrVec3CrossZ(const float v1x, const float v1y, const float v2x, const float v2y)	{return v1x * v2y - v1y * v2x;}			// Nur die Z Coordinate berechnen
inline float		OrVec3Dot(const OrVector3& v1, const OrVector3& v2)									{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float		OrVec3Angle(const OrVector3& v1, const OrVector3& v2)								{return OrCos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) * OrInvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
inline OrVector3	OrVec3LrpNormal(const OrVector3& v1, const OrVector3& v2, const float p)			{return OrVec3NormalizeEx(v1 + p * (v2 - v1));}
inline OrVector3	OrVec3Min(const OrVector3& v1, const OrVector3& v2)									{return OrVector3(OrMinf(v1.x, v2.x), OrMinf(v1.y, v2.y), OrMinf(v1.z, v2.z));}
inline OrVector3	OrVec3Max(const OrVector3& v1, const OrVector3& v2)									{return OrVector3(OrMaxf(v1.x, v2.x), OrMaxf(v1.y, v2.y), OrMaxf(v1.z, v2.z));}
inline OrVector3	OrVec3Lrp(const OrVector3& v1, const OrVector3& v2, const float f)					{return OrVector3(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y), v1.z+f*(v2.z-v1.z));}
OrVector3			OrVec3Random();

OrVector3	OrVec3TransformCoords(const OrVector3& v, const OrMatrix& m, float* const pfOutW);																																									// 3D-Vektor mit Matrix multiplizieren
OrVector3	OrVec3TransformCoords(const OrVector3& v, const OrMatrix& m);																																														// 3D-Vektor mit Matrix multiplizieren
OrVector3	OrVec3TransformDirection(const OrVector3& v, const OrMatrix& m);																																													// 3D-Richtungsvektor mit Matrix multiplizieren
/*OrVector3	OrVec3TransformNormal(const OrVector3& v, const OrMatrix& m);																																														// 3D-Normalenvektor mit Matrix multiplizieren
OrVector3	OrVec3TransformNormal_TranspInv(const OrVector3& v, const OrMatrix& m);																																												// 3D-Normalenvektor mit Matrix multiplizieren
*/

// ******************************************************************** //