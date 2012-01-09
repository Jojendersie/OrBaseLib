// ******************************************************************************** //
// OrRandom.cpp																		//
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
// Creates an unsinged random Number
dword OrE::Algorithm::RandU()
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
	return y;
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


// ******************************************************************************** //
OrE::Algorithm::PerlinNoise::PerlinNoise(dword _dwSeed)
{
	// Use mersenne twister to seed a table for later random samplings
	SRand(_dwSeed);
	for(int i=0; i<1024; ++i)
		m_dwWhiteNoiseTable[i] = RandU();
}

// ******************************************************************************** //
dword OrE::Algorithm::PerlinNoise::Sample1D(int _i)
{
	return m_dwWhiteNoiseTable[_i&0x3ff] + m_dwWhiteNoiseTable[(_i>>5)&0x3ff]
		 ^ m_dwWhiteNoiseTable[(_i*7+11)&0x3ff];
}

// ******************************************************************** //
// Funktion zur Interpolation (zwischen zwei Punkten des Rauschens)
// Varianten: Cosinus und C2-stetige Polynomfunktion
// Die Varianten unterscheiden sich kaum bis gar nicht
//
// Zweck:
// Die Funktion erhält als Eingabe drei float Werte _dA und _dB sowie _dR.
// Für _dR aus dem Intervall [0,1] gibt Interpolate eine Cosinus-Interpolation
// mit _dA und _dB als Maximmum- bzw. Minimumwert.
// Für _dR = 0 ergibt Interpolate = _dA. Für _dR = 1 ergibt Interpolate = _dB.
double Interpolate(double  _dA, double  _dB, double _dR)
{
	// Polynomfunktion für Details sehe "Burger-GradientNoiseGerman-2008".
	double _dF = _dR*_dR*_dR*(_dR*(_dR*6.0f-15.0f)+10.0f);
	return  (_dA * (1-_dF) + _dB * _dF);
}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand1D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX)
{
	double dRes = 0.0;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequenz = (float)(1<<i);
		// We need 2 samples per dimension -> 2 samples total
		float fFracX = _fX * fFrequenz;
		int iX = (int)fFracX;
		fFracX -= iX;
		double s0 = fAmplitude*(Sample1D(iX)/2147483648.0);
		double s1 = fAmplitude*(Sample1D(iX+1)/2147483648.0);

		dRes += Interpolate(s0, s1, fFracX);
	}

	// Transform to [0,1]
	return float(dRes)*OrE::Math::Ln(_fPersistence)/(OrE::Math::Pow(_fPersistence, (float)_iHeightOctave)-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand2D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY)
{
	double dRes = 0.0;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequenz = (float)(1<<i);
		// We need 2 samples per dimension -> 2 samples total
		float fFracX = _fX * fFrequenz;
		float fFracY = _fY * fFrequenz;
		int iX = (int)fFracX;
		int iY = (int)fFracY;
		fFracX -= iX;
		fFracY -= iY;
		double s00 = fAmplitude*((Sample1D(iX)^Sample1D(iY))/2147483648.0);
		double s10 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY))/2147483648.0);
		double s01 = fAmplitude*((Sample1D(iX)^Sample1D(iY+1))/2147483648.0);
		double s11 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY+1))/2147483648.0);

		dRes += Interpolate(Interpolate(s00, s10, fFracX), Interpolate(s01, s11, fFracX), fFracY);
	}

	// Transform to [0,1]
	return float(dRes)*OrE::Math::Ln(_fPersistence)/(OrE::Math::Pow(_fPersistence, (float)_iHeightOctave)-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand3D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY, float _fZ)
{
	double dRes = 0.0;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequenz = (float)(1<<i);
		// We need 2 samples per dimension -> 2 samples total
		float fFracX = _fX * fFrequenz;
		float fFracY = _fY * fFrequenz;
		float fFracZ = _fZ * fFrequenz;
		int iX = (int)fFracX;
		int iY = (int)fFracY;
		int iZ = (int)fFracZ;
		fFracX -= iX;
		fFracY -= iY;
		fFracZ -= iZ;
		double s000 = fAmplitude*((Sample1D(iX)^Sample1D(iY)^Sample1D(iZ))/2147483648.0);
		double s100 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY)^Sample1D(iZ))/2147483648.0);
		double s010 = fAmplitude*((Sample1D(iX)^Sample1D(iY+1)^Sample1D(iZ))/2147483648.0);
		double s110 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY+1)^Sample1D(iZ))/2147483648.0);
		double s001 = fAmplitude*((Sample1D(iX)^Sample1D(iY)^Sample1D(iZ+1))/2147483648.0);
		double s101 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY)^Sample1D(iZ+1))/2147483648.0);
		double s011 = fAmplitude*((Sample1D(iX)^Sample1D(iY+1)^Sample1D(iZ+1))/2147483648.0);
		double s111 = fAmplitude*((Sample1D(iX+1)^Sample1D(iY+1)^Sample1D(iZ+1))/2147483648.0);

		dRes += Interpolate(Interpolate(Interpolate(s000, s100, fFracX), Interpolate(s010, s110, fFracX), fFracY),
							Interpolate(Interpolate(s001, s101, fFracX), Interpolate(s011, s111, fFracX), fFracY), fFracZ);
	}

	// Transform to [0,1]
	return float(dRes)*OrE::Math::Ln(_fPersistence)/(OrE::Math::Pow(_fPersistence, (float)_iHeightOctave)-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// *************************************EOF**************************************** //