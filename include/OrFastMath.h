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
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Math {

const float fPi = 3.14159265358979f;
const double dPi = 3.1415926535897932384626433832795;
const float f2Pi = fPi*2.0f;
const double d2Pi = dPi*2.0;
const float fPi_2 = fPi*0.5f;
const double dPi_2 = dPi*0.5;

// ******************************************************************************** //
// Min und Max - Funktionen, damit nicht irgendwelche Dinge importiert werden.
__forceinline float Maxf(const float a, const float b)		{return a > b ? a : b;}
__forceinline double Maxd(const double a, const double b)	{return a > b ? a : b;}
__forceinline int Max(const int a, const int b)				{return a > b ? a : b;}
__forceinline dword Maxu(const dword a, const dword b)		{return a > b ? a : b;}
__forceinline float Minf(const float a, const float b)		{return a < b ? a : b;}
__forceinline double Mind(const double a, const double b)	{return a < b ? a : b;}
__forceinline int Min(const int a, const int b)				{return a < b ? a : b;}
__forceinline dword Minu(const dword a, const dword b)		{return a < b ? a : b;}
__forceinline float Clampf(const float a, const float min, const float max)	{return a < min ? min : (a > max ? max : a);}
__forceinline dword Clampu(const dword a, const dword min, const dword max)	{return a < min ? min : (a > max ? max : a);}
__forceinline float Absf(const float a)						{return a < 0 ? -a: a;}
__forceinline int Abs(const int a)							{return a&0x7fffffff;}
__forceinline int Sgn(const int a)							{return (a&0x80000000) | 1;}		// Not mathematicly: [-oo,0] -> -1; (0,oo] -> 1;
__forceinline float Sgnf(const float a)						{return (a>0)?(1.0f):(-1.0f);}		// Not mathematicly: [-oo,0] -> -1; (0,oo] -> 1;

// ******************************************************************************** //
// Berechnet den Arcuscosinus: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
float Arccos( float r );

// ******************************************************************************** //
// Berechnet den Arcussinus: arctan( r / -sqr( 1.0f - r * r ) )
float Arcsin( float r );

// ******************************************************************************** //
// Berechnet den Arcustangens: arctan( r )
float Arctan( float r );

// ******************************************************************************** //
// Berechnet den Sinus (Bogenmaß)
float Sin( float r );

// ******************************************************************************** //
// Berechnet den Kosinus (Bogenmaß)
float Cos( float r );

// ******************************************************************************** //
// Berechnet den Tangens (Bogenmaß)
float Tan( float r );

// ******************************************************************************** //
// quadriert eine Zahl
float Sqr( float r );

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes
float Sqrt( float r );

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück (sehr schnell)
inline __declspec(naked) float __fastcall InvSqrt(float fValue)
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

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück
// etwas sicherer, aber auch ungenauer und langsamer
float InvSqrtEx( float r );

// ******************************************************************************** //
// Logarithmus naturalis
float Ln( float r );
// Logarithmus dualis
inline float Ld( float r )	{return Ln(r)*1.442695041f;}

// ******************************************************************************** //
// Calculate the rounded (down) logarithm to the base 2
int Log2(int iValue);

// ******************************************************************************** //
// exponential function base^exponent
float __fastcall Pow(float fBase, float fExponent);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //