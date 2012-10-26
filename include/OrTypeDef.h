// ******************************************************************************** //
// OrTypeDef.h																		//
// ===========																		//
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
//	Declarations and predeclarations of all "primitive" types						//
// ******************************************************************************** //

#pragma once

typedef unsigned __int64 qword;
typedef unsigned __int32 dword;
typedef unsigned __int16 word;
typedef unsigned __int8 byte;

// ******************************************************************************** //
// An integral type which has always the size of (void*)
template <int BYTES> struct PtrToInt;
template <> struct PtrToInt<2> {typedef word int_t;};
template <> struct PtrToInt<4> {typedef dword int_t;};
template <> struct PtrToInt<8> {typedef qword int_t;};
typedef PtrToInt<sizeof(void*)>::int_t uiptr;


// ******************************************************************************** //
// Classes

namespace OrE {
namespace Math {
class Vec2;
class Vec3;
class Vec4;
class Matrix;
class Matrix2x3;
}; // namespace Math
}; // namespace OrE

// *************************************EOF**************************************** //