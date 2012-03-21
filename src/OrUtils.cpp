// ******************************************************************************** //
// OrRandom.cpp																		//
// ============																		//
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
// Implmentation of different pseudo-random generators.								//
//																					//
// ******************************************************************************** //

#include <ctime>
#include "..\include\OrTypeDef.h"
#include "..\include\OrUtils.h"

using namespace OrE::Math;
using namespace OrE::Utils;

// ******************************************************************************** //
// TimeQuery() returns the number of passed seconds scince the last call.
// There are 8 marks which can be used independently.
clock_t g_QueryCounter[8] = {0};						// The 8 Timemarks
const double CLOCKS_PER_SEC_INV = 1.0/CLOCKS_PER_SEC;	// Precalculated for performance
double OrE::Utils::TimeQuery(TimeQuerySlots _Slot)
{
	// Remember old timestamp and sample new one
	clock_t OldTime = g_QueryCounter[_Slot];
	g_QueryCounter[_Slot] = clock();
	// Convergate difference to seconds
	return (g_QueryCounter[_Slot] - OldTime)*CLOCKS_PER_SEC_INV;
}

// ******************************************************************************** //
// Returns the time elapsed since the program was started in seconds.
float OrE::Utils::TimeSinceProgramStart()
{
	return float(clock()*CLOCKS_PER_SEC_INV);
}

// *************************************EOF**************************************** //