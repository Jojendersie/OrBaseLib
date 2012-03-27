// ******************************************************************************** //
// OrUtils.h																		//
// =========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by a comment at	https://github.com/Jojendersie/.	//
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
namespace Utils {

// ******************************************************************************** //
// Time measurement
// TimeQuery() returns the number of passed seconds scince the last call.
// There are 8 marks which can be used independently.
enum TimeQuerySlots {
	TIME_SLOT_0,
	TIME_SLOT_1,
	TIME_SLOT_2,
	TIME_SLOT_3,
	TIME_SLOT_4,
	TIME_SLOT_5,
	TIME_SLOT_6,
	TIME_SLOT_7
};
double TimeQuery(TimeQuerySlots _Slot);

// ******************************************************************************** //
// Returns the time elapsed since the program was started in seconds.
float TimeSinceProgramStart();

// ******************************************************************************** //
// String helper functions
// Robust auxiliary function to check if *p1 != *p2 (runs with 0 pointers...)
// Returns true if both pointers are equal
bool Strneq(const char* p1, const char* p2);

// Auxiliary function to create a copy of a subsrting.
// Parameter:
//	_dwFrom - 0-indexed index of first char to copy (inclusive)
//	_dwTo - 0-indexed index of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix begining in _dwFrom
// Returns a 0-terminated string which have to be deleted with free().
char* Substr(const char* _pcString, const dword _dwFrom, dword _dwTo);

// Counts number of matching characters
dword Match(const char* _pcStr1, const char* _pcStr2);

};
};

// *************************************EOF**************************************** //