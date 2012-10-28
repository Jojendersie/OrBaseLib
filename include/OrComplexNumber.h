// ******************************************************************************** //
// OrComplexNumber.h																//
// =================																//
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
	struct ComplexNumber
	{
		float		r;
		float		i;

		// Constructors
		ComplexNumber(): r(0.0f), i(0.0f)							{}
		ComplexNumber(float _r): r(_r), i(0.0f)						{}
		ComplexNumber(float _r, float _i): r(_r), i(_i)				{}
		ComplexNumber(const ComplexNumber& _n): r(_n.r), i(_n.i)	{}

		// Assignment operators
		ComplexNumber& operator = (const ComplexNumber& n)	{r = n.r; i = n.i; return *this;}
		ComplexNumber& operator = (const float f)			{r = f; i = 0.0f; return *this;}
		ComplexNumber& operator += (const ComplexNumber& n)	{r += n.r; i += n.i; return *this;}
		ComplexNumber& operator -= (const ComplexNumber& n)	{r -= n.r; i -= n.i; return *this;}
		ComplexNumber& operator *= (const ComplexNumber& n)	{r = n.r*r - n.i*i; i = r*n.i + i*n.r; return *this;}
		ComplexNumber& operator *= (const float f)			{r *= f; i *= f; return *this;}
		ComplexNumber& operator /= (const ComplexNumber& n)	{float q = 1.0f/(n.r*n.r + n.i*n.i); r = (r*n.r + i*n.i)*q; i = (i*n.r - r*n.i)*q; return *this;}
		ComplexNumber& operator /= (float f)				{f = 1/f; r *= f; i *= f; return *this;}

		// ******************************************************************************** //
		// Property functions
		inline float Length() const		{return Sqrt(r * r + i * i);}
		inline float Arg() const		{return Arctan(i/r);}
		inline float LengthSq() const	{return (r * r + i * i);}
	};

	typedef ComplexNumber* ComplexNumberP;

	// ******************************************************************************** //
	// Arithmetic operators
	inline ComplexNumber operator + (const ComplexNumber& a, const ComplexNumber& b)	{return ComplexNumber(a.r + b.r, a.i + b.i);}
	inline ComplexNumber operator - (const ComplexNumber& a, const ComplexNumber& b)	{return ComplexNumber(a.r - b.r, a.i - b.i);}
	inline ComplexNumber operator - (const ComplexNumber& n)							{return ComplexNumber(-n.r, -n.i);}
	// Conjugation
	inline ComplexNumber operator ~ (const ComplexNumber& n)							{return ComplexNumber(n.r, -n.i);}
	inline ComplexNumber operator * (const ComplexNumber& a, const ComplexNumber& b)	{return ComplexNumber(a.r*b.r - a.i*b.i, a.r*b.i + a.i*b.r);}
	inline ComplexNumber operator * (const ComplexNumber& n, const float f)				{return ComplexNumber(n.r * f, n.i * f);}
	inline ComplexNumber operator * (const float f, const ComplexNumber& n)				{return ComplexNumber(n.r * f, n.i * f);}
	inline ComplexNumber operator / (const ComplexNumber& a, const ComplexNumber& b)	{float q = 1.0f/(b.r*b.r + b.i*b.i); return ComplexNumber((a.r*b.r + a.i*b.i)*q, (a.i*b.r - a.r*b.i)*q);}
	inline ComplexNumber operator / (const ComplexNumber& n, float f)					{f = 1/f; return ComplexNumber(n.r * f, n.i * f);}

	// ******************************************************************************** //
	// Comparison operators
	inline bool operator == (const ComplexNumber& a, const ComplexNumber& b) {if(a.r != b.r) return false; return a.i == b.i;}
	inline bool operator != (const ComplexNumber& a, const ComplexNumber& b) {if(a.r != b.r) return true; return a.i != b.i;}

	// ******************************************************************************** //
	// Functions

	// A complex number to the power of a complex number.
	// Internal "converts" to the Euler form and back.
	ComplexNumber Pow(const ComplexNumber& a, const ComplexNumber& b);

	// Faster power variant for real numbered exponents
	ComplexNumber Pow(const ComplexNumber& a, float b);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //