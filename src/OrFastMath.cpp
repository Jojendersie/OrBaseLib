// ******************************************************************************** //
// OrFastMath.cpp																	//
// ==============																	//
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

#include "..\include\OrTypeDef.h"
#include "..\include\OrFastMath.h"

const float asm_one = 1.0f;

//using namespace OrE::Math;

// ******************************************************************************** //
// Berechnet den Arcuscosinus: pi/2 + arctan( r / -sqr( 1.0f - r * r ) )
/*float OrE::Math::Arccos( float r )
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

// ******************************************************************************** //
// Berechnet den Arcussinus: arctan( r / -sqr( 1.0f - r * r ) )
float OrE::Math::Arcsin( float r )
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

// ******************************************************************************** //
// Berechnet den Arcustangens: arctan( r )
float OrE::Math::Arctan( float r )
{
	__asm {
		fld r // r0 = r
		fld1 // {{ r0 = atan( r0 )
		fpatan // }}
	} // returns r0
}

// ******************************************************************************** //
// Berechnet den Sinus (Bogenmaß)
float OrE::Math::Sin( float r )
{
	__asm {
		fld r // r0 = r
		fsin // r0 = sinf( r0 )
	} // returns r0
}

// ******************************************************************************** //
// Berechnet den Kosinus (Bogenmaß)
float OrE::Math::Cos( float r )
{
	__asm {
		fld r // r0 = r
		fcos // r0 = cosf( r0 )
	} // returns r0
}

// ******************************************************************************** //
// Berechnet den Tangens (Bogenmaß)
float OrE::Math::Tan( float r )
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

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes
float OrE::Math::Sqrt( float r )
{
	__asm {
		fld r // r0 = r
		fsqrt // r0 = sqrtf( r0 )
	} // returns r0
}*/

// ******************************************************************************** //
// Zieht die Wurzel eines Floatwertes und gibt den Kehrwert zurück
/*float OrE::Math::InvSqrt(float fValue)
{
	float fValueHalf = fValue*0.5f;
	int i = *(int*)&fValue;
	i = 0x5f3759df - (i>>1);
	fValue = *(float*)&i;
	// Error reducing
	//fValue -= 0.00025f;
	// Newtonstep
	//fValue *= (1.5f-fValueHalf*fValue*fValue);
	fValue *= (1.5f-fValueHalf*fValue*fValue);
	return fValue * (1.5f-fValueHalf*fValue*fValue);//+0.000013f;
}

// Schnellere Variante ohne Fehlerkorrektur
float OrE::Math::_InvSqrt(float fValue)
{
	float fValueHalf = fValue*0.5f;
	int i = *(int*)&fValue;
	i = 0x5f3759df - (i>>1);
	return (*(float*)&i) - 0.000056161f;
}*/


/*float OrE::Math::InvSqrtEx( float r )
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
}*/

// ******************************************************************************** //
// Logarithmus dualis
int OrE::Math::_Ld( float r )
{
	// Rounded log2 from exponent
	int i = *(int*)&r;
	return (i>>23)-127;
}
float OrE::Math::Ld( float r )
{
	// Rounded log2 from exponent
	int i = *(int*)&r;
	i = (i>>23)-127;
	// Find fractinal from linear interpolation
	// (1<<i) is the power of 2 <= r
	// the next heigher power would be (1<<(i+1)) == (1<<i)*2
	// what we want to know, where between is r: (1<<i) <= r < (1<<i)+(1<<i)
	int p = (1<<i);
	return i + (r-p)/(float)(p);
}
int OrE::Math::Ld( int r )
{
	// Rounded log2 from exponent
	float f = (float)r;
	r = *(int*)&f;
	return (r>>23)-127;
}

// natürlicher Logarithmus
float OrE::Math::Ln( float r )
{
	// Doing the same as for Ld
	int i = *(int*)&r;
	i = (i>>23)-127;
	int p = (1<<i);
	return (i + (r-p)/(float)(p))*0.693147180559f;	// Transform to basis e

	/*const float asm_1_div_log2_e = 0.693147180559f;
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
	}*/
}

// ******************************************************************************** //
// Funktion für den gerundeten 2er Logarithmus (ganzzahl)
/*int OrE::Math::Log2(int iValue)
{
	__asm bsf eax, iValue
//	__asm mov dword ptr[iValue], eax
//	return iValue;
}*/

// ******************************************************************************** //
// Exponentialfunktion
/*float __fastcall OrE::Math::Pow(float fBase, float fExponent)
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
}*/

// ******************************************************************************** //
// Gray Code convertions
unsigned int OrE::Math::GrayCodeToNum(unsigned int _uiGrayCode)
{
        _uiGrayCode ^= (_uiGrayCode >> 16);
        _uiGrayCode ^= (_uiGrayCode >> 8);
        _uiGrayCode ^= (_uiGrayCode >> 4);
        _uiGrayCode ^= (_uiGrayCode >> 2);
        _uiGrayCode ^= (_uiGrayCode >> 1);
        return _uiGrayCode;
}

unsigned int OrE::Math::NumToGrayCode(unsigned int _uiNum)
{
        return (_uiNum>>1)^_uiNum;
}


// ******************************************************************************** //
// Parallel bit counting to estimate the number of 1 bits in a number
const qword QMASK_01010101  = 0x5555555555555555;
const qword QMASK_00110011  = 0x3333333333333333;
const qword QMASK_00001111  = 0x0f0f0f0f0f0f0f0f;
const qword QMASK_0x8_1x8   = 0x00ff00ff00ff00ff;
const qword QMASK_0x16_1x16 = 0x0000ffff0000ffff;
const qword QMASK_0x32_1x32 = 0x00000000ffffffff;
qword OrE::Math::CountBits64(qword n)
{
	n =    (n & QMASK_01010101)  + ((n >> 1) & QMASK_01010101);
	n =    (n & QMASK_00110011)  + ((n >> 2) & QMASK_00110011);
	n =    (n & QMASK_00001111)  + ((n >> 4) & QMASK_00001111);
	n =    (n & QMASK_0x8_1x8)   + ((n >> 8) & QMASK_0x8_1x8);
	n =    (n & QMASK_0x16_1x16) + ((n >> 16) & QMASK_0x16_1x16);
	return (n & QMASK_0x32_1x32) + ((n >> 32) & QMASK_0x32_1x32);
}

const dword MASK_01010101  = 0x55555555;
const dword MASK_00110011  = 0x33333333;
const dword MASK_00001111  = 0x0f0f0f0f;
const dword MASK_0x8_1x8   = 0x00ff00ff;
const dword MASK_0x16_1x16 = 0x0000ffff;
dword OrE::Math::CountBits32(dword n)
{
	n =    (n & MASK_01010101)  + ((n >> 1) & MASK_01010101);
	n =    (n & MASK_00110011)  + ((n >> 2) & MASK_00110011);
	n =    (n & MASK_00001111)  + ((n >> 4) & MASK_00001111);
	n =    (n & MASK_0x8_1x8)   + ((n >> 8) & MASK_0x8_1x8);
	return (n & MASK_0x16_1x16) + ((n >> 16) & MASK_0x16_1x16);
}

const word WMASK_01010101  = 0x5555;
const word WMASK_00110011  = 0x3333;
const word WMASK_00001111  = 0x0f0f;
const word WMASK_0x8_1x8   = 0x00ff;
word OrE::Math::CountBits16(word n)
{
	n =    (n & WMASK_01010101) + ((n >> 1) & WMASK_01010101);
	n =	   (n & WMASK_00110011) + ((n >> 2) & WMASK_00110011);
	n =    (n & WMASK_00001111) + ((n >> 4) & WMASK_00001111);
	return (n & WMASK_0x8_1x8)  + ((n >> 8) & WMASK_0x8_1x8);
}

// *************************************EOF**************************************** //