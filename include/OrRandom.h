// ******************************************************************************** //
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#pragma once

// ******************************************************************************** //
void	OrSRand(dword _dwSeed);							// Initialisate Mersenne Twister
float	OrRand();										// Creates a random number between 0 and 1 (inclusive)
int		OrRand(int _iMin, int _iMax);					// Creates a integral random number between _iMin and _iMax (inclusive)
float	OrExpRand(float _fLambda);						// Creates a random number of an exponential distribution [0,+Inf]
float	OrStdNormRand(float _fMean, float _fVariance);	// Creates a random number of a normal distribution [-Inf,+Inf]
float	OrNormRand();									// Creates a random number of a normal distribution [-Inf,+Inf]

// *************************************EOF**************************************** //