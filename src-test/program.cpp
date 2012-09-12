// ******************************************************************************** //
// program.cpp																		//
// ===========																		//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Entry point for the test application. The test application shows		//
//			examples for usage and is the unit test for the engine at the same time.//
//																					//
// ******************************************************************************** //

#include <conio.h>
#include <iostream>

#include "../include/OrBaseLib.h"
#include "test.h"

int main()
{
	OrE::Algorithm::SRand( 2 );

	test_math();
	test_list();
	test_avl();

	std::cout << "\nPress any key to close ...\n";
	_getch();
	return 0;
}