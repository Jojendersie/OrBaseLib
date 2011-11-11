/*************************************************************************

	OrVector4.h
	===========
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 4D-Vektoren und Quaternionen

	Autor: Johannes Jendersie
	Verwendete Quellen: keine

*************************************************************************/

#pragma once

// ******************************************************************** //
// Die 4D-Vektor-Klasse
class OrVector4
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
			OrVector2 v1;	// Vektoren
			OrVector2 v2;
		};

		struct
		{
			OrVector3 v3;
			float	r;
		};

		float			c[4];			// Array der Koordinaten
	};

	// Konstruktoren
	OrVector4()																										{}
	OrVector4(const OrVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w)													{}
	OrVector4(const OrVector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f)													{}
	OrVector4(const OrVector3& v, const float fw) : x(v.x), y(v.y), z(v.z), w(fw)									{}
	OrVector4(const OrVector2& v) : x(v.x), y(v.y), z(1.0f), w(1.0f)												{}
	OrVector4(const OrVector2& v, const OrVector2& v2) : x(v.x), y(v.y), z(v2.x), w(v2.y)							{}
	OrVector4(const float f) : x(f), y(f), z(f), w(f)																{}
	OrVector4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w)			{}
	OrVector4(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2]), w(pfComponent[3]){}

	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}
	operator OrVector3 ()				{return v3;}

	// Zuweisungsoperatoren
	OrVector4& operator = (const OrVector4& v)	{x = v.x; y = v.y; z = v.z; w = v.w; return *this;}
	OrVector4& operator = (const float f)		{x = f; y = f; z = f; w = f; return *this;}
	OrVector4& operator += (const OrVector4& v)	{x += v.x; y += v.y; z += v.z; w += v.w; return *this;}
	OrVector4& operator -= (const OrVector4& v)	{x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;}
	OrVector4& operator *= (const OrVector4& v)	{x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;}
	OrVector4& operator *= (const float f)		{x *= f; y *= f; z *= f; w *= f; return *this;}
	OrVector4& operator /= (const OrVector4& v)	{x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;}
	OrVector4& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; w *= f; return *this;}
};

// Arithmetische Operatoren
inline OrVector4 operator + (const OrVector4& a, const OrVector4& b)	{return OrVector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);}
inline OrVector4 operator - (const OrVector4& a, const OrVector4& b)	{return OrVector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);}
inline OrVector4 operator - (const OrVector4& v)						{return OrVector4(-v.x, -v.y, -v.z, -v.w);}
inline OrVector4 operator * (const OrVector4& a, const OrVector4& b)	{return OrVector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);}
inline OrVector4 operator * (const OrVector4& v, const float f)			{return OrVector4(v.x * f, v.y * f, v.z * f, v.w * f);}
inline OrVector4 operator * (const float f, const OrVector4& v)			{return OrVector4(v.x * f, v.y * f, v.z * f, v.w * f);}
inline OrVector4 operator / (const OrVector4& a, const OrVector4& b)	{return OrVector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);}
inline OrVector4 operator / (const OrVector4& v, float f)				{f = 1/f; return OrVector4(v.x * f, v.y * f, v.z * f, v.w * f);}

// Vergleichsoperatoren
inline bool operator == (const OrVector4& a, const OrVector4& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == b.w;}
inline bool operator != (const OrVector4& a, const OrVector4& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != b.w;}
inline bool operator == (const OrVector4& a, const OrVector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == 1.0f;}
inline bool operator != (const OrVector4& a, const OrVector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != 1.0f;}
inline bool operator == (const OrVector3& b, const OrVector4& a) {if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == 1.0f;}
inline bool operator != (const OrVector3& b, const OrVector4& a) {if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != 1.0f;}

// ******************************************************************** //
// Funktionen deklarieren
inline float		OrVec4Length(const OrVector4& v)													{return OrSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline float		OrVec4LengthSq(const OrVector4& v)													{return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;}
inline OrVector4	OrVec4Normalize(const OrVector4& v)													{return v * OrInvSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline OrVector4	OrVec4NormalizeEx(const OrVector4& v)												{return v / OrInvSqrtEx(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
inline OrVector4	OrVec4Cross(const OrVector4& v1, const OrVector4& v2)								{return OrVector4((v1.y * v2.z - v1.z * v2.y + v1.z * v2.w - v1.w * v2.z + v1.y * v2.w - v1.w * v2.y), 
																								                          (v1.z * v2.x - v1.x * v2.z + v1.w * v2.x - v1.x * v2.w + v1.z * v2.w - v1.w * v2.z),
                                                                                                                          (v1.x * v2.y - v1.y * v2.x + v1.w * v2.y - v1.y * v2.w + v1.w * v2.x - v1.x * v2.w),
																														  (v1.x * v2.y - v1.y * v2.x + v1.y * v2.z - v1.z * v2.y + v1.x * v2.z - v1.z * v2.x));}
inline float		OrVec4Dot(const OrVector4& v1, const OrVector4& v2)									{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;}
inline float		OrVec4Angle(const OrVector4& v1, const OrVector4& v2)								{return OrCos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w) * OrInvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z + v2.w * v2.w)));}
inline OrVector4	OrVec4InterpolateCoords(const OrVector4& v1, const OrVector4& v2, const float p)	{return v1 + p * (v2 - v1);}
inline OrVector4	OrVec4InterpolateNormal(const OrVector4& v1, const OrVector4& v2, const float p)	{return OrVec4NormalizeEx(v1 + p * (v2 - v1));}
inline OrVector4	OrVec4Min(const OrVector4& v1, const OrVector4& v2)									{return OrVector4(OrMinf(v1.x, v2.x), OrMinf(v1.y, v2.y), OrMinf(v1.z, v2.z), OrMinf(v1.w, v2.w));}
inline OrVector4	OrVec4Max(const OrVector4& v1, const OrVector4& v2)									{return OrVector4(OrMaxf(v1.x, v2.x), OrMaxf(v1.y, v2.y), OrMaxf(v1.z, v2.z), OrMaxf(v1.w, v2.w));}
OrVector4			OrVec4Random();


// ******************************************************************** //
// Die Quaternion-Klasse (4D Hyperkugel)
/*class ORKTECK_API OrQuaternion
{
public:
	// Variablen
	OrVector3	m_v3;
	float		m_fS;

	// Konstruktoren
	OrQuaternion() : m_v3(0.0f), m_fS(1)										{}
	OrQuaternion(const OrQuaternion& q) : m_v3(q.m_v3), m_fS(q.m_fS)		{}
	OrQuaternion(const OrVector4& v) : m_v3(v.x, v.y, v.z), m_fS(v.w)		{}
	OrQuaternion(const float f, const OrVector3& v) : m_fS(f), m_v3(v)		{}
	OrQuaternion(const OrVector3& v) : m_fS(1), m_v3(v)						{}
	OrQuaternion(const float a, const float b, const float c, const float d) : m_v3(a, b, c), m_fS(d)	{}
	OrQuaternion(const OrVector3& v, float f)							// erzeugt die Quaternion anhand von Achsen
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
	OrQuaternion&	operator*=(const OrQuaternion& q) { m_fS *= q.m_fS; m_fS -= OrVector3Dot(m_v3, q.m_v3); m_v3 = m_fS * q.m_v3 + m_v3 * q.m_fS + OrVector3Cross(m_v3, q.m_v3); return *this;}
	OrQuaternion&	operator/=(float f) { m_v3 /= f; m_fS /= f; return *this; }
//	OrQuaternion&	operator/=(const OrQuaternion& q) { m_v3 /= f; m_fS /= f; return *this;}
	OrQuaternion&	operator=(const OrQuaternion& q) { m_v3 = q.m_v3; m_fS = q.m_fS; return *this; }
};

// Arithmetische Operatoren
inline OrQuaternion operator + (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS + b.m_fS, a.m_v3 + b.m_v3);}
inline OrQuaternion operator - (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS - b.m_fS, a.m_v3 - b.m_v3);}
inline OrQuaternion operator - (const OrQuaternion& v)							{return OrQuaternion(-v.m_fS, -v.m_v3);}
inline OrQuaternion operator * (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion(a.m_fS * b.m_fS - OrVector3Dot(a.m_v3, b.m_v3), a.m_fS * b.m_v3 + a.m_v3 * b.m_fS + OrVector3Cross(a.m_v3, b.m_v3));}
//inline OrQuaternion operator * (const OrQuaternion& v, const float f)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
//inline OrQuaternion operator * (const float f, const OrQuaternion& v)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
//inline OrQuaternion operator / (const OrQuaternion& a, const OrQuaternion& b)	{return OrQuaternion( --- );}
//inline OrQuaternion operator / (const OrQuaternion& v, const float f)			{return OrQuaternion(v.m_v4 / f);}

// Vergleichsoperatoren
inline bool operator == (const OrQuaternion& a, const OrQuaternion& b) {if(a.m_v3.x != b.m_v3.x) return false; if(a.m_v3.y != b.m_v3.y) return false; if(a.m_v3.z != b.m_v3.z) return false; return a.m_fS == b.m_fS;}
inline bool operator != (const OrQuaternion& a, const OrQuaternion& b) {if(a.m_v3.x != b.m_v3.x) return true; if(a.m_v3.y != b.m_v3.y) return true; if(a.m_v3.z != b.m_v3.z) return true; return a.m_fS != b.m_fS;}

// ******************************************************************** //
// Funktionen deklarieren
inline OrQuaternion&	OrQuaternionNormalizeEx(OrQuaternion& q)												{float mag = OrSqrt(q.m_fS * q.m_fS + OrVector3Dot(q.m_v3, q.m_v3)); if (mag > 0.0000001) { q.m_fS /= mag; q.m_v3 /= mag;} else {q.m_fS = 1; q.m_v3 = OrVector3(0.0f, 0.0f, 0.0f);} return q;}
inline OrQuaternion&	OrQuaternionNormalize(OrQuaternion& q)													{float mag = OrSqrt(q.m_fS * q.m_fS + OrVector3Dot(q.m_v3, q.m_v3)); q.m_fS /= mag; q.m_v3 /= mag; return q;}
//inline void				OrQuaternionApplyRotation(OrVector3* pRes, const OrVector3& v)						{*pRes = *pRes * OrQuaternion(0, v) * OrQuaternion(pRes->m_fS, -pRes->m_v3);}
// optimiertere Varianten
inline void				OrQuaternionApplyRotation(const OrQuaternion& q1, const OrVector3& v, OrVector3* pRes)	{OrVector3 vq1 = q1.m_fS * v + OrVector3Cross(q1.m_v3, v); OrVector3 iq1 = -q1.m_v3; *pRes = OrVector3Dot(q1.m_v3, v) * iq1 + vq1 * q1.m_fS + OrVector3Cross(vq1, iq1);}
inline void				OrQuaternionApplyRotation(const OrQuaternion& q1, OrVector3* pRes)						{OrVector3 vq1 = q1.m_fS * *pRes + OrVector3Cross(q1.m_v3, *pRes); OrVector3 iq1 = -q1.m_v3; *pRes = OrVector3Dot(q1.m_v3, *pRes) * iq1 + vq1 * q1.m_fS + OrVector3Cross(vq1, iq1);}
inline OrQuaternion		OrQuaternionInterpolate(const OrQuaternion& q1, const OrQuaternion& q2, float f)
{
	OrQuaternion	result;

	float	f0, f1;

	float	cos_omega = OrVector3Dot(q1.m_v3, q2.m_v3) + q1.m_fS * q2.m_fS;
	OrQuaternion	qtemp(q2);

	// angleichen wenn nötig.
	if (cos_omega < 0) {
		cos_omega = -cos_omega;
		qtemp = -qtemp;
	}

	if (cos_omega < 0.99) {
		// Spherische interp. durchführen
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
// ******************************************************************** //