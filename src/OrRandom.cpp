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
#include "..\include\OrVector2.h"
#include "..\include\OrVector3.h"

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
	return _fMean + _fVariance*Sqrt(Max(0.0f,-2.0f*Ln(Rand()+0.000000000000000000000000000000000000000000001f))) * Cos(f2Pi*Rand());
}

// ******************************************************************************** //
// Creates a random number of a normal distribution [-Inf,+Inf]
float OrE::Algorithm::NormRand()
{
	// Slow but stable, there are faster ways
	return Sqrt(Max(0.0f,-2.0f*Ln(Rand()+0.000000000000000000000000000000000000000000001f))) * Cos(f2Pi*Rand());
}


// ******************************************************************************** //
OrE::Algorithm::PerlinNoise::PerlinNoise(dword _dwSeed)
{
	m_dwSeed = _dwSeed*1987;
	// Use mersenne twister to seed a table for later random samplings
//	SRand(_dwSeed);
//	for(int i=0; i<1024; ++i)
		//m_dwWhiteNoiseTable[i] = RandU();
}

// ******************************************************************************** //
double OrE::Algorithm::PerlinNoise::Sample1D(__int64 _i)
{
	_i += m_dwSeed;
	_i ^= (_i<<13);
	return (((_i * (_i * _i * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483647.0);
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
inline float InterpolationPolynom(float _dR)
{
	// Polynomfunktion für Details sehe "Burger-GradientNoiseGerman-2008".
	return _dR*_dR*_dR*(_dR*(_dR*6.0f-15.0f)+10.0f);
}

inline float Derivative(float _dR)
{
	return _dR*_dR*(_dR*(_dR-2.0f)+1.0f)*30.0f;
}

inline void IntFrac(float _f, int& _iInt, float& _fFrac)	{_iInt = Floor(_f); _fFrac = _f-_iInt;}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand1D(float _fX)
{
	// We need 2 samples per dimension -> 2 samples total
	float fFracX;
	int iX;
	IntFrac(_fX, iX, fFracX);

	float s0 = (float)Sample1D( iX   *57);
	float s1 = (float)Sample1D((iX+1)*57);

	float u = InterpolationPolynom(fFracX);

	return OrE::Math::Lerp(s0, s1, u);
}

float OrE::Algorithm::PerlinNoise::Rand1D(float _fX, float& _vOutGrad)
{
	// We need 2 samples per dimension -> 2 samples total
	float fFracX;
	int iX;
	IntFrac(_fX, iX, fFracX);

	float s0 = (float)Sample1D( iX   *57);
	float s1 = (float)Sample1D((iX+1)*57);

	float u = InterpolationPolynom(fFracX);
	float du = Derivative(fFracX);

	// Calculate the gradient per component
	_vOutGrad = (s1-s0)*du;

	return OrE::Math::Lerp(s0, s1, u);
}

float OrE::Algorithm::PerlinNoise::Rand1D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX)
{
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		fRes += fAmplitude*Rand1D(_fX * fFrequence);
	}

	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

float OrE::Algorithm::PerlinNoise::Rand1D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float& _vOutGrad)
{
	_vOutGrad = 0.0f;
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		float vNormal;
		fRes += fAmplitude*Rand1D(_fX * fFrequence, vNormal);
		_vOutGrad += vNormal;

//		double d = dAmplitude*Rand1D(_fX * fFrequence, &dx);	Interesting alternative
//		dRes += d/(dx*dx+1.0f);
	}

	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand2D(float _fX, float _fY)
{
	// We need 2 samples per dimension -> 4 samples total
	float fFracX, fFracY;
	int iX, iY;
	IntFrac(_fX, iX, fFracX);
	IntFrac(_fY, iY, fFracY);

	float s00 = (float)Sample1D( iX   *57 +  iY   *101);
	float s10 = (float)Sample1D((iX+1)*57 +  iY   *101);
	float s01 = (float)Sample1D( iX   *57 + (iY+1)*101);
	float s11 = (float)Sample1D((iX+1)*57 + (iY+1)*101);

	float u = InterpolationPolynom(fFracX);
	float v = InterpolationPolynom(fFracY);

	return OrE::Math::Lerp(OrE::Math::Lerp(s00, s10, u), OrE::Math::Lerp(s01, s11, u), v);
}

float OrE::Algorithm::PerlinNoise::Rand2D(float _fX, float _fY, OrE::Math::Vector2& _vOutGrad)
{
	// We need 2 samples per dimension -> 4 samples total
	float fFracX, fFracY;
	int iX, iY;
	IntFrac(_fX, iX, fFracX);
	IntFrac(_fY, iY, fFracY);

	float s00 = (float)Sample1D( iX   *57 +  iY   *101);
	float s10 = (float)Sample1D((iX+1)*57 +  iY   *101);
	float s01 = (float)Sample1D( iX   *57 + (iY+1)*101);
	float s11 = (float)Sample1D((iX+1)*57 + (iY+1)*101);

	float u = InterpolationPolynom(fFracX);
	float v = InterpolationPolynom(fFracY);
	float du = Derivative(fFracX);
	float dv = Derivative(fFracY);

	float k1 = s10 - s00;
	float k2 = s01 - s00;

	_vOutGrad.x = (k1+(s11-s01-k1)*v)*du;
	_vOutGrad.y = (k2+(s11-s10-k1)*u)*dv;

	//	   OrE::Math::Lerp(OrE::Math::Lerp(s00, s10, u), OrE::Math::Lerp(s01, s11, u), v);
	return OrE::Math::Lerp(				   s00 + k1 * u, OrE::Math::Lerp(s01, s11, u), v);
}

float OrE::Algorithm::PerlinNoise::Rand2D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY)
{
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		fRes += fAmplitude*Rand2D(_fX*fFrequence, _fY*fFrequence);
	}

	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

float OrE::Algorithm::PerlinNoise::Rand2D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY, OrE::Math::Vector2& _vOutGrad)
{
	_vOutGrad.x = _vOutGrad.y = 0.0f;
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		OrE::Math::Vector2 vNormal;
		fRes += fAmplitude*Rand2D(_fX*fFrequence, _fY*fFrequence, vNormal);
		_vOutGrad += vNormal;
	}

	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// ******************************************************************************** //
float OrE::Algorithm::PerlinNoise::Rand3D(float _fX, float _fY, float _fZ)
{
	// We need 2 samples per dimension -> 8 samples total
	float fFracX, fFracY, fFracZ;
	int iX, iY, iZ;
	IntFrac(_fX, iX, fFracX);
	IntFrac(_fY, iY, fFracY);
	IntFrac(_fZ, iZ, fFracZ);

	float s000 = (float)Sample1D( iX   *57 +  iY   *101 +  iZ   *307);
	float s100 = (float)Sample1D((iX+1)*57 +  iY   *101 +  iZ   *307);
	float s010 = (float)Sample1D( iX   *57 + (iY+1)*101 +  iZ   *307);
	float s110 = (float)Sample1D((iX+1)*57 + (iY+1)*101 +  iZ   *307);
	float s001 = (float)Sample1D( iX   *57 +  iY   *101 + (iZ+1)*307);
	float s101 = (float)Sample1D((iX+1)*57 +  iY   *101 + (iZ+1)*307);
	float s011 = (float)Sample1D( iX   *57 + (iY+1)*101 + (iZ+1)*307);
	float s111 = (float)Sample1D((iX+1)*57 + (iY+1)*101 + (iZ+1)*307);

	float u = InterpolationPolynom(fFracX);
	float v = InterpolationPolynom(fFracY);
	float w = InterpolationPolynom(fFracZ);

	return OrE::Math::Lerp(OrE::Math::Lerp(OrE::Math::Lerp(s000, s100, u), OrE::Math::Lerp(s010, s110, u), v),
						   OrE::Math::Lerp(OrE::Math::Lerp(s001, s101, u), OrE::Math::Lerp(s011, s111, u), v), w);
}

float OrE::Algorithm::PerlinNoise::Rand3D(float _fX, float _fY, float _fZ, OrE::Math::Vector3& _vOutGrad)
{
	// We need 2 samples per dimension -> 8 samples total
	float fFracX, fFracY, fFracZ;
	int iX, iY, iZ;
	IntFrac(_fX, iX, fFracX);
	IntFrac(_fY, iY, fFracY);
	IntFrac(_fZ, iZ, fFracZ);

	float s000 = (float)Sample1D( iX   *57 +  iY   *101 +  iZ   *307);
	float s100 = (float)Sample1D((iX+1)*57 +  iY   *101 +  iZ   *307);
	float s010 = (float)Sample1D( iX   *57 + (iY+1)*101 +  iZ   *307);
	float s110 = (float)Sample1D((iX+1)*57 + (iY+1)*101 +  iZ   *307);
	float s001 = (float)Sample1D( iX   *57 +  iY   *101 + (iZ+1)*307);
	float s101 = (float)Sample1D((iX+1)*57 +  iY   *101 + (iZ+1)*307);
	float s011 = (float)Sample1D( iX   *57 + (iY+1)*101 + (iZ+1)*307);
	float s111 = (float)Sample1D((iX+1)*57 + (iY+1)*101 + (iZ+1)*307);

	float u = InterpolationPolynom(fFracX);
	float v = InterpolationPolynom(fFracY);
	float w = InterpolationPolynom(fFracZ);
	float du = Derivative(fFracX);
	float dv = Derivative(fFracY);
	float dw = Derivative(fFracZ);
	float uv = u*v;
	float uw = u*w;
	float vw = v*w;

	float k0 = s000;
    float k1 = s100 - s000;
    float k2 = s010 - s000;
    float k3 = s001 - s000;
    float k4 = s110 - s010 - k1;
    float k5 = s000 - s010 - s001 + s011;
    float k6 = - k1 - s001 + s101;
    float k7 = - k4 + s001 - s101 - s011 + s111;

	_vOutGrad.x = du * (k1 + k4*v + k6*w + k7*vw);
	_vOutGrad.y = dv * (k2 + k4*u + k5*w + k7*uw);
	_vOutGrad.z = dw * (k3 + k6*u + k5*v + k7*uv);

    return k0 + k1*u + k2*v + k3*w + k4*uv + k5*vw + k6*uw + k7*uv*w;
}

float OrE::Algorithm::PerlinNoise::Rand3D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY, float _fZ)
{
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		fRes += fAmplitude*Rand3D(_fX*fFrequence, _fY*fFrequence, _fZ*fFrequence);
	}
	
	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

float OrE::Algorithm::PerlinNoise::Rand3D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY, float _fZ, OrE::Math::Vector3& _vOutGrad)
{
	_vOutGrad.x = _vOutGrad.y = _vOutGrad.z = 0.0f;
	float fRes = 0.0f;
	for(int i=_iLowOctave; i<=_iHeightOctave; ++i)
	{
		float fAmplitude = OrE::Math::Pow(_fPersistence, (float)i);
		float fFrequence = (float)(1<<i);

		OrE::Math::Vector3 vNormal;
		fRes += fAmplitude*Rand3D(_fX*fFrequence, _fY*fFrequence, _fZ*fFrequence, vNormal);
		_vOutGrad += vNormal;
	}
	
	// Transform to [-1,1]
	return fRes*2.0f*(_fPersistence-1.0f)/(OrE::Math::Pow(_fPersistence, (float)(_iHeightOctave+1))-OrE::Math::Pow(_fPersistence, (float)_iLowOctave))-1.0f;
}

// *************************************EOF**************************************** //