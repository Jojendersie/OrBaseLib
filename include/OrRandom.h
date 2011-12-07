// ******************************************************************************** //
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Algorithm {

// ******************************************************************************** //
void	SRand(dword _dwSeed);							// Initialisate Mersenne Twister
float	Rand();										// Creates a random number between 0 and 1 (inclusive)
int		Rand(int _iMin, int _iMax);					// Creates a integral random number between _iMin and _iMax (inclusive)
float	ExpRand(float _fLambda);						// Creates a random number of an exponential distribution [0,+Inf]
float	StdNormRand(float _fMean, float _fVariance);	// Creates a random number of a normal distribution [-Inf,+Inf]
float	NormRand();									// Creates a random number of a normal distribution [-Inf,+Inf]

}; // namespace OrE
}; // namespace Algorithm
// *************************************EOF**************************************** //