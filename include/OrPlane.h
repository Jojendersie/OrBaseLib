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
// A class for planes
class Plane
{
public:
	// Variables
	union
	{
		struct
		{
			float a;		// Variables of plane equation
			float b;
			float c;
			float d;
		};

		struct
		{
			Vec3		n;	// Normal vector
			float		d;	// Distance to 0
		};

		float v[4];			// Array access
	};

	// Constructors
	Plane()																	{}
	Plane(const Plane& p) : a(p.a), b(p.b), c(p.c), d(p.d)					{}
	Plane(const float _a, const float _b, const float _c, const float _d)	{float fInvLenght = Vec3(_a, _b, _c).LengthInv(); a = _a*fInvLenght; b = _b*fInvLenght; c = _c*fInvLenght; d = _d*fInvLenght;}
	Plane(const float* pfValue)												{float fInvLenght = Vec3(pfValue).LengthInv(); a = pfValue[0]*fInvLenght; b = pfValue[1]*fInvLenght; c = pfValue[2]*fInvLenght; d = pfValue[3]*fInvLenght;}
	Plane(const Vec3& _n, float _d)											{float fInvLenght = _n.LengthInv(); a = _n.x*fInvLenght; b = _n.y*fInvLenght; c = _n.z*fInvLenght; d = _d*fInvLenght;}
	Plane(const Vec3& _p, const Vec3& _n)									{float fInvLenght = _n.LengthInv(); a = _n.x*fInvLenght; b = _n.y*fInvLenght; c = _n.z*fInvLenght; d = -n.x * _p.x - n.y * _p.y - n.z * _p.z;}
	Plane(const Vec3& v1, const Vec3& v2, const Vec3& v3)					{Vec3 _n=Vec3::Cross(v3 - v2, v1 - v2); float fInvLenght = _n.LengthInv(); a = _n.x*fInvLenght; b = _n.y*fInvLenght; c = _n.z*fInvLenght; d = -n.x * v1.x - n.y * v1.y - n.z * v1.z;}

	// Casting-operators
	operator float* () {return (float*)(v);}

	// Assignment operator
	Plane& operator = (const Plane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}

	// Methods for plain calculations
	inline Vec3 CalcReflect(Vec3 vRay) {return (vRay - vRay.Dot(n)*2.0f*n);}
	static inline Plane Normalize(const Plane& p)							{const float fInvLength = p.n.LengthInv(); return Plane(p.n * fInvLength, p.d * fInvLength);}
	inline Plane Normalize()												{const float fInvLength = n.LengthInv(); a *= fInvLength; b *= fInvLength; c *= fInvLength; d *= fInvLength;}
	inline float DotNormal(const Plane& p, const Vec3& v)					{return p.a * v.x + p.b * v.y + p.c * v.z;}
	inline float DotCoords(const Plane& p, const Vec3& v)					{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
};

// Comparison operators
inline bool operator == (const Plane& a, const Plane& b) {if(a.a != b.a) return false; if(a.b != b.b) return false; if(a.c != b.c) return false; return a.d == b.d;}
inline bool operator != (const Plane& a, const Plane& b) {if(a.a != b.a) return true; if(a.b != b.b) return true; if(a.c != b.c) return true; return a.d != b.d;}



}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //