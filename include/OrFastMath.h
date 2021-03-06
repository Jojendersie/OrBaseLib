// ******************************************************************************** //
// OrFastMath.h																		//
// ============																		//
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
//	Fast mathematical base functions												//
//	Sources: http://www.website.masmforum.com/tutorials/fptute/fpuchap11.htm#f2xm1	//
//	http://aggregate.org/MAGIC/														//
//	http://graphics.stanford.edu/~seander/bithacks.html								//
//																					//
//	Benchmarks (in s/1 mrd calls):													//
//		Float addition:					 2.513										//
//		Double addition:				 2.747										//
//		Float multiplication:			 2.95										//
//		Double multiplication:			 3.43										//
//		Float division:					11.5										//
//		Double division:				12.3										//
//		Float InvSqrt:					94.0										//
//		Float InvSqrt2:					76.0										//
//		Float sqrt:					   136.4										//
//		Float Ln:					    94.0										//
//		Float log:					   203.0										//
//		Float Min/Max:				    65.0										//
//		Integer Min/Max:				62.0										//
//		Cast int->float:				 3.1										//
//		Cast float->int:				 7.0										//
//		Cast float->uint32:				 25.0										//
//		std::floor:					   174.7										//
//		Floor:							71.8										//
// ******************************************************************************** //

#pragma once

#include <cmath>

namespace OrE {
namespace Math {

const float fPi = 3.14159265358979f;
const double dPi = 3.1415926535897932384626433832795;
const float f2Pi = fPi*2.0f;
const double d2Pi = dPi*2.0;
const float fPi_2 = fPi*0.5f;
const double dPi_2 = dPi*0.5;
const float fE = 2.71828182845904f;
const double dE  = 2.71828182845904523536028747135266249;

// ******************************************************************************** //
// Min and Max - Functions, the standard functions are bad to find
template <class T> const T Min(const T a, const T b)		{return a < b ? a : b;}
template <class T> const T Max(const T a, const T b)		{return a > b ? a : b;}
template <class T> const T Clamp(const T a, const T min, const T max)	{return a < min ? min : (a > max ? max : a);}
template <class T> const T Abs(const T a)					{return a < 0 ? -a : a;}
template <class T> const T Lerp(const T a, const T b, const T f)		{return a + (b-a)*f;}
__forceinline int Sgn(const int a)							{return (a<0)?-1:1;}			// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
__forceinline float Sgn(const float a)						{return (a<0)?-1.0f:1.0f;}		// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
__forceinline double Sgn(const double a)					{return (a<0)?-1.0:1.0;}		// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
template <typename T> int Signum(T val)						{return (val < T(0)) ? -1 : (T(0) < val);}					// mathematically: [-oo,0) -> -1; 0 -> 0, (0,oo] -> 1
__forceinline int Floor(const float a)						{int r=(int)a; return r - (int)((a<0)&&(a-r!=0.0f));}		// Round down
__forceinline int Ceil(const float a)						{int r=(int)a; return r + (int)((a>0)&&(a-r!=0.0f));}		// Round up
__forceinline int Round(const float a)						{return Ceil(a+0.5f);}										// Round

// Bit hacking alternatives:
//__forceinline int Abs(const int a)						{int m=a>>31; return (a+m)^m;}	a little bit slower than the other variant

//	http://stereopsis.com/FPU.html#convert											//
/*inline int Int(double _dVal)
{
	//_dVal = (_dVal + 68719476736.0*1.5);
//	return (*(int*)&_dVal) >> 16; 
	_dVal = (_dVal + 1688849860263936.0);
	return (*(int*)&_dVal)>>2; 
}

inline int Int(float _fVal)
{
	return Int((double)_fVal);
}*/

// ******************************************************************************** //
// Arcuscosine: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
__forceinline float Arccos( float r )	{return acos(r);}

// ******************************************************************************** //
// Arcussine: arctan( r / -sqr( 1.0f - r * r ) )
__forceinline float Arcsin( float r )	{return asin(r);}

// ******************************************************************************** //
// Arctangent: arctan( r )
__forceinline float Arctan( float r )	{return atan(r);}

// ******************************************************************************** //
// Sine (Radian measure)
__forceinline float Sin( float r )		{return sin(r);}

// ******************************************************************************** //
// Cosine (Radian measure)
__forceinline float Cos( float r )		{return cos(r);}

// ******************************************************************************** //
// Tangent (Radian measure)
__forceinline float Tan( float r )		{return tan(r);}

// ******************************************************************************** //
// Square a number
__forceinline float Sqr( float r )		{return r*r;}

// ******************************************************************************** //
// The square root; standard function is faster than the ones below on modern hardware
__forceinline float Sqrt( float r )		{return sqrt(r);}

// ******************************************************************************** //
inline float InvSqrt(float fValue)	{return float(1.0/sqrt(double(fValue)));}
// DEPRECATED Version below: Is slower on modern hardware
// Square root (fast on old hardware)
// Accuracy: smaller than 0.1%
//float InvSqrt(float fValue);
// Accuracy: worst case error 4%, avg 2.1%
//float _InvSqrt(float fValue);

// ******************************************************************************** //

// ******************************************************************************** //
// 1/square root of a number
/*inline __declspec(naked) float __fastcall InvSqrt(float fValue)
{
	__asm
	{
		mov eax, 0be6eb508h
		mov uint32 ptr[esp-12],03fc00000h
		sub eax, uint32 ptr[esp + 4]
		sub uint32 ptr[esp+4], 800000h
		shr eax, 1
		mov uint32 ptr[esp -  8], eax

		fld uint32 ptr[esp -  8]
		fmul st, st
		fld uint32 ptr[esp -  8]
		fxch st(1)
		fmul uint32 ptr[esp +  4]
		fld uint32 ptr[esp - 12]
		fld st(0)
		fsub st,st(2)

		fld st(1)
		fxch st(1)
		fmul st(3),st
		fmul st(3),st
		fmulp st(4),st
		fsub st,st(2)

		fmul st(2),st
		fmul st(3),st
		fmulp st(2),st
		fxch st(1)
		fsubp st(1),st

		fmulp st(1), st
		ret 4
	}
}
*/


// ******************************************************************************** //
// Logarithm naturalism
// Accuracy: avg. error 0.5%. Do not use between 0 and 1! max error from 400%
float Ln( float r );
// Logarithm dualism
int _Ld( float r );
float Ld( float r );
int Ld( int r );

// ******************************************************************************** //
// Calculate the rounded (down) logarithm to the base 2
//int Log2(int iValue);

// ******************************************************************************** //
// exponential function base^exponent
__forceinline float Pow(float fBase, float fExponent)	{return pow(fBase, fExponent);}

// ******************************************************************************** //
// Gray Code conversions
unsigned int GrayCodeToNum(unsigned int _uiGrayCode);
unsigned int NumToGrayCode(unsigned int _uiNum);

// ******************************************************************************** //
// Parallel bit counting to estimate the number of 1 bits in a number
uint64 CountBits64(uint64 n);
uint32 CountBits32(uint32 n);
uint16 CountBits16(uint16 n);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //