// ******************************************************************************** //
// test_list.cpp																	//
// =============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::List.									//
//																					//
// ******************************************************************************** //

#include "../include/OrBaseLib.h"
#include <iostream>

void test_list()
{
	OrE::ADT::List MyList;
	
	std::cout << "******** Start test OrE::ADT::List ********\n";

	// Add primitive data (intergers) to the list
	OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 );
	for( int i=0; i<10000; ++i )
		MyList.Insert( (void*)OrE::Algorithm::Rand(0,1000), i );

	std::cout << "\tTime for 10000 insertions: " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	// Iterate over all 10000 elements and change data
	OrE::ADT::Iterator<OrE::ADT::ListNode> It(&MyList);
	while(++It) It->pObject = (void*)((int)It->pObject + 500);

	std::cout << "\tTime to iterate over all 10000 elements: " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	// Search 1000 elements (in list and not in list 50%)
	for( int i=0; i<1000; ++i )
		MyList.Search( OrE::Algorithm::Rand(0,20000) );

	std::cout << "\tTime to search for 1000 elements: " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	// Delete 1000 elements
	for( int i=0; i<1000; ++i )
		MyList.Delete( OrE::Algorithm::Rand(0,10000) );

	std::cout << "\tTime to remove 1000 elements: " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	MyList.Clear();
	std::cout << "\tTime to remove everything else with Clear(): " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	// Add primitive data (intergers) to the list
	OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 );
	for( int i=0; i<10000; ++i )
		MyList.SetInsert( (void*)OrE::Algorithm::Rand(0,1000), i );

	std::cout << "\tTime for 10000 SetInsert (Insertionsort Worst Case) " << OrE::Utils::TimeQuery( OrE::Utils::TIME_SLOT_0 ) << '\n';

	std::cout << '\n';
}