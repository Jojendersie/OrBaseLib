// ******************************************************************************** //
// OrVec4.h																		//
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
// Die 4D-Vektor-Klasse
class Vec4
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
			float w;
		};

		struct
		{
			float left;   // Texturkoordinaten
			float top;
			float right;
			float bottom;
		};

		struct
		{
			Vec2 v1;	// Vektoren
			Vec2 v2;
		};

		struct
		{
			Vec3 v3;
			float	r;
		};

		float			c[4];			// Array der Koordinaten
	};

	// Konstruktoren
	Vec4()																										{}
	Vec4(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)													{}
	Vec4(const Vec3& v) : x(v.x), y(v.y), z(v.z), w(1.0f)													{}
	Vec4(const Vec3& v, const float fw) : x(v.x), y(v.y), z(v.z), w(fw)									{}
	Vec4(const Vec2& v) : x(v.x), y(v.y), z(1.0f), w(1.0f)												{}
	Vec4(const Vec2& v, const Vec2& v2) : x(v.x), y(v.y), z(v2.x), w(v2.y)							{}
	Vec4(const float f) : x(f), y(f), z(f), w(f)																{}
	Vec4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w)			{}
	Vec4(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2]), w(pfComponent[3]){}

	// Casting-Operatoren
	operator float* ()						{return (float*)(c);}
	operator Vec3 ()						{return v3;}

	// Zuweisungsoperatoren
	Vec4& operator = (const Vec4& v)	{x = v.x; y = v.y; z = v.z; w = v.w; return *this;}
	Vec4& operator = (const float f)		{x = f; y = f; z = f; w = f; return *this;}
	Vec4& operator += (const Vec4& v)	{x += v.x; y += v.y; z += v.z; w += v.w; return *this;}
	Vec4& operator -= (const Vec4& v)	{x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;}
	Vec4& operator *= (const Vec4& v)	{x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;}
	Vec4& operator *= (const float f)	{x *= f; y *= f; z *= f; w *= f; return *this;}
	Vec4& operator /= (const Vec4& v)	{x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;}
	Vec4& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; w *= f; return *this;}
};

// ******************************************************************************** //
// Arithmetische Operatoren
inline Vec4 operator + (const Vec4& a, const Vec4& b)	{return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);}
inline Vec4 operator - (const Vec4& a, const Vec4& b)	{return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);}
inline Vec4 operator - (const Vec4& v)					{return Vec4(-v.x, -v.y, -v.z, -v.w);}
inline Vec4 operator * (const Vec4& a, const Vec4& b)	{return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);}
inline Vec4 operator * (const Vec4& v, const float f)		{return Vec4(v.x * f, v.y * f, v.z * f, v.w * f);}
inline Vec4 operator * (const float f, const Vec4& v)		{return Vec4(v.x * f, v.y * f, v.z * f, v.w * f);}
inline Vec4 operator / (const Vec4& a, const Vec4& b)	{return Vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);}
inline Vec4 operator / (const Vec4& v, float f)			{f = 1/f; return Vec4(v.x * f, v.y * f, v.z * f, v.w * f);}

// ******************************************************************************** //
// Vergleichsoperatoren
inline bool operator == (const Vec4& a, const Vec4& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == b.w;}
inline bool operator != (const Vec4& a, const Vec4& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != b.w;}
inline bool operator == (const Vec4& a, const Vec3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == 1.0f;}
inline bool operator != (const Vec4& a, const Vec3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != 1.0f;}
inline bool operator == (const Vec3& b, const Vec4& a) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == 1.0f;}
inline bool operator != (const Vec3& b, const Vec4& a) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != 1.0f;}

// ******************************************************************************** //
// Funktionen deklarieren
inline float	Vec4Length(const Vec4& v)												{return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline float	Vec4LengthSq(const Vec4& v)												{return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;}
inline Vec4	Vec4Normalize(const Vec4& v)												{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline Vec4	Vec4NormalizeEx(const Vec4& v)											{return v / InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline float	Vec4Dot(const Vec4& v1, const Vec4& v2)								{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;}
inline float	Vec4Angle(const Vec4& v1, const Vec4& v2)								{return Cos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w) * InvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z + v2.w * v2.w)));}
inline Vec4	Vec4LrpNormal(const Vec4& v1, const Vec4& v2, const float p)			{return Vec4NormalizeEx(v1 + p * (v2 - v1));}
inline Vec4	Vec4Min(const Vec4& v1, const Vec4& v2)								{return Vec4(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w));}
inline Vec4	Vec4Max(const Vec4& v1, const Vec4& v2)								{return Vec4(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w));}
inline Vec4	Vec4Lrp(const Vec4& v1, const Vec4& v2, const float f)				{return Vec4(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y), v1.z+f*(v2.z-v1.z), v1.w+f*(v2.w-v1.w));}
Vec4			Vec4Transform(const Vec4& v, const Matrix& m);																																														// 4D-Vektor mit Matrix multiplizieren
Vec4			Vec4Random();
Vec4			Vec4Cross(const Vec4& v1, const Vec4& v2, const Vec4& v3);


// ******************************************************************************** //
// Die Quaternion-Klasse (4D Hyperkugel)
/*class ORKTECK_API OrQuaternion
{
public:
	// Variablen
	Vec3	m_v3;
	float		m_fS;

	// Konstruktoren
	OrQuaternion() : m_v3(0.0f), m_fS(1)										{}
	OrQuaternion(const OrQuaternion& q) : m_v3(q.m_v3), m_fS(q.m_fS)		{}
	OrQuaternion(const Vec4& v) : m_v3(v.x, v.y, v.z), m_fS(v.w)		{}
	OrQuaternion(const float f, const Vec3& v) : m_fS(f), m_v3(v)		{}
	OrQuaternion(const Vec3& v) : m_fS(1), m_v3(v)						{}
	OrQuaternion(const float a, const float b, const float c, const float d) : m_v3(a, b, c), m_fS(d)	{}
	OrQuaternion(const Vec3& v, float f)							// erzeugt die Quaternion anhand von Achsen
	{
		m_fS = cosf(f / 2);
		m_v3 = v;
		m_v3 *= sinf(f / 2);
	}

	// Zuweisungsoperatoren
	OrQuaternion&	operator+=(const OrQuaternion& q) { m_v3 += q.m_v3; m_fS += q.m_fS; return *this; }
	OrQuaternion&	operator+=(const float f) { m_v3 += f; m_fS += f; return *this; }
	OrQuaternion&	operator-=(const OrQuaternion& q) { m_v3 -= q.m_v3; m_fS -= q.m_fS; return *this; }
	OrQuaternion&	operator-=(const float f) { m_v3 -= f; m_fS -= f; return *this; }
	OrQuaternion&	operator*=(float f) { m_v3 *= f; m_fS *= f; return *this; }
	OrQuaternion&	operator*=(const OrQuaternion& q) { m_fS *= q.m_fS; m_fS -= OrVec3Dot(m_v3, q.m_v3); m_v3 = m_fS * q.m_v3 + m_v3 * q.m_fS + OrVec3Cross(m_v3, q.m_v3); return *this;}
	OrQuaternion&	operator/=(float f) { m_v3 /= f; m_fS /= f; return *this; }
//	OrQuaternion&	operator/=(const OrQuaternion& q) { m_v3 /= f; m_fS /= f; return *this;}
	OrQuaternion&	operator=(const OrQuaternion& q) { m_v3 = q.m_v3; m_fS = q.m_fS; return *this; }
};

// Arithmetische Operatoren
inline OrQuaternion operator + (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS + b.m_fS, a.m_v3 + b.m_v3);}
inline OrQuaternion operator - (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS - b.m_fS, a.m_v3 - b.m_v3);}
inline OrQuaternion operator - (const OrQuaternion& v)							{return OrQuaternion(-v.m_fS, -v.m_v3);}
inline OrQuaternion operator * (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS * b.m_fS - OrVec3Dot(a.m_v3, b.m_v3), a.m_fS * b.m_v3 + a.m_v3 * b.m_fS + OrVec3Cross(a.m_v3, b.m_v3));}
//inline OrQuaternion operator * (const OrQuaternion& v, const float f)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
//inline OrQuaternion operator * (const float f, const OrQuaternion& v)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
//inline OrQuaternion operator / (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion( --- );}
//inline OrQuaternion operator / (const OrQuaternion& v, const float f)			{return OrQuaternion(v.m_v4 / f);}

// Vergleichsoperatoren
inline bool operator == (const OrQuaternion& a, const OrQuaternion& b) {if(a.m_v3.x != b.m_v3.x) return false; if(a.m_v3.y != b.m_v3.y) return false; if(a.m_v3.z != b.m_v3.z) return false; return a.m_fS == b.m_fS;}
inline bool operator != (const OrQuaternion& a, const OrQuaternion& b) {if(a.m_v3.x != b.m_v3.x) return true; if(a.m_v3.y != b.m_v3.y) return true; if(a.m_v3.z != b.m_v3.z) return true; return a.m_fS != b.m_fS;}

// ******************************************************************************** //
// Funktionen deklarieren
inline OrQuaternion&	OrQuaternionNormalizeEx(OrQuaternion& q)												{float mag = Sqrt(q.m_fS * q.m_fS + OrVec3Dot(q.m_v3, q.m_v3)); if (mag > 0.0000001) { q.m_fS /= mag; q.m_v3 /= mag;} else {q.m_fS = 1; q.m_v3 = Vec3(0.0f, 0.0f, 0.0f);} return q;}
inline OrQuaternion&	OrQuaternionNormalize(OrQuaternion& q)													{float mag = Sqrt(q.m_fS * q.m_fS + OrVec3Dot(q.m_v3, q.m_v3)); q.m_fS /= mag; q.m_v3 /= mag; return q;}
//inline void				OrQuaternionApplyRotation(Vec3* pRes, const Vec3& v)						{*pRes = *pRes * OrQuaternion(0, v) * OrQuaternion(pRes->m_fS, -pRes->m_v3);}
// optimiertere Varianten
inline void				OrQuaternionApplyRotation(const OrQuaternion& q1, const Vec3& v, Vec3* pRes)	{Vec3 vq1 = q1.m_fS * v + OrVec3Cross(q1.m_v3, v); Vec3 iq1 = -q1.m_v3; *pRes = OrVec3Dot(q1.m_v3, v) * iq1 + vq1 * q1.m_fS + OrVec3Cross(vq1, iq1);}
inline void				OrQuaternionApplyRotation(const OrQuaternion& q1, Vec3* pRes)						{Vec3 vq1 = q1.m_fS * *pRes + OrVec3Cross(q1.m_v3, *pRes); Vec3 iq1 = -q1.m_v3; *pRes = OrVec3Dot(q1.m_v3, *pRes) * iq1 + vq1 * q1.m_fS + OrVec3Cross(vq1, iq1);}
inline OrQuaternion		OrQuaternionInterpolate(const OrQuaternion& q1, const OrQuaternion& q2, float f)
{
	OrQuaternion	result;

	float	f0, f1;

	float	cos_omega = OrVec3Dot(q1.m_v3, q2.m_v3) + q1.m_fS * q2.m_fS;
	OrQuaternion	qtemp(q2);

	// angleichen wenn n�tig.
	if (cos_omega < 0) {
		cos_omega = -cos_omega;
		qtemp = -qtemp;
	}

	if (cos_omega < 0.99) {
		// Spherische interp. durchf�hren
		float	omega = acosf(cos_omega);
		float	sin_omega = sinf(omega);
		f0 = sinf((1 - f) * omega) / sin_omega;
		f1 = sinf(f * omega) / sin_omega;
	} else {
		// Quaternions are close; just do straight lerp and avoid division by near-zero.
		f0 = 1 - f;
		f1 = f;
	}
	
	result.m_v3 = q1.m_v3 * f0 + qtemp.m_v3 * f1;
	result.m_fS = q1.m_fS * f0 + qtemp.m_fS * f1;

	return OrQuaternionNormalize(result);
}
*/
}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //