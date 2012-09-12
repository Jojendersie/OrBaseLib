// ******************************************************************************** //
// test_math.cpp																	//
// =============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing and benchmarking of all mathematical functions.					//
//																					//
// ******************************************************************************** //

#include "../include/OrBaseLib.h"
#include <iostream>

using namespace OrE::Math;

void test_math()
{	
	OrE::Utils::TimeQuerySlot Time0;

	std::cout << "******** Start test OrE::Math ********\n";

	// Test all hit functions
	bool bSucc = true;
	Vec3 vFeedback;
	Ellipsoid E1(Vec3(0.0f), Vec3(1.0f));
	Ellipsoid E2(Vec3(2.0f, 0.0f, 0.0f), Vec3(1.1f, 0.5f, 2.0f));
	Ellipsoid E3(Vec3(2.0f, 0.1f, 0.0f), Vec3(1.0f, 1.0f, 2.0f));
	Ellipsoid E4(Vec3(0.1f, 3.0f, 0.0f), Vec3(0.8f, 3.5f, 0.5f));
	Sphere S1(Vec3(0.0f), 1.0f);
	Sphere S2(Vec3(1.0f, 1.0f, 0.0f), 0.35f);

	bSucc &= HitTest(E1, E2) == true;
	bSucc &= HitTest(E1, E3) == false;
	bSucc &= HitTest(E1, E4) == true;
	bSucc &= HitTest(E2, E4) == false;
	bSucc &= HitTest(S1, S2) == false;
	std::cout << (bSucc ? "\tHit testing: succedded\n" : "\tHit testing: failed\n") << '\n';

	std::cout << "******** Start benchmark OrE::Math ********\n";
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		HitTest(S1, S2);
	std::cout << "\tTime for 10000 HitTest (Sphere-Sphere) calls: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	for( int i=0; i<10000; ++i )
		HitTest(E1, E2);
	std::cout << "\tTime for 10000 HitTest (Ellipsoid-Ellipsoid) calls: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	for( int i=0; i<10000; ++i )
		HitDetection(E1, E2, vFeedback);
	std::cout << "\tTime for 10000 HitDetection (Ellipsoid-Ellipsoid) calls: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	std::cout << '\n';
}