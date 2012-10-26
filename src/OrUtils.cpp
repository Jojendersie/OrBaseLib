// ******************************************************************************** //
// OrRandom.cpp																		//
// ============																		//
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
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "..\include\OrTypeDef.h"
#include "..\include\OrUtils.h"
#include "..\include\OrFastMath.h"

using namespace OrE::Math;
using namespace OrE::Utils;

// ******************************************************************************** //
// TimeQuery() returns the number of passed seconds scince the last call.

#ifdef WINDOWS
#include "windows.h"
#endif
const double CLOCKS_PER_SEC_INV = 1.0/CLOCKS_PER_SEC;	// Precalculated for performance

double OrE::Utils::TimeQuery(TimeQuerySlot& _Slot)
{
	// Remember old timestamp and sample new one
	qword OldTime = _Slot;
#ifdef WINDOWS
	qword qwF;
	QueryPerformanceFrequency((LARGE_INTEGER*)&qwF);
	QueryPerformanceCounter((LARGE_INTEGER*)&_Slot);
#else
	g_QueryCounter[_Slot] = clock();
#endif
	// Convergate difference to seconds
	return (_Slot - OldTime)/double(qwF);
}

// ******************************************************************************** //
// Finds out if TimeQuery() with the current slot was used scince program start.
/*bool OrE::Utils::IsTimeSlotUsed(TimeQuerySlots _Slot)
{
	return g_QueryCounter[_Slot]!=0;
}*/

// ******************************************************************************** //
// Returns the time elapsed since the program was started in seconds.
float OrE::Utils::TimeSinceProgramStart()
{
	return float(clock()*CLOCKS_PER_SEC_INV);
}

// ******************************************************************************** //
// Robust auxiliary function to check if *p1 != *p2
bool OrE::Utils::Strneq(const char* p1, const char* p2)
{
	if(p1==p2) return false;
	if(p1==0) return true;	// p2 muss ja !=p1 sein und ist !=0
	if(p2==0) return true;
	return strcmp(p1, p2)!=0;
}

// ******************************************************************************** //
// Auxiliary function to create a copy of a subsrting.
// Parameter:
//	_dwFrom - 0-indexed index of first char to copy (inclusive)
//	_dwTo - 0-indexed index of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix begining in _dwFrom
char* OrE::Utils::Substr(const char* _pcString, const dword _dwFrom, dword _dwTo)
{
	// Spezialfall: String bis zum Ende
	if(_dwTo == 0xffffffff) _dwTo = Max(0,int(strlen(_pcString))-1);

	// Definition: von 0 bis 0 bedeutet wir wollen ein Zeichen
	char* pcRet = (char*)malloc((_dwTo+2-_dwFrom)*sizeof(char));
	dword i=_dwFrom;
	for(;i<=_dwTo;++i)
		pcRet[i-_dwFrom] = _pcString[i];
	pcRet[_dwTo-_dwFrom+1] = 0;

	return pcRet;
}

// ******************************************************************************** //
dword OrE::Utils::Match(const char* _pcStr1, const char* _pcStr2)
{
	//dword dwMax = OrE::Math::Min(strlen(_pcStr1), strlen(_pcStr2));
	dword i;
	for(i=0; (_pcStr1[i]*_pcStr2[i] != 0) && (_pcStr1[i]==_pcStr2[i]); ++i);
	return i;
}

// ******************************************************************************** //
// Compare a string and ignore the case.
int OrE::Utils::Strcmpic(const char* _pcStr1, const char* _pcStr2)
{
	while(*_pcStr1 && *_pcStr2)
	{
		// "lexicographic" order and early 0 in one of the strings causes
		// one of the following returns.
		if(tolower(*_pcStr1) < tolower(*_pcStr2)) return -1;
		if(tolower(*_pcStr1) > tolower(*_pcStr2)) return 1;
		++_pcStr1; ++_pcStr2;
	}
	// One or both strings ended
	if(tolower(*_pcStr1) < tolower(*_pcStr2)) return -1;
	if(tolower(*_pcStr1) > tolower(*_pcStr2)) return 1;
	return 0;
}

// *************************************EOF**************************************** //