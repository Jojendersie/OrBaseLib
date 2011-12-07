// ******************************************************************************** //
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrRandom.h"
#include "..\include\OrFastMath.h"

using namespace OrE::Math;
using namespace OrE::Algorithm;

// ******************************************************************************** //
// Interne Zufallsgeneratorzahlen - Mersenne Twister
const int       OR_MT_W		=	34;
const int       OR_MT_N		=	624;
const int       OR_MT_M		=	397;
const int       OR_MT_R		=	31;
const unsigned	OR_MT_A[2]	=	{ 0, 0x9908B0DF };	
const int       OR_MT_U		=	11;
const int       OR_MT_S		=	7;
const unsigned	OR_MT_B		=	0x9D2C5680;
const int       OR_MT_T		=	15;
const unsigned	OR_MT_C		=	 0xEFC60000;
const int       OR_MT_L		=	18;
const unsigned	OR_MT_LLMASK=	0x7FFFFFFF;
const unsigned	OR_MT_UMASK	=	0x80000000;

dword g_adwMT[OR_MT_N];
dword g_dwMTIndex = 0;

// ******************************************************************************** //
// Initialisate Mersenne Twister
void OrE::Algorithm::SRand(dword _dwSeed)
{
	// fill table
	for(int i=0; i<OR_MT_N; i++)
	{
		g_adwMT[i] = i%2?_dwSeed+i:_dwSeed*i;
	}

	g_dwMTIndex = OR_MT_N;
}

// ******************************************************************************** //
// der eigentliche Generator: erzeugt die Tabelle neu
void OrMTReCreate()
{
	dword y;

	g_dwMTIndex = 0;

	for (int k=0 ; k<OR_MT_N-OR_MT_M ; ++k)
	{
		y = (g_adwMT[k] & OR_MT_UMASK) | (g_adwMT[k+1] & OR_MT_LLMASK);
		g_adwMT[k] = g_adwMT[k+OR_MT_M] ^ (y >> 1) ^ OR_MT_A[y & 1];
	}
	for (int k=OR_MT_N-OR_MT_M ; k<OR_MT_N-1 ; ++k) {
		y = (g_adwMT[k] & OR_MT_UMASK) | (g_adwMT[k+1] & OR_MT_LLMASK);
		g_adwMT[k] = g_adwMT[k+(OR_MT_M-OR_MT_N)] ^ (y >> 1) ^ OR_MT_A[y & 1];
	}
	y = (g_adwMT[OR_MT_N-1] & OR_MT_UMASK) | (g_adwMT[0] & OR_MT_LLMASK);
	g_adwMT[OR_MT_N-1] = g_adwMT[OR_MT_M-1] ^ (y >> 1) ^ OR_MT_A[y & 1];
}

// ******************************************************************************** //
// Creates a integral random number between _iMin and _iMax (inclusive)
int	OrE::Algorithm::Rand(int _iMin, int _iMax)
{
	dword y;

	if(g_dwMTIndex == OR_MT_N)
		OrMTReCreate();


	y = g_adwMT[g_dwMTIndex++];
	y ^=y >> OR_MT_U;
	y ^=y << OR_MT_S & OR_MT_B;
	y ^=y << OR_MT_T & OR_MT_C;
	y ^=y >> OR_MT_L;

	// Bereichseinschränkung
	return (y % (_iMax + 1 - _iMin)) + _iMin;
}

// ******************************************************************************** //
// Creates a random number between 0 and 1 (inclusive)
float OrE::Algorithm::Rand()
{
	dword y;

	if(g_dwMTIndex == OR_MT_N)
		OrMTReCreate();

	y = g_adwMT[g_dwMTIndex++];
	y ^=y >> OR_MT_U;
	y ^=y << OR_MT_S & OR_MT_B;
	y ^=y << OR_MT_T & OR_MT_C;
	y ^=y >> OR_MT_L;

	// Bereichseinschränkung
	return (float)(y*(2.328306437e-10));
}

// ******************************************************************************** //
// Creates a random number of an exponential distribution [0,+Inf]
float OrE::Algorithm::ExpRand(float _fLambda)
{
	return -Ln(Rand()+0.000000000000000000000000000000000000000000001f)/_fLambda;
}	

// ******************************************************************************** //
// Creates a random number of a normal distribution [-Inf,+Inf]
float OrE::Algorithm::StdNormRand(float _fMean, float _fVariance)
{
	// Slow but stable, there are faster ways
	return _fMean + _fVariance*Sqrt(Maxf(0.0f,-2.0f*Ln(Rand()+0.000000000000000000000000000000000000000000001f))) * Cos(f2Pi*Rand());
}

// ******************************************************************************** //
// Creates a random number of a normal distribution [-Inf,+Inf]
float OrE::Algorithm::NormRand()
{
	// Slow but stable, there are faster ways
	return Sqrt(Maxf(0.0f,-2.0f*Ln(Rand()+0.000000000000000000000000000000000000000000001f))) * Cos(f2Pi*Rand());
}
