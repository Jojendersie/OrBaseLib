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

};
};

// *************************************EOF**************************************** //