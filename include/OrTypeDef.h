// ******************************************************************************** //
// OrTypeDef.h																		//
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
// ******************************************************************************** //
//	Declaration and predeclarations of all "primitive" types						//
// ******************************************************************************** //

#pragma once

typedef unsigned __int64 qword;
typedef unsigned __int32 dword;
typedef unsigned __int16 word;
typedef unsigned __int8 byte;

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

// ******************************************************************************** //
// Atomare Funktion zum Sichern kritischer Bereiche
/*inline bool OrLock(volatile dword* _pLockVar)
{
	__asm mov eax, _pLockVar
try_enter:
	__asm
	{
        pause							// spin and wait loop: Performance
		bts     dword ptr[eax], 0		// Bit0 -> CarryFlag, 1 -> Bit0 (Test and Set)
        jc      try_enter
	}
}

__forceinline void OrUnlock(volatile dword* _pLockVar)
{
	*_pLockVar = 0;
}*/

// *************************************EOF**************************************** //