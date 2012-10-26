// ******************************************************************************** //
// test_list.cpp																	//
// =============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::MultiMap.								//
//																					//
// ******************************************************************************** //

#include "OrBaseLib.h"
#include "OrDebug.h"
#include <iostream>
#include <thread>

#define NUM_THREADS 4

char* TEST_GROUPS[] = { "ABC", "keks", "nomnom", "explosion", "3DModel", "הצü" };
OrE::ADT::MultiMap* g_pMyMap;
void test_multimap_thread();

void test_multimap()
{
	OrE::ADT::MultiMap MyMap;
	g_pMyMap = &MyMap;

	std::cout << "******** Start test OrE::ADT::MultiMap ********\n";

	MyMap.Add( (void*)1, "keks" );
	MyMap.Add( (void*)2, "keks", "nomnom" );
	MyMap.Add( (void*)3, "nomnom" );

	MyMap.Unmap( (void*)3, "keks" );					// Test to remove from group, which does not includes object 3
	MyMap.Unmap( (void*)3, "nomnom" );					// Remove from its only group
	
	Assert( MyMap.IsIn( (void*)3 ) == false );
	Assert( MyMap.IsIn( (void*)2 ) == true );
	Assert( MyMap.IsIn( (void*)1, "keks" ) == true );

	// Double add -> object should only be in map once
	MyMap.Add( (void*)4, "nomnom" );
	MyMap.Add( (void*)4, "keks" );
	Assert( MyMap.GetNumElements() == 3 );

	// Iterator test
	auto It = MyMap.GetIterator( "nomnom" );
	int iCount = 0;
	while( ++It )
	{
		// Group has two elements
		Assert((uiptr)It == 2 || (uiptr)It == 4);
		++iCount;
	}
	Assert( iCount == 2 );

	It = MyMap.GetIterator();
	iCount = 0;
	while( ++It ) ++iCount;
	Assert( iCount == 3 );

	// This test causes an assertion
	// auto It2 = MyMap.GetIterator( "nom" );

	std::cout << "\tPassed single threaded test.\n";

	OrE::Utils::TimeQuerySlot Slot0;
	OrE::Utils::TimeQuery( Slot0 );
	std::thread aThreads[NUM_THREADS];
	for( int i=0; i<NUM_THREADS; ++i )
		aThreads[i] = std::thread( test_multimap_thread );

	for( int i=0; i<NUM_THREADS; ++i )
		aThreads[i].join();

	Assert( MyMap.GetNumElements() < 5000 );

	std::cout << "\tPassed multi threaded test.\n";
	std::cout << "\tPassed time for multi thread test: " << OrE::Utils::TimeQuery( Slot0 ) << " s\n";

	// Test area for profiling
	OrE::Utils::TimeQuery( Slot0 );
	//for( int i=0; i<11000; ++i )
	//	OrE::Algorithm::Rand(1,5000);
	//std::cout << "\tPassed time for 11000 rands: " << OrE::Utils::TimeQuery( Slot0 ) << " s\n";
	OrE::Utils::TimeQuery( Slot0 );
	test_multimap_thread();
	std::cout << "\tPassed time for test_multimap_thread: " << OrE::Utils::TimeQuery( Slot0 ) << " s\n";

	std::cout << '\n';
}

void test_multimap_thread()
{
	// Do 1000 turns of testing
	for( int i=0; i<1000; ++i )
	{
		const char* pcTurnGroup = TEST_GROUPS[OrE::Algorithm::Rand(0,5)];
		// Insert arbitrary object to arbitrary group
		g_pMyMap->Add( (void*)OrE::Algorithm::Rand(1,5000), pcTurnGroup );

		// Search 3 Objects
		g_pMyMap->IsIn( (void*)OrE::Algorithm::Rand(1,5000), TEST_GROUPS[OrE::Algorithm::Rand(0,5)] );
		g_pMyMap->IsIn( (void*)OrE::Algorithm::Rand(1,5000), TEST_GROUPS[OrE::Algorithm::Rand(0,5)] );
		g_pMyMap->IsIn( (void*)OrE::Algorithm::Rand(1,5000), TEST_GROUPS[OrE::Algorithm::Rand(0,5)] );

		// Iterate over one group (which assuredly exists)
		auto It = g_pMyMap->GetIterator( pcTurnGroup );
		int iCount = 0;
		while( ++It ) ++iCount;

		// Remove and unmap - perhaps
		g_pMyMap->Unmap( (void*)OrE::Algorithm::Rand(1,5000), TEST_GROUPS[OrE::Algorithm::Rand(0,5)] );
		g_pMyMap->Remove( (void*)OrE::Algorithm::Rand(1,5000) );
	}
}