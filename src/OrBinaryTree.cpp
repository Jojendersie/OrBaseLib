#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"


// ******************************************************************************** //
// Standard search with a key
OrBinaryTreeNodeP OrBinaryTree::Search(qword _qwKey)
{
	// Use internal search, but do not return the node if it's just a "near by case".
	OrBinaryTreeNodeP pN = SearchNode(_qwKey);
	return (pN && pN->qwKey == _qwKey)?pN:nullptr;
}

// ******************************************************************************** //
OrBinaryTreeNodeP OrBinaryTree::GetFirst()
{
	return Min(m_pRoot);
}

// ******************************************************************************** //
OrBinaryTreeNodeP OrBinaryTree::GetLast()
{
	return Max(m_pRoot);
}

// ******************************************************************************** //
OrBinaryTreeNodeP OrBinaryTree::GetNext(OrADTElementP _pCurrent)
{
	// Inorder traverse -> left site was seen before
	if(OrBinaryTreeNodeP(_pCurrent)->pRight) return Min(OrBinaryTreeNodeP(_pCurrent)->pRight);
	else {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		while(IsRightChild(OrBinaryTreeNodeP(_pCurrent))) _pCurrent = OrBinaryTreeNodeP(_pCurrent)->pParent;
		return _pCurrent?OrBinaryTreeNodeP(_pCurrent)->pParent:nullptr;
	}
}

// ******************************************************************************** //
OrBinaryTreeNodeP OrBinaryTree::GetPrevious(OrADTElementP _pCurrent)
{
	// Inorder traverse -> right site was seen before
	if(OrBinaryTreeNodeP(_pCurrent)->pLeft) return Max(OrBinaryTreeNodeP(_pCurrent)->pLeft);
	else {
		while(IsLeftChild(OrBinaryTreeNodeP(_pCurrent))) _pCurrent = OrBinaryTreeNodeP(_pCurrent)->pParent;
		return _pCurrent?OrBinaryTreeNodeP(_pCurrent)->pParent:nullptr;
	}
}


// ******************************************************************************** //
// Exchange two nodes
void OrBinaryTree::Swap(OrBinaryTreeNodeP _pN1, OrBinaryTreeNodeP _pN2)
{
	// Save all pointers of one node in temporary swap memory.
	OrBinaryTreeNodeP pP = _pN1->pParent;
	OrBinaryTreeNodeP pL = _pN1->pLeft;
	OrBinaryTreeNodeP pR = _pN1->pRight;
	// Copy nodepointers from one to the other.
	_pN1->pParent = _pN2->pParent;
	_pN1->pLeft = _pN2->pLeft;
	_pN1->pRight = _pN2->pRight;
	// Fill second node from temporary variables.
	_pN2->pParent = pP;
	_pN2->pLeft = pL;
	_pN2->pRight = pR;
}

// ******************************************************************************** //
// Replaces the current node by its left child
OrBinaryTreeNodeP OrBinaryTree::RotateRight(OrBinaryTreeNodeP _pNode)
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
		OrBinaryTreeNodeP pO = _pNode->pLeft;
		_pNode->pLeft = pO->pRight;
		pO->pRight = _pNode;// change parents of all changed childs
		pO->pParent = _pNode->pParent;
		if(_pNode->pLeft) _pNode->pLeft->pParent = _pNode;
		_pNode->pParent = pO;
		// Not in the picture: Parent of N has now the new node as child
		if(pO->pParent)
		{
			if(pO->pParent->pLeft == _pNode)
				pO->pParent->pLeft = pO;
			else pO->pParent->pRight = pO;
		} else m_pRoot = pO;
		return pO;
	}
	return nullptr;
}

// ******************************************************************************** //
// Replaces the current node by its right child
OrBinaryTreeNodeP OrBinaryTree::RotateLeft(OrBinaryTreeNodeP _pNode)
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
		OrBinaryTreeNodeP pO = _pNode->pRight;
		_pNode->pRight = pO->pLeft;
		pO->pLeft = _pNode;
		pO->pParent = _pNode->pParent;
		if(_pNode->pRight) _pNode->pRight->pParent = _pNode;
		_pNode->pParent = pO;
		// Not in the picture: Parent of N has now the new node as child
		if(pO->pParent)
		{
			if(pO->pParent->pLeft == _pNode)
				pO->pParent->pLeft = pO;
			else pO->pParent->pRight = pO;
		} else m_pRoot = pO;
		return pO;
	}
	return nullptr;
}

// ******************************************************************************** //
void OrBinaryTree::DeleteAll(OrBinaryTreeNodeP _pNode)
{
	// Redundant but not that worse
	m_pRoot = nullptr;
	// Release resources ...
	m_pDeleteCallback(_pNode->pObject);
	// ... children ...
	if(_pNode->pLeft) DeleteAll(_pNode->pLeft);
	if(_pNode->pRight) DeleteAll(_pNode->pRight);
	// ... the node (last reference)
	delete _pNode;
}

// ******************************************************************************** //
OrBinaryTree::~OrBinaryTree()
{
	DeleteAll(m_pRoot);
}

// *************************************EOF**************************************** //