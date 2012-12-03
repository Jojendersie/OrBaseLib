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
//	Declarations of all "primitive" types											//
// ******************************************************************************** //

#pragma once

#include <limits.h>

//typedef unsigned int64_t qword;
//typedef unsigned int32_t dword;
//typedef unsigned int16_t word;
//typedef unsigned int8_t byte;

#if(UCHAR_MAX == 0xff)
	typedef unsigned char uint8;
	typedef unsigned char byte;
#else
	#error "sizeof(char) is not 1"
#endif

#if(SCHAR_MAX == 0x7f)
	typedef signed char int8;
#else
	#error "sizeof(char) is not 1"
#endif

#if(USHRT_MAX == 0xffff)
	typedef unsigned short uint16;
	typedef unsigned short word;
#else
	#if(UINT_MAX == 0xffff)
		typedef unsigned int uint16;
		typedef unsigned int word;
	#else
		#error "cannot support 16 bit unsigned integer on this plattform"
	#endif
#endif

#if(SHRT_MAX == 0x7fff)
	typedef signed short int16;
#else
	#if(INT_MAX == 0x7fff)
		typedef signed int int16;
	#else
		#error "cannot support 16 bit signed integer on this plattform"
	#endif
#endif

#if(USHRT_MAX == 0xffffffff)
	typedef unsigned short uint32;
	typedef unsigned short dword;
#else
	#if(UINT_MAX == 0xffffffff)
		typedef unsigned int uint32;
		typedef unsigned int dword;
	#else
		#if(ULONG_MAX == 0xffffffff)
			typedef unsigned long uint32;
			typedef unsigned long dword;
		#else
			#error "cannot support 32 bit unsigned integer on this plattform"
		#endif
	#endif
#endif

#if(SHRT_MAX == 0x7fffffff)
	typedef signed short int32;
#else
	#if(INT_MAX == 0x7fffffff)
		typedef signed int int32;
		#else
			#if(LONG_MAX == 0x7fffffff)
			typedef signed long int32;
		#else
			#error "cannot support 32 bit signed integer on this plattform"
		#endif
	#endif
#endif

#if(ULLONG_MAX == 0xffffffffffffffff)
	typedef unsigned long long uint64;
	typedef unsigned long long qword;
#else
	#if(UINT_MAX == 0xffffffffffffffff)
		typedef unsigned int uint64;
		typedef unsigned int qword;
	#else
		#if(ULONG_MAX == 0xffffffffffffffff)
			typedef unsigned long uint64;
			typedef unsigned long qword;
		#else
			#error "cannot support 64 bit unsigned integer on this plattform"
		#endif
	#endif
#endif

#if(LLONG_MAX == 0x7fffffffffffffff)
	typedef signed long long int64;
#else
	#if(INT_MAX == 0x7fffffffffffffff)
		typedef signed int int64;
		#else
			#if(LONG_MAX == 0x7fffffffffffffff)
			typedef signed long int64;
		#else
			#error "cannot support 64 bit signed integer on this plattform"
		#endif
	#endif
#endif

// ******************************************************************************** //
// An integral type which has always the size of (void*)
template <int BYTES> struct PtrToInt;
template <> struct PtrToInt<2> {typedef uint16 int_t;};
template <> struct PtrToInt<4> {typedef uint32 int_t;};
template <> struct PtrToInt<8> {typedef uint64 int_t;};
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