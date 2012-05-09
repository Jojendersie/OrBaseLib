// ******************************************************************************** //
// OrFastMath.h																		//
// ============																		//
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
//		Cast float->dword:				 25.0										//
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
// Min und Max - Funktionen, damit nicht irgendwelche Dinge importiert werden.
template <class T> const T Min(const T a, const T b)		{return a < b ? a : b;}
template <class T> const T Max(const T a, const T b)		{return a > b ? a : b;}
template <class T> const T Clamp(const T a, const T min, const T max)	{return a < min ? min : (a > max ? max : a);}
template <class T> const T Abs(const T a)					{return a < 0 ? -a : a;}
template <class T> const T Lerp(const T a, const T b, const T f)		{return a + (b-a)*f;}
__forceinline int Sgn(const int a)							{return (a<0)?-1:1;}			// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
__forceinline float Sgn(const float a)						{return (a<0)?-1.0f:1.0f;}		// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
__forceinline double Sgn(const double a)					{return (a<0)?-1.0:1.0;}		// Not mathematicly: [-oo,0) -> -1; [0,oo] -> 1;
__forceinline int Floor(const float a)						{int r=(int)a; return r - (int)((a<0)&&(a-r!=0.0f));}		// Round down
__forceinline int Ceil(const float a)						{int r=(int)a; return r + (int)((a>0)&&(a-r!=0.0f));}		// Round up

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
// Berechnet den Arcuscosinus: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
__forceinline float Arccos( float r )	{return acos(r);}

// ******************************************************************************** //
// Berechnet den Arcussinus: arctan( r / -sqr( 1.0f - r * r ) )
__forceinline float Arcsin( float r )	{return asin(r);}

// ******************************************************************************** //
// Berechnet den Arcustangens: arctan( r )
__forceinline float Arctan( float r )	{return atan(r);}

// ******************************************************************************** //
// Berechnet den Sinus (Bogenmaß)
__forceinline float Sin( float r )		{return sin(r);}

// ******************************************************************************** //
// Berechnet den Kosinus (Bogenmaß)
__forceinline float Cos( float r )		{return cos(r);}

// ******************************************************************************** //
// Berechnet den Tangens (Bogenmaß)
__forceinline float Tan( float r )		{return tan(r);}

// ******************************************************************************** //
// quadriert eine Zahl
__forceinline float Sqr( float r )		{return r*r;}

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes
__forceinline float Sqrt( float r )		{return sqrt(r);}

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück (sehr schnell)
// Accuracy: smaller than 0.1%
float InvSqrt(float fValue);
// Accuracy: worst case error 4%, avg 2.1%
float _InvSqrt(float fValue);

// ******************************************************************************** //
// Nächst größere 2 er Potenz

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück (sehr schnell)
/*inline __declspec(naked) float __fastcall InvSqrt(float fValue)
{
	__asm
	{
		mov eax, 0be6eb508h
		mov dword ptr[esp-12],03fc00000h
		sub eax, dword ptr[esp + 4]
		sub dword ptr[esp+4], 800000h
		shr eax, 1
		mov dword ptr[esp -  8], eax

		fld dword ptr[esp -  8]
		fmul st, st
		fld dword ptr[esp -  8]
		fxch st(1)
		fmul dword ptr[esp +  4]
		fld dword ptr[esp - 12]
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
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück
// etwas sicherer, aber auch ungenauer und langsamer
//float InvSqrtEx( float r );

// ******************************************************************************** //
// Logarithmus naturalis
// Accuracy: avg. error 0.5%. Do not use between 0 and 1! maxerror from 400%
float Ln( float r );
// Logarithmus dualis
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
// Gray Code convertions
unsigned int GrayCodeToNum(unsigned int _uiGrayCode);
unsigned int NumToGrayCode(unsigned int _uiNum);

// ******************************************************************************** //
// Parallel bit counting to estimate the number of 1 bits in a number
qword CountBits64(qword n);
dword CountBits32(dword n);
word CountBits16(word n);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //