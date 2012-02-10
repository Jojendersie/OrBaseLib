// ******************************************************************************** //
// OrRandom.h																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// is project. You can do this by a comment at	https://github.com/Jojendersie/.	//
// Futhermore you have to state this project as a source of your project.			//
//																					//
// For details on this project see: Readme.txt										//
//																					//
// ******************************************************************************** //
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Algorithm {

// ******************************************************************************** //
void	SRand(dword _dwSeed);							// Initialisate Mersenne Twister
float	Rand();											// Creates a random number between 0 and 1 (inclusive)
int		Rand(int _iMin, int _iMax);						// Creates an integral random number between _iMin and _iMax (inclusive)
dword	RandU();										// Creates an unsinged random Number
float	ExpRand(float _fLambda);						// Creates a random number of an exponential distribution [0,+Inf]
float	StdNormRand(float _fMean, float _fVariance);	// Creates a random number of a normal distribution [-Inf,+Inf]
float	NormRand();										// Creates a random number of a normal distribution [-Inf,+Inf]

// ******************************************************************************** //
// A perlin noise object for n-D-Noise Variables. (~4KB per Object)
class PerlinNoise
{
private:
	dword m_dwSeed;
	double Sample1D(__int64 _i);
public:
	PerlinNoise(dword _dwSeed);

	// One sample at the specified position R^d -> [-1,1]
	// Parameter:
	//	_iLowOctave - Octave with the lowest frequence (inclusive)
	//	_iHeightOctave - Octave with the heighest frequence (inclusive). All frequences between are sampled too.
	//	_fPersistence - Relative height of a frequenze relative to the last one. Recommended: 0.5f.
	//	_fX - 1 float coordinate.
	float Rand1D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX);
	float Rand2D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY);
	float Rand3D(int _iLowOctave, int _iHeightOctave, float _fPersistence, float _fX, float _fY, float _fZ);
};

}; // namespace OrE
}; // namespace Algorithm
// *************************************EOF**************************************** //