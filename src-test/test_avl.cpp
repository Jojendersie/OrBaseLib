// ******************************************************************************** //
// test_avl.cpp																		//
// ============																		//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::AVLTree.									//
//																					//
// ******************************************************************************** //

#include <iostream>
#include "../include/OrBaseLib.h"
#include "../include/OrDebug.h"

void test_avl()
{
	OrE::ADT::AVLTree MyAVL;
	OrE::Utils::TimeQuerySlot Time0;
	
	std::cout << "******** Start test OrE::ADT::AVLTree ********\n";

	// Add primitive data (intergers) to the list
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		MyAVL.Insert( (void*)OrE::Algorithm::Rand(0,1000), i );

	std::cout << "\tTime for 10000 insertions: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	std::cout << "\tTree height: " << MyAVL.GetHeight() << '\n';

	// Iterate over all 10000 elements and change data
	OrE::ADT::Iterator<OrE::ADT::BinaryTreeNode> It(&MyAVL);
	while(++It) {
		It->pObject = (void*)((int)It->pObject + 500);
		OrE::ADT::BinaryTreeNode* pNode = It;
		Assert( pNode->GetLeft() != pNode );
		Assert( pNode->GetRight() != pNode );
		Assert( pNode->GetParent() != pNode );
	}

	std::cout << "\tTime to iterate over all 10000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Search 1000 elements (in tree and not in tree 50%)
	for( int i=0; i<1000; ++i )
		MyAVL.Search( OrE::Algorithm::Rand(0,20000) );

	std::cout << "\tTime to search for 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Delete 1000 elements
	for( int i=0; i<1000; ++i )
	{
		MyAVL.Delete( OrE::Algorithm::Rand(0,10000) );
		while(++It) {
			OrE::ADT::BinaryTreeNode* pNode = It;
			Assert( pNode->GetLeft() != pNode );
			Assert( pNode->GetRight() != pNode );
			Assert( pNode->GetParent() != pNode );
		}
	}

	std::cout << "\tTime to remove 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	std::cout << '\n';
	MyAVL.Clear();



	OrE::ADT::LinkedAVLTree MyLAVL;
	
	std::cout << "******** Start test OrE::ADT::LinkedAVLTree ********\n";

	// Add primitive data (intergers) to the list
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		MyLAVL.Insert( (void*)OrE::Algorithm::Rand(0,1000), i );

	std::cout << "\tTime for 10000 insertions: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	std::cout << "\tTree height: " << MyLAVL.GetHeight() << '\n';

	// Iterate over all 10000 elements and change data
	OrE::ADT::Iterator<OrE::ADT::BinaryTreeLinkedNode> It2(&MyLAVL);
	while(++It2) It2->pObject = (void*)((int)It2->pObject + 500);

	std::cout << "\tTime to iterate over all 10000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Search 1000 elements (in tree and not in tree 50%)
	for( int i=0; i<1000; ++i )
		MyLAVL.Search( OrE::Algorithm::Rand(0,20000) );

	std::cout << "\tTime to search for 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Delete 1000 elements
	for( int i=0; i<1000; ++i )
		MyLAVL.Delete( OrE::Algorithm::Rand(0,10000) );

	std::cout << "\tTime to remove 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	std::cout << '\n';
}