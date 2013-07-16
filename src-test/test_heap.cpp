// ******************************************************************************** //
// test_heap.cpp																	//
// =============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::Heap.									//
//																					//
// ******************************************************************************** //

#include <iostream>
#include "../include/OrBaseLib.h"
#include "../include/OrFibHeap.h"
#include "../include/OrDebug.h"

void test_heapt();

void test_heap()
{
	OrE::ADT::Heap MyHeap;
	OrE::Utils::TimeQuerySlot Time0;
	
	std::cout << "******** Start test OrE::ADT::Heap ********\n";

	// Add primitive data (intergers) to the heap
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		MyHeap.Insert( (void*)OrE::Algorithm::Rand(0,1000), i%2 );

	std::cout << "\tTime for 10000 insertions: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	MyHeap.DeleteMin();
	std::cout << "\tTime for first DeleteMin (O(n)): " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	// Iterate over all 1000 elements and change there keys and check there correctness
	OrE::ADT::Heap::Iterator It(&MyHeap);
	int x = 0;
	while(x < 1000 &&  ++It) {
		MyHeap.ChangeKey( It, OrE::Algorithm::Rand(0,10000) );
		OrE::ADT::HeapNode* pNode = It;
		// node assertions
#ifdef _DEBUG
//		It->CheckNode();
#endif
		++x;
	}
	// Not sure how heap reacts to changes in its structure during iteration (ChangeKey)
	Assert( x==1000 );

	std::cout << "\tIterate over 1000 elements, changing there keys\n\tand testing validity: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	MyHeap.Clear();
	std::cout << "\tDeleteMin (Clear) for all 9999 elements: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	std::cout << '\n';
	test_heapt();
}


void test_heapt()
{
	OrE::ADT::HeapT<int> MyHeap;
	OrE::Utils::TimeQuerySlot Time0;
	
	std::cout << "******** Start test OrE::ADT::Heap ********\n";

	// Add primitive data (intergers) to the heap
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		MyHeap.Insert( OrE::Algorithm::Rand(0,1000), i%2 );

	std::cout << "\tTime for 10000 insertions: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	MyHeap.Delete(MyHeap.Min());
	std::cout << "\tTime for first DeleteMin (O(n)): " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	// Iterate over all 1000 elements and change there keys and check there correctness
	auto It = MyHeap.GetFirst();
	int x = 0;
	while(x < 1000) {
		MyHeap.ChangeKey( It, OrE::Algorithm::Rand(0,10000) );
		It = MyHeap.GetNext(It);
		++x;
	}
	// Not sure how heap reacts to changes in its structure during iteration (ChangeKey)
	Assert( x==1000 );

	std::cout << "\tIterate over 1000 elements, changing there keys: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	MyHeap.Clear();
	std::cout << "\tDeleteMin (Clear) for all 9999 elements: " << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	std::cout << '\n';
}