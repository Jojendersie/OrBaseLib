/********************************************************************

	OrFastMath.h
	============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Schnelle Mathematische Funktionen

	Quellen: http://www.website.masmforum.com/tutorials/fptute/fpuchap11.htm#f2xm1

********************************************************************/

#pragma once

const float fPi = 3.14159265358979f;
const double dPi = 3.1415926535897932384626433832795;
const float f2Pi = fPi*2.0f;
const double d2Pi = dPi*2.0;
const float fPi_2 = fPi*0.5f;
const double dPi_2 = dPi*0.5;

// ******************************************************************** //
// Min und Max - Funktionen, damit nicht irgendwelche Dinge importiert werden.
__forceinline float OrMaxf(const float a, const float b)	{return a > b ? a : b;}
__forceinline double OrMaxd(const double a, const double b)	{return a > b ? a : b;}
__forceinline int OrMax(const int a, const int b)			{return a > b ? a : b;}
__forceinline dword OrMaxu(const dword a, const dword b)	{return a > b ? a : b;}
__forceinline float OrMinf(const float a, const float b)	{return a < b ? a : b;}
__forceinline double OrMind(const double a, const double b)	{return a < b ? a : b;}
__forceinline int OrMin(const int a, const int b)			{return a < b ? a : b;}
__forceinline dword OrMinu(const dword a, const dword b)	{return a < b ? a : b;}
__forceinline float OrClampf(const float a, const float min, const float max)	{return a < min ? min : (a > max ? max : a);}
__forceinline dword OrClampu(const dword a, const dword min, const dword max)	{return a < min ? min : (a > max ? max : a);}
__forceinline float OrAbsf(const float a)					{return a < 0 ? -a: a;}
__forceinline int OrAbs(const int a)						{return a&0x7fffffff;}

// ******************************************************************** //
// Berechnet den Arcuscosinus: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
float OrArccos( float r );

// ******************************************************************** //
// Berechnet den Arcussinus: arctan( r / -sqr( 1.0f - r * r ) )
float OrArcsin( float r );

// ******************************************************************** //
// Berechnet den Arcustangens: arctan( r )
float OrArctan( float r );

// ******************************************************************** //
// Berechnet den Sinus (Bogenmaß)
float OrSin( float r );

// ******************************************************************** //
// Berechnet den Kosinus (Bogenmaß)
float OrCos( float r );

// ******************************************************************** //
// Berechnet den Tangens (Bogenmaß)
float OrTan( float r );

// ******************************************************************** //
// quadriert eine Zahl
float OrSqr( float r );

// ******************************************************************** //
// Zieht die Wurzel eines Floatwertes
float OrSqrt( float r );

// ******************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück (sehr schnell)
inline __declspec(naked) float __fastcall OrInvSqrt(float fValue)
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

// ******************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück
// etwas sicherer, aber auch ungenauer und langsamer
float OrInvSqrtEx( float r );

// ******************************************************************** //
// natürlicher Logarithmus
float OrLn( float r );

// ******************************************************************** //
// Funktion für den gerundeten 2er Logarithmus (ganzzahl)
int OrLog2(int iValue);

// ******************************************************************** //
// Exponentialfunktion
float __fastcall OrPow(float fBase, float fExponent);

// ******************************************************************** //
