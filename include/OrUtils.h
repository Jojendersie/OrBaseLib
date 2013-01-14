// ******************************************************************************** //
// OrUtils.h																		//
// =========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quite easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by a comment at	https://github.com/Jojendersie/.	//
//																					//
// For details on this project see: Readme.txt										//
//																					//
// ******************************************************************************** //
// Implementation of different pseudo-random generators.							//
//																					//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Utils {

// ******************************************************************************** //
// Time measurement
// TimeQuery() returns the number of passed seconds since the last call.
// Use can use arbitrary independent time slots.
typedef unsigned long long TimeQuerySlot;
double TimeQuery(TimeQuerySlot& _Slot);

// ******************************************************************************** //
// Finds out if TimeQuery() with the current slot was used since program start.
// bool IsTimeSlotUsed(TimeQuerySlots _Slot);

// ******************************************************************************** //
// Returns the time elapsed since the program was started in seconds.
float TimeSinceProgramStart();

// ******************************************************************************** //
// String helper functions
// Robust auxiliary function to check if *p1 != *p2 (runs with 0 pointers...)
// Returns true if both pointers are equal
bool Strneq(const char* p1, const char* p2);

// Auxiliary function to create a copy of a substring.
// Parameter:
//	_dwFrom - 0-indexed index of first char to copy (inclusive)
//	_dwTo - 0-indexed index of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix beginning in _dwFrom
// Returns a 0-terminated string which have to be deleted with free().
char* Substr(const char* _pcString, const uint32 _dwFrom, uint32 _dwTo);

// Counts number of matching characters
uint32 Match(const char* _pcStr1, const char* _pcStr2);

// Compare a string and ignore the case.
// returns -1 if str1 lexicographic smaller than str2
//			0 if str1 equal to str2
//			1 if str1 lexicographic bigger than str2
int Strcmpic(const char* _pcStr1, const char* _pcStr2);


};
};

// *************************************EOF**************************************** //