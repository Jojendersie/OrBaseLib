// ******************************************************************************** //
// OrRansdom.h																		//
// ===========																		//
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
int		Rand(int _iMin, int _iMax);						// Creates a integral random number between _iMin and _iMax (inclusive)
float	ExpRand(float _fLambda);						// Creates a random number of an exponential distribution [0,+Inf]
float	StdNormRand(float _fMean, float _fVariance);	// Creates a random number of a normal distribution [-Inf,+Inf]
float	NormRand();										// Creates a random number of a normal distribution [-Inf,+Inf]

}; // namespace OrE
}; // namespace Algorithm
// *************************************EOF**************************************** //