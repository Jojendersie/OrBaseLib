// ******************************************************************************** //
// OrBinaryTree.cpp																	//
// ================																	//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"

using namespace OrE::ADT;

// ******************************************************************************** //
// Standard search with a key
BinaryTreeNodeP OrE::ADT::BinaryTree::Search(qword _qwKey)
{
	// Use internal search, but do not return the node if it's just a "near by case".
	BinaryTreeNodeP pN = SearchNode(_qwKey);
	return (pN && pN->qwKey == _qwKey)?pN:nullptr;
}

// ******************************************************************************** //
BinaryTreeNodeP OrE::ADT::BinaryTree::GetFirst()
{
	return Min(m_pRoot);
}

// ******************************************************************************** //
BinaryTreeNodeP OrE::ADT::BinaryTree::GetLast()
{
	return Max(m_pRoot);
}

// ******************************************************************************** //
BinaryTreeNodeP OrE::ADT::BinaryTree::GetNext(ADTElementP _pCurrent)
{
	// Inorder traverse -> left site was seen before
	if(BinaryTreeNodeP(_pCurrent)->pRight) return Min(BinaryTreeNodeP(_pCurrent)->pRight);
	else {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		while(IsRightChild(BinaryTreeNodeP(_pCurrent))) _pCurrent = BinaryTreeNodeP(_pCurrent)->pParent;
		return _pCurrent?BinaryTreeNodeP(_pCurrent)->pParent:nullptr;
	}
}

// ******************************************************************************** //
BinaryTreeNodeP OrE::ADT::BinaryTree::GetPrevious(ADTElementP _pCurrent)
{
	// Inorder traverse -> right site was seen before
	if(BinaryTreeNodeP(_pCurrent)->pLeft) return Max(BinaryTreeNodeP(_pCurrent)->pLeft);
	else {
		while(IsLeftChild(BinaryTreeNodeP(_pCurrent))) _pCurrent = BinaryTreeNodeP(_pCurrent)->pParent;
		return _pCurrent?BinaryTreeNodeP(_pCurrent)->pParent:nullptr;
	}
}


// ******************************************************************************** //
// Exchange two nodes
void OrE::ADT::BinaryTree::Swap(BinaryTreeNodeP _pN1, BinaryTreeNodeP _pN2)
{
	// Save all pointers of one node in temporary swap memory.
	BinaryTreeNodeP pP = _pN1->pParent;
	BinaryTreeNodeP pL = _pN1->pLeft;
	BinaryTreeNodeP pR = _pN1->pRight;
	// Copy node pointers from one to the other.
	_pN1->pParent = (_pN2->pParent == _pN1) ? _pN2 : _pN2->pParent;
	_pN1->pLeft = (_pN2->pLeft == _pN1) ? _pN2 : _pN2->pLeft;
	_pN1->pRight = (_pN2->pRight == _pN1) ? _pN2 : _pN2->pRight;
	// Fill second node from temporary variables.
	_pN2->pParent = (pP == _pN2) ? _pN1 : pP;
	_pN2->pLeft = (pL == _pN2) ? _pN1 : pL;
	_pN2->pRight = (pR == _pN2) ? _pN1 : pR;

	// Repair surrounding of node 1
	if( _pN1->pLeft ) _pN1->pLeft->pParent = _pN1;
	if( _pN1->pRight ) _pN1->pRight->pParent = _pN1;

	// Repair surrounding of node 2
	if( _pN2->pLeft ) _pN2->pLeft->pParent = _pN2;
	if( _pN2->pRight ) _pN2->pRight->pParent = _pN2;

	// Special case: exchange two children of the same node -> swap left and right
	if( _pN1->pParent == _pN2->pParent )
	{
		pL = _pN1->pParent->pLeft;
		_pN1->pParent->pLeft = _pN1->pParent->pRight;
		_pN1->pParent->pRight = pL;
	} else {
		// Repair surrounding of node 1
		if( _pN1->pParent )
		{
			if( _pN1->pParent->pLeft == _pN2 )
				_pN1->pParent->pLeft = _pN1;
			else if( _pN1->pParent->pRight == _pN2 )
				_pN1->pParent->pRight = _pN1;
		} else m_pRoot = _pN1;

		// Repair surrounding of node 2
		if( _pN2->pParent )
		{
			if( _pN2->pParent->pLeft == _pN1 )
				_pN2->pParent->pLeft = _pN2;
			else if( _pN2->pParent->pRight == _pN1 )
				_pN2->pParent->pRight = _pN2;
		} else m_pRoot = _pN2;
	}
}

// ******************************************************************************** //
// Replaces the current node by its left child
BinaryTreeNodeP OrE::ADT::BinaryTree::RotateRight(BinaryTreeNodeP _pNode)
{
	// Right rotation from node N
	//     -->
	//		N				    O
	//	^  / \  			   / \
	//  | O   C 		->	  A   N
	//	 / \				     / \
	//	A	B				    B   C
	if(_pNode->pLeft)
	{
		// Just replace some pointers (look in the picture)
		BinaryTreeNodeP pO = _pNode->pLeft;
		_pNode->pLeft = pO->pRight;
		pO->pRight = _pNode;
		// change parents of all changed childs
		pO->pParent = _pNode->pParent;
		_pNode->pParent = pO;
		if(_pNode->pLeft) _pNode->pLeft->pParent = _pNode;
		// Not in the picture: Parent of N has now the new node as child
		if(pO->pParent)
		{
			if(pO->pParent->pLeft == _pNode)
				pO->pParent->pLeft = pO;
			else pO->pParent->pRight = pO;
		} else m_pRoot = pO;
		return pO;
	}
	return _pNode;
}

// ******************************************************************************** //
// Replaces the current node by its right child
BinaryTreeNodeP OrE::ADT::BinaryTree::RotateLeft(BinaryTreeNodeP _pNode)
{
	// Left rotation from node N
	//     <--
	//		N				    O
	//	   / \  ^			   / \
	//    A   O |		->	  N   C
	//		 / \			 / \
	//		B   C			A   B
	if(_pNode->pRight)
	{
		// Just replace some pointers (look in the picture)
		BinaryTreeNodeP pO = _pNode->pRight;
		_pNode->pRight = pO->pLeft;
		pO->pLeft = _pNode;
		// change parents of all changed childs
		pO->pParent = _pNode->pParent;
		_pNode->pParent = pO;
		if(_pNode->pRight) _pNode->pRight->pParent = _pNode;
		// Not in the picture: Parent of N has now the new node as child
		if(pO->pParent)
		{
			if(pO->pParent->pLeft == _pNode)
				pO->pParent->pLeft = pO;
			else pO->pParent->pRight = pO;
		} else m_pRoot = pO;
		return pO;
	}
	return _pNode;
}

// ******************************************************************************** //
void OrE::ADT::BinaryTree::DeleteAll(BinaryTreeNodeP _pNode)
{
	// Redundant but not that worse
	m_pRoot = nullptr;
	// Release resources ...
	if(m_pDeleteCallback) m_pDeleteCallback(_pNode->pObject);
	// ... children ...
	if(_pNode->pLeft) DeleteAll(_pNode->pLeft);
	if(_pNode->pRight) DeleteAll(_pNode->pRight);
	// ... the node (last reference)
	delete _pNode;
}

// ******************************************************************************** //
OrE::ADT::BinaryTree::~BinaryTree()
{
	DeleteAll(m_pRoot);
}

// *************************************EOF**************************************** //