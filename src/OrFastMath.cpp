/*************************************************************************

	OrFastMath.cpp
	==============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Schnelle Mathematische Funktionen

*************************************************************************/

#include "..\include\OrTypeDef.h"
#include "..\include\OrFastMath.h"

const float asm_one = 1.0f;

// ******************************************************************** //
// Berechnet den Arcuscosinus: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
float OrArccos( float r )
{
	//float asm_half_pi = half_pi;
	__asm {
		fld r // r0 = r
		fld r // r1 = r0, r0 = r
		fmul r // r0 = r0 * r
		fsubr asm_one // r0 = r0 - 1.f
		fsqrt // r0 = sqrtf( r0 )
		fchs // r0 = - r0
		fdiv // r0 = r1 / r0
		fld1 // {{ r0 = atan( r0 )
		fpatan // }}
		fadd fPi_2 // r0 = r0 + pi / 2
	} // returns r0
}

// ******************************************************************** //
// Berechnet den Arcussinus: arctan( r / -sqr( 1.0f - r * r ) )
float OrArcsin( float r )
{
	__asm {
		fld r // r0 = r
		fld r // r1 = r0, r0 = r
		fmul r // r0 = r0 * r
		fsubr asm_one // r0 = r0 - 1.f
		fsqrt // r0 = sqrtf( r0 )
		fdiv // r0 = r1 / r0
		fld1 // {{ r0 = atan( r0 )
		fpatan // }}
	} // returns r0
}

// ******************************************************************** //
// Berechnet den Arcustangens: arctan( r )
float OrArctan( float r )
{
	__asm {
		fld r // r0 = r
		fld1 // {{ r0 = atan( r0 )
		fpatan // }}
	} // returns r0
}

// ******************************************************************** //
// Berechnet den Sinus (Bogenmaß)
float OrSin( float r )
{
	__asm {
		fld r // r0 = r
		fsin // r0 = sinf( r0 )
	} // returns r0
}

// ******************************************************************** //
// Berechnet den Kosinus (Bogenmaß)
float OrCos( float r )
{
	__asm {
		fld r // r0 = r
		fcos // r0 = cosf( r0 )
	} // returns r0
}

// ******************************************************************** //
// Berechnet den Tangens (Bogenmaß)
float OrTan( float r )
{
	// return sin( r ) / cos( r );
	__asm {
		fld r // r0 = r
		fsin // r0 = sinf( r0 )
		fld r // r1 = r0, r0 = r
		fcos // r0 = cosf( r0 )
		fdiv // r0 = r1 / r0
	} // returns r0
}

// ******************************************************************** //
// quadriert eine Zahl
float OrSqr( float r )
{
	__asm {
		fld r // r0 = r
		fld r // r1=r0, r0=r
		fmul // r0 = r0 * r1
	} // returns r0
}

// ******************************************************************** //
// Zieht die Wurzel eines Floatwertes
float OrSqrt( float r )
{
	__asm {
		fld r // r0 = r
		fsqrt // r0 = sqrtf( r0 )
	} // returns r0
}

// ******************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück
// etwas sicherer, aber auch ungenauer und langsamer
float OrInvSqrtEx( float r )
{
	const float asm_dot5 = 0.5f;
	const float asm_1dot5 = 1.5f;

	__asm {
		fld r // r0 = r
		fmul asm_dot5 // r0 = r0 * .5f
		mov eax, r // eax = r
		shr eax, 0x1 // eax = eax >> 1
		neg eax // eax = -eax
		add eax, 0x5F400000 // eax = eax & MAGICAL NUMBER
		mov r, eax // r = eax
		fmul r // r0 = r0 * r
		fmul r // r0 = r0 * r
		fsubr asm_1dot5 // r0 = 1.5f - r0
		fmul r // r0 = r0 * r
	} // returns r0
}

// ******************************************************************** //
// natürlicher Logarithmus
float OrLn( float r )
{
	const float asm_1_div_log2_e = 0.693147180559f;
	const float asm_neg1_div_3 = -0.33333333333333333333333333333f;
	const float asm_neg2_div_3 = -0.66666666666666666666666666667f;
	const float asm_2 = 2.0f;

	int log_2 = 0;

	__asm {
		// log_2 = ( ( r >> 0x17 ) & 0xFF ) - 0x80;
		mov eax, r
		sar eax, 0x17
		and eax, 0xFF
		sub eax, 0x80
		mov log_2, eax

		// r = ( r & 0x807fffff ) + 0x3f800000;
		mov ebx, r
		and ebx, 0x807FFFFF
		add ebx, 0x3F800000
		mov r, ebx

		// r = ( asm_neg1_div_3 * r + asm_2 ) * r + asm_neg2_div_3;   // (1)
		fld r
		fmul asm_neg1_div_3
		fadd asm_2
		fmul r
		fadd asm_neg2_div_3
		fild log_2
		fadd
		fmul asm_1_div_log2_e
	}
}

// ******************************************************************** //
// Funktion für den gerundeten 2er Logarithmus (ganzzahl)
int OrLog2(int iValue)
{
	__asm bsf eax, iValue
//	__asm mov dword ptr[iValue], eax
//	return iValue;
}

// ******************************************************************** //
// Exponentialfunktion
float __fastcall OrPow(float fBase, float fExponent)
{
	const float asm_1 = 1.0f;
	__asm
	{
		fld fExponent		// r0 = exp
		fld fBase			// r0 = base; r1 = exp
		fyl2x				// r1 = r1*ld(r0); pop r0
		fld  st				// Kopie anlegen r0=r1=exp*ld(base)
		frndint				// runden zur Ganzzahl
		fsub st(1),st		// in r0 nur den Fraktalteil behalten
		fxch st(1)			// tauschen
		f2xm1				// Nachkommateil exponentieren
		fadd asm_1			// Die 1 wieder herausrechnen
		fscale				// addieren des Integerwerts und des Exponenten des Gleitkommteils
		fstp st(1)			// Register 1 "verwerfen"

		;fxtract				// r0 = Mantisse [1;2]; r1 = Exp
		;fadd asm_neg1		// r0 -= 1		=> [0;1]
		;f2xm1				// r0 = 2^r0-1
		;fsub asm_neg1		// r0 += 1;
		;fscale				// Mantisse und Exponent zusammenführen
	}						// returns r0
}


// ******************************************************************** //