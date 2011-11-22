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
class OrVector2;
class OrVector3;
class OrVector4;
class OrMatrix;
class OrMatrix2x3;

// ******************************************************************** //
// Atomare Funktion zum Sichern kritischer Bereiche
inline bool OrLock(volatile dword* _pLockVar)
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
}

// *************************************EOF**************************************** //