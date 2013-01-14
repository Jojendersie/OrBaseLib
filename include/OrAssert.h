// ******************************************************************************** //
// OrAssert.h																		//
// ==========																		//
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
// My own assertion, which doesn't jump into an other file and did not ask for		//
// debugging.																		//
// ******************************************************************************** //

#pragma once
#ifdef _DEBUG
#include <intrin.h>

// Simple assertion like cassert, but stops in the line of source, where the
// assertion is located in not in some other file like the std::assert.
#define Assert(a) if(!(a)) __debugbreak()

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) if((A)!=(Const)) __debugbreak()
#define AssertNeq(A,Const) if((A)==(Const)) __debugbreak()

#else	// _DEBUG


#define Assert(a)

// The Assert__ execute A (even in release) and test for non-/equality with Const (debug only)
#define AssertEq(A,Const) A
#define AssertNeq(A,Const) A

#endif // _DEBUG

// *************************************EOF**************************************** //