// ******************************************************************************** //
// OrVec4.h																		//
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
	// The 4D-Vector-Class
	class Vec4
	{
	public:
		// Variables
		union
		{
			struct
			{
				float x;		// coordinates
				float y;
				float z;
				float w;
			};

			struct
			{
				float left;		// 2D rectangle
				float top;
				float right;
				float bottom;
			};

			struct
			{
				Vec2 xy;		// vector swizzle
				Vec2 zw;
			};

			struct
			{
				Vec3 xyz;
				float	r;
			};

			float c[4];			// Array of coordinates
		};

		// Constructors
		Vec4()																										{}
		Vec4(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)														{}
		Vec4(const Vec3& v) : x(v.x), y(v.y), z(v.z), w(1.0f)														{}
		Vec4(const Vec3& v, const float fw) : x(v.x), y(v.y), z(v.z), w(fw)											{}
		Vec4(const float fx, const Vec3& v) : x(fx), y(v.x), z(v.y), w(v.z)											{}
		Vec4(const Vec2& v) : x(v.x), y(v.y), z(1.0f), w(1.0f)														{}
		Vec4(const Vec2& v, float fz, float fw) : x(v.x), y(v.y), z(fz), w(fw)										{}
		Vec4(float fx, const Vec2& v, float fw) : x(fx), y(v.x), z(v.y), w(fw)										{}
		Vec4(float fx, float fy, const Vec2& v) : x(fx), y(fy), z(v.x), w(v.y)										{}
		Vec4(const Vec2& v, const Vec2& v2) : x(v.x), y(v.y), z(v2.x), w(v2.y)										{}
		Vec4(const float f) : x(f), y(f), z(f), w(f)																{}
		Vec4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w)			{}
		Vec4(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2]), w(pfComponent[3])	{}

		// Casting-operators
		operator float* ()					{return (float*)(c);}
		operator const float* () const		{return (const float*)(c);}
		operator Vec3 () const				{return xyz;}

		// Casting to "color"
		operator uint32 () const				{return (byte(Clamp(x*255.0, 0.0, 255.0))<<24) | (byte(Clamp(y*255.0, 0.0, 255.0))<<16) | (byte(Clamp(z*255.0, 0.0, 255.0))<<8) | byte(Clamp(w*255.0, 0.0, 255.0));}

		// Assignment operators
		Vec4& operator = (const Vec4& v)	{x = v.x; y = v.y; z = v.z; w = v.w; return *this;}
		Vec4& operator = (const float f)	{x = f; y = f; z = f; w = f; return *this;}
		Vec4& operator += (const Vec4& v)	{x += v.x; y += v.y; z += v.z; w += v.w; return *this;}
		Vec4& operator -= (const Vec4& v)	{x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;}
		Vec4& operator *= (const Vec4& v)	{x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;}
		Vec4& operator *= (const float f)	{x *= f; y *= f; z *= f; w *= f; return *this;}
		Vec4& operator /= (const Vec4& v)	{x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;}
		Vec4& operator /= (float f)			{f = 1/f; x *= f; y *= f; z *= f; w *= f; return *this;}

		// ******************************************************************************** //
		// Arithmetic operators
		inline Vec4 operator + (const Vec4& b) const	{return Vec4(x + b.x, y + b.y, z + b.z, w + b.w);}
		inline Vec4 operator - (const Vec4& b) const	{return Vec4(x - b.x, y - b.y, z - b.z, w - b.w);}
		inline Vec4 operator - () const					{return Vec4(-x, -y, -z, -w);}
		inline Vec4 operator * (const Vec4& b) const	{return Vec4(x * b.x, y * b.y, z * b.z, w * b.w);}
		inline Vec4 operator * (const float f) const	{return Vec4(x * f, y * f, z * f, w * f);}
		inline Vec4 operator / (const Vec4& b) const	{return Vec4(x / b.x, y / b.y, z / b.z, w / b.w);}
		inline Vec4 operator / (float f) const			{f = 1/f; return Vec4(x * f, y * f, z * f, w * f);}

		// ******************************************************************************** //
		// Comparison operators
		inline bool operator == (const Vec4& b) {if(x != b.x) return false; if(y != b.y) return false; if(z != b.z) return false; return w == b.w;}
		inline bool operator != (const Vec4& b) {if(x != b.x) return true; if(y != b.y) return true; if(z != b.z) return true; return w != b.w;}
		inline bool operator == (const Vec3& b) {if(x != b.x) return false; if(y != b.y) return false; if(z != b.z) return false; return w == 1.0f;}
		inline bool operator != (const Vec3& b) {if(x != b.x) return true; if(y != b.y) return true; if(z != b.z) return true; return w != 1.0f;}

	
		// ******************************************************************************** //
		// Vector4 functions
		// The non-static and non-const functions always change the calling object
		inline float		Length() const											{return Sqrt(x * x + y * y + z * z + w * w);}
		inline float		LengthSq() const										{return x * x + y * y + z * z + w * w;}
		inline const Vec4&	Normalize()												{*this *= InvSqrt(x * x + y * y + z * z + w * w); return *this;}
		inline static Vec4	Normalize(const Vec4& v)								{return v * InvSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);}
		inline const Vec4&	NormalizeEx()											{float fL = Sqrt(x * x + y * y + z * z + w * w); *this /= OrE::Math::Max(fL, 0.0000000000000000001f); return *this;}
		inline static Vec4	NormalizeEx(const Vec4& v)								{float fL = Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); return v / OrE::Math::Max(fL, 0.0000000000000000001f);}
		inline float		Dot(const Vec4& v) const								{return x * v.x + y * v.y + z * v.z + w * v.w;}
	//	inline static float	Dot(const Vec4& v1, const Vec4& v2)						{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;}
		inline float		Angle(const Vec4& v) const								{return Arccos((x * v.x + y * v.y + z * v.z + w * v.w) * InvSqrt((x * x + y * y + z * z + w * w) * (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w)));}
	//	inline static float	Angle(const Vec4& v1, const Vec4& v2)					{return Arccos((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w) * InvSqrt((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z + v1.w * v1.w) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z + v2.w * v2.w)));}
		static Vec4			RandomDirection();
		static Vec4			RandomPosition();
		const Vec4&			Cross(const Vec4& v2, const Vec4& v3);
		static Vec4			Cross(const Vec4& v1, const Vec4& v2, const Vec4& v3);
	};

	// ******************************************************************************** //
	inline Vec4 operator * (const float f, const Vec4& v)		{return Vec4(v.x * f, v.y * f, v.z * f, v.w * f);}
	inline bool operator == (const Vec3& b, const Vec4& a)		{if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == 1.0f;}
	inline bool operator != (const Vec3& b, const Vec4& a)		{if(a.x != b.x) return true; if(a.y != b.y) return true; if(a.z != b.z) return true; return a.w != 1.0f;}


	// ******************************************************************************** //
	inline Vec4	Min(const Vec4& v1, const Vec4& v2)							{return Vec4(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w));}
	inline Vec4	Max(const Vec4& v1, const Vec4& v2)							{return Vec4(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w));}
	inline Vec4	Lerp(const Vec4& v1, const Vec4& v2, const float f)			{return Vec4(v1.x+f*(v2.x-v1.x), v1.y+f*(v2.y-v1.y), v1.z+f*(v2.z-v1.z), v1.w+f*(v2.w-v1.w));}
	inline Vec4	Abs(const Vec4& v1)											{return Vec4(Abs(v1.x), Abs(v1.y), Abs(v1.z), Abs(v1.w));}
	// The spherical interpolation applies only to normal vectors
	Vec4 Slerp(const Vec4& v1, const Vec4& v2, const float t);




	// ******************************************************************************** //
	// Union quaternions (4D - complex numbers) for rotation in 3D space
	// Scalar operations are disabled because they only change the length of the quaternion
	class Quaternion
	{
	private:
		Quaternion& operator *= (float f) { i*=f; j*=f; k*=f; r*=f; return *this; }
	public:
		// Variables
		float i, j, k, r;

		// Constructors
		Quaternion() : i(0), j(0), k(0), r(1)										{}
		Quaternion( const Quaternion& _q ) : i(_q.i), j(_q.j), k(_q.k), r(_q.r)		{}

		// From unnormalized vector
		Quaternion( const Vec4& v ) : i(v.x), j(v.y), k(v.z), r(v.w)					{ Normalize(); }

		// From arbitrary components (attention: normalization - use tricks if you
		// know what you do (Quaternion q(); q.i=...) )
		Quaternion( float r, float i, float j, float k ) : i(i), j(j), k(k), r(r)		{ Normalize(); }

		// arbitrary NORMALIZED axis + Angle.
		Quaternion( const Vec3& _vAxis, float _a )
		{
			// Using -sin to get the same result as for the matrix.
			// TODO: Matrix wrong? - the conjugated = inverse rotation
			float fSin = float( - sin( _a * 0.5 ) );
			r = float( cos( _a * 0.5 ) );
			i = _vAxis.x * fSin;
			j = _vAxis.y * fSin;
			k = _vAxis.z * fSin;
		}

		// From Matrix
		Quaternion(const Matrix& _m);

		// From Euler angles
		Quaternion( float _fYaw, float _fPitch, float _fRoll );
		// C++11 variant:  Quaternion( float _fYaw, float _fPitch, float _fRoll ) : Quaternion( MatrixRotation( _fYaw, _fPitch, _fRoll ) ) {}

		// Functions
		inline float				Length() const									{return Sqrt(i * i + j * j + k * k + r * r);}
		inline float				LengthSq() const								{return i * i + j * j + k * k + r * r;}
		inline const Quaternion&	Normalize()										{*this *= InvSqrt(i * i + j * j + k * k + r * r); return *this;}
		inline static Quaternion	Normalize(const Quaternion& a)					{return Quaternion( a.r, a.i, a.j, a.k );}
		inline const Quaternion&	NormalizeEx()									{float fL = Sqrt(i * i + j * j + k * k + r * r); *this *= 1.0f/OrE::Math::Max(fL, 0.0000001f); return *this;}
		inline static Quaternion	NormalizeEx(const Quaternion& a)				{return Quaternion( a.r, a.i, a.j, a.k );}	// Same as non Ex version
		inline float				Dot(const Quaternion& b) const					{return i * b.i + j * b.j + k * b.k + r * b.r;}
		inline float				Angle(const Quaternion& b) const				{return Arccos( Dot( b ) );}

		// Casting-operators
		operator float* ()					{return (float*)(&i);}
		operator const float* () const		{return (const float*)(&i);}
		operator Vec4 () const				{return *this;}
		operator Matrix () const;

		// Assignment operators
		Quaternion& operator += (const Quaternion& b) { i+=b.i; j+=b.j; k+=b.k; r+=b.r; Normalize(); return *this; }
		Quaternion& operator -= (const Quaternion& b) { i-=b.i; j-=b.j; k-=b.k; r-=b.r; Normalize(); return *this; }
		Quaternion& operator *= (const Quaternion& b)
		{
			// Quaternion multiplication - non commutative (a*b != a*b)
			float nr = r*b.r - i*b.i - j*b.j - k*b.k;
			float ni = r*b.i + i*b.r + j*b.k - k*b.j;
			float nj = r*b.j + j*b.r + k*b.i - i*b.k;
				   k = r*b.k + k*b.r + i*b.j - j*b.i;
			r = nr;
			i = ni;
			j = nj;
			return *this;
		}
		Quaternion& operator /= (float f) { f = 1/f; i*=f; j*=f; k*=f; return *this; }

		// a/=b  <=>  a=a*(b^-1)  <=>  a=a*~b (~b = b conjugated)
		Quaternion& operator /= (const Quaternion& b)
		{
			float nr =   r*b.r + i*b.i + j*b.j + k*b.k;
			float ni = - r*b.i + i*b.r - j*b.k + k*b.j;
			float nj = - r*b.j + j*b.r - k*b.i + i*b.k;
				   k = - r*b.k + k*b.r - i*b.j + j*b.i;
			r = nr;
			i = ni;
			j = nj;
			return *this;
		}
		Quaternion& operator = (const Quaternion& _q) { i=_q.i; j=_q.j; k=_q.k; r=_q.r; return *this; }
	};

	// ******************************************************************************** //
	// Arithmetic operators
	inline Quaternion operator + (const Quaternion& a, const Quaternion& b)	{return Quaternion(a.r+b.r, a.i+b.i, a.j+b.j, a.k+b.k);}
	inline Quaternion operator - (const Quaternion& a, const Quaternion& b)	{return Quaternion(a.r-b.r, a.i-b.i, a.j-b.j, a.k-b.k);}
	inline Quaternion operator - (const Quaternion& a)							{Quaternion b; b.r=-a.r; b.i=-a.i; b.j=-a.j; b.k=-a.k; return b;}	// Avoiding the normalize - no use of component constructor
	// Conjugation / inversion of a
	inline Quaternion operator ~ (const Quaternion& a)							{Quaternion b; b.r=a.r; b.i=-a.i; b.j=-a.j; b.k=-a.k; return b;}	// Avoiding the normalize - no use of component constructor
	// Quaternion multiplication - non commutative (a*b != a*b)
	inline Quaternion operator * (const Quaternion& a, const Quaternion& b)
	{
		// Return with implicit normalization
		return Quaternion(
			a.r*b.r - a.i*b.i - a.j*b.j - a.k*b.k,
			a.r*b.i + a.i*b.r + a.j*b.k - a.k*b.j,
			a.r*b.j + a.j*b.r + a.k*b.i - a.i*b.k,
			a.r*b.k + a.k*b.r + a.i*b.j - a.j*b.i
			);
	}

	// a/b  <=>  a/(b^-1)  <=>  a/~b (~b = b conjugated)
	inline Quaternion operator / (const Quaternion& a, const Quaternion& b)
	{
		// Return with implicit normalization
		return Quaternion(
			  a.r*b.r + a.i*b.i + a.j*b.j + a.k*b.k,
			- a.r*b.i + a.i*b.r - a.j*b.k + a.k*b.j,
			- a.r*b.j + a.j*b.r - a.k*b.i + a.i*b.k,
			- a.r*b.k + a.k*b.r - a.i*b.j + a.j*b.i
			);
	}

	// ******************************************************************************** //
	// Comparison operators
	#define Quaternion_EPSILON 0.000001f
	inline bool operator == (const Quaternion& a, const Quaternion& b) { if(Abs(a.r-b.r) > Quaternion_EPSILON) return false; if(Abs(a.i-b.i) > Quaternion_EPSILON) return false; if(Abs(a.j-b.j) > Quaternion_EPSILON) return false; return Abs(a.k-b.k) < Quaternion_EPSILON; }
	inline bool operator != (const Quaternion& a, const Quaternion& b) { if(Abs(a.r-b.r) >= Quaternion_EPSILON) return true; if(Abs(a.i-b.i) >= Quaternion_EPSILON) return true; if(Abs(a.j-b.j) >= Quaternion_EPSILON) return true; return Abs(a.k-b.k) >= Quaternion_EPSILON; }

	//Quaternion operator * (const Quaternion& a, const float f)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
	//Quaternion operator * (const float f, const Quaternion& a)			{return OrQuaternion(v.m_fS * f; v.m_v3 * f);}
	//Quaternion operator / (const Quaternion& v, const float f)			{return OrQuaternion(v.m_v4 / f);}

	// ******************************************************************************** //
	// Functions
	// The spherical interpolation for union quaternions
	Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float t);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //