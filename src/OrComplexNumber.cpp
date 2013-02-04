#include "../include/OrTypeDef.h"
#include "../include/OrFastMath.h"
#include "../include/OrComplexNumber.h"

// ******************************************************************************** //
// A complex number to the power of a complex number.
// Internal "converts" to the euler form and back.
OrE::Math::ComplexNumber OrE::Math::Pow(const ComplexNumber& a, const ComplexNumber& b)
{
	double dArgA = a.Arg();
	double dLenA = a.LengthSq();						// Using square and use an implicit exponent of 0.5 (faster)
	double dAngle = b.r*dArgA+0.5*b.i*log(dLenA);
	return float(pow(dLenA, b.r*0.5) * exp(-b.i*dArgA)) * 
		ComplexNumber(float(cos(dAngle)),
					  float(sin(dAngle)));
}

// ******************************************************************************** //
// Faster power variant for real numbered exponents
OrE::Math::ComplexNumber OrE::Math::Pow(const ComplexNumber& a, float b)
{
	double dArgA = a.Arg();
	double dLenA = a.LengthSq();						// Using square and use an implicit exponent of 0.5 (faster)
	double dAngle = b*dArgA;
	return float(pow(dLenA, b*0.5)) * 
		ComplexNumber(float(cos(dAngle)),
					  float(sin(dAngle)));
}