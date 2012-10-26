// ******************************************************************************** //
// OrPlane.h																		//
// =========																		//
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

#include "OrVector3.h"

namespace OrE {
namespace Math {


// ******************************************************************************** //
// Die Ebenenklasse
class Plane
{
public:
	// Variablen
	union
	{
		struct
		{
			float a;		// Variablen der Ebenengleichung
			float b;
			float c;
			float d;
		};

		struct
		{
			Vec3		n;	// Normalenvektor
			float		d;	// Distanz
		};

		float v[4];			// Array der Variablen
	};

	// Konstruktoren
	Plane()																	{}
	Plane(const Plane& p) : a(p.a), b(p.b), c(p.c), d(p.d)					{}
	Plane(const float _a, const float _b, const float _c, const float _d)	{float fInvLenght = Vec3(_a, _b, _c).LengthInv(); a = _a*fInvLenght; b = _b*fInvLenght; c = _c*fInvLenght; d = _d*fInvLenght;}
	Plane(const float* pfValue)												{float fInvLenght = Vec3(pfValue).LengthInv(); a = pfValue[0]*fInvLenght; b = pfValue[1]*fInvLenght; c = pfValue[2]*fInvLenght; d = pfValue[3]*fInvLenght;}
	Plane(const Vec3& _n, float _d)											{float fInvLenght = _n.LengthInv(); a = _n.x*fInvLenght; b = _n.y*fInvLenght; c = _n.z*fInvLenght; d = _d*fInvLenght;}

	// Casting-Operatoren
	operator float* () {return (float*)(v);}

	// Zuweisungsoperatoren
	Plane& operator = (const Plane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}

	// Methoden
	inline Vec3 CalcReflect(Vec3 vRay) {return (vRay - vRay.Dot(n)*2.0f*n);}
};

// Vergleichsoperatoren
inline bool operator == (const Plane& a, const Plane& b) {if(a.a != b.a) return false; if(a.b != b.b) return false; if(a.c != b.c) return false; return a.d == b.d;}
inline bool operator != (const Plane& a, const Plane& b) {if(a.a != b.a) return true; if(a.b != b.b) return true; if(a.c != b.c) return true; return a.d != b.d;}

// ******************************************************************************** //
// Funktionen deklarieren
inline Plane	PlaneNormalize(const Plane& p)									{const float fInvLength = p.n.LengthInv(); return Plane(p.n * fInvLength, p.d * fInvLength);}
inline float	PlaneDotNormal(const Plane& p, const Vec3& v)					{return p.a * v.x + p.b * v.y + p.c * v.z;}
inline float	PlaneDotCoords(const Plane& p, const Vec3& v)					{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
inline Plane	PlaneFromPointNormal(const Vec3& p, const Vec3& n)				{return Plane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
inline Plane	PlaneFromPoints(const Vec3& v1, const Vec3& v2, const Vec3& v3)	{return PlaneFromPointNormal(v1, Vec3::Cross(v3 - v2, v1 - v2));}


}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //