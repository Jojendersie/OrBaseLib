// ******************************************************************************** //
// OrAVLTree.cpp																	//
// =============																	//
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
#include "..\include\OrAVLTree.h"
#include "..\include\OrFastMath.h"
#include <assert.h>

using namespace OrE::ADT;

// ******************************************************************************** //
// Rebuild AVL property, if and only if _pNode changed it's height by one.
// (_pNode has to be the parent of the inserted or deleted node)
void OrE::ADT::AVLTree::Rebalance(BinaryTreeNodeP _pNode)
{
	bool bChanged = true;
	while(bChanged && _pNode)
	{
		int iHl = GetHeight(_pNode->pLeft);
		int iHr = GetHeight(_pNode->pRight);
		// Rebalance?
		if(iHl < iHr-1)			// Balance +2
		{
			int iHrl = GetHeight(_pNode->pRight->pLeft);
			int iHrr = GetHeight(_pNode->pRight->pRight);
			if(iHrl>iHrr)		// right-left case
				RotateRight(_pNode->pRight);
			// right-right case
			RotateLeft(_pNode);
		} else if(iHl-1 > iHr)	// Balance -2
		{
			int iHll = GetHeight(_pNode->pLeft->pLeft);
			int iHlr = GetHeight(_pNode->pLeft->pRight);
			if(iHll<iHlr)		// left-right case
				RotateLeft(_pNode->pLeft);
			// left-left case
			RotateRight(_pNode);
		}

		// Renew height for all possible touched nodes
		if(_pNode->pLeft) _pNode->pLeft->iParam = Math::Max(GetHeight(_pNode->pLeft->pLeft), GetHeight(_pNode->pLeft->pRight))+1;
		if(_pNode->pRight) _pNode->pRight->iParam = Math::Max(GetHeight(_pNode->pRight->pLeft), GetHeight(_pNode->pRight->pRight))+1;
		int iNewHeight = Math::Max(GetHeight(_pNode->pLeft), GetHeight(_pNode->pRight))+1;
		bChanged = (iNewHeight!=_pNode->iParam);
		_pNode->iParam = iNewHeight;

		_pNode = _pNode->pParent;
	}
}

// ******************************************************************************** //
// Search with a key (searches for nearest element)
BinaryTreeNodeP OrE::ADT::AVLTree::SearchNode(qword _qwKey)
{
	if(!m_pRoot) return nullptr;
	// Search normal
	BinaryTreeNodeP pCurrent = m_pRoot;
	while(pCurrent->qwKey!=_qwKey)
	{
		if(pCurrent->qwKey<_qwKey)	// Search in the right tree
		{
			if(!pCurrent->pRight) return pCurrent;	// Return nearest one if not found
			pCurrent = pCurrent->pRight;
		} else {			// Search in the left tree
			if(!pCurrent->pLeft) return pCurrent;	// Return nearest one if not found
			pCurrent = pCurrent->pLeft;
		}
	}

	return pCurrent;
}

// ******************************************************************************** //
// Insert operation
BinaryTreeNodeP OrE::ADT::AVLTree::Insert(void* _pObject, qword _qwKey)
{
	// Search for the element. If already in tree just increase the reference counter.
	BinaryTreeNodeP pParent = SearchNode(_qwKey);
	BinaryTreeNodeP pNewNode = nullptr;
	if(!pParent)
	{
		// Insert first node
		m_pRoot = new BinaryTreeNode(nullptr, _pObject, _qwKey);
		m_pRoot->iParam = 1;
		return m_pRoot;
	} else if(_qwKey == pParent->qwKey)
	{
		// Result can be returned instantly because the balancing does not change
		pParent->AddRef();
		return pParent;
	} else {
		// Otherwise we now know the parent for the new node.
		pNewNode = new BinaryTreeNode(pParent, _pObject, _qwKey);
		pNewNode->iParam = 1;
		if(_qwKey < pParent->qwKey)
			pParent->pLeft = pNewNode;
		else
			pParent->pRight = pNewNode;
	}

	// Rebuild AVL property
	Rebalance(pParent);

	return pNewNode;
}

// ******************************************************************************** //
// Standard operation delete
void OrE::ADT::AVLTree::Delete(qword _qwKey)
{
	// Search the node
	BinaryTreeNodeP pNode = Search(_qwKey);
	Delete(pNode);
}

// ******************************************************************************** //
// Faster delete operation without search
void OrE::ADT::AVLTree::Delete(ADTElementP _pNode)
{
	if(!_pNode) return;
	if(_pNode->Release()) return;	// Return if there is still an reference

	BinaryTreeNodeP pNode = (BinaryTreeNodeP)_pNode;
DeleteNow:
	// If node is an half-leaf/leaf delete
	if(!pNode->pRight)
	{
		// Remove references
		if(IsLeftChild(pNode)) pNode->pParent->pLeft = pNode->pLeft;
		else pNode->pParent->pRight = pNode->pLeft;
		if(pNode->pLeft) pNode->pLeft->pParent = pNode->pParent;
		BinaryTreeNodeP pParent = pNode->pParent;
		// Delete now
		if(m_pDeleteCallback) m_pDeleteCallback(pNode->pObject);
		delete pNode;
		// Rebuild AVL property
		Rebalance(pParent);
	} else if(!pNode->pLeft) {
		// Remove references
		if(IsLeftChild(pNode)) pNode->pParent->pLeft = pNode->pRight;
		else pNode->pParent->pRight = pNode->pRight;
		pNode->pRight->pParent = pNode->pParent;
		BinaryTreeNodeP pParent = pNode->pParent;
		// Delete now
		if(m_pDeleteCallback) m_pDeleteCallback(pNode->pObject);
		delete pNode;
		// Rebuild AVL property
		Rebalance(pParent);
	} else {
		// Replace the node with the largest of the left site (half leaf guaranteed)
		BinaryTreeNodeP _pLMax = Max(pNode->pLeft);
		Swap(pNode, _pLMax);

		// Delete the half-leaf
		goto DeleteNow;
	}
}

// ******************************************************************************** //
// Insert operation
BinaryTreeLinkedNodeP OrE::ADT::LinkedAVLTree::Insert(void* _pObject, qword _qwKey)
{
	// Insert as usual
	// Search for the element. If already in tree just increase the reference counter.
	BinaryTreeLinkedNodeP pParent = (BinaryTreeLinkedNodeP)SearchNode(_qwKey);
	BinaryTreeLinkedNodeP pNewNode = nullptr;
	if(!pParent)
	{
		// Insert first node
		m_pRoot = new BinaryTreeLinkedNode(nullptr, _pObject, _qwKey);
		m_pRoot->iParam = 1;
		return (BinaryTreeLinkedNodeP)m_pRoot;
	} else if(_qwKey == pParent->qwKey)
	{
		// Result can be returned instantly because the balancing does not change
		pParent->AddRef();
		return pParent;
	} else {
		// Otherwise we now know the parent for the new node.
		pNewNode = new BinaryTreeLinkedNode(pParent, _pObject, _qwKey);
		pNewNode->iParam = 1;
		if(_qwKey < pParent->qwKey)
			pParent->pLeft = pNewNode;
		else
			pParent->pRight = pNewNode;
	}

	assert(pNewNode->pLeft == nullptr);
	assert(pNewNode->pRight == nullptr);
	// Update double linked list. The Next or Prev element is the parent (assertions)!
	if(IsLeftChild(pNewNode))
	{
		pNewNode->pPrev = pParent->pPrev;
		pNewNode->pNext = pParent;
		if(pNewNode->pPrev) pNewNode->pPrev->pNext = pNewNode;
		BinaryTreeLinkedNodeP(pNewNode->pParent)->pPrev = pNewNode;
	} else
	{
		pNewNode->pNext = pParent->pNext;
		pNewNode->pPrev = pParent;
		if(pNewNode->pNext) pNewNode->pNext->pPrev = pNewNode;
		BinaryTreeLinkedNodeP(pNewNode->pParent)->pNext = pNewNode;
	}

	// Rebuild AVL property
	Rebalance(pParent);
	return pNewNode;
}

// ******************************************************************************** //
// Faster delete operation without search
void OrE::ADT::LinkedAVLTree::Delete(ADTElementP _pNode)
{
	// First repair the linked list
	if(BinaryTreeLinkedNodeP(_pNode)->pPrev) BinaryTreeLinkedNodeP(_pNode)->pPrev->pNext = BinaryTreeLinkedNodeP(_pNode)->pNext;
	if(BinaryTreeLinkedNodeP(_pNode)->pNext) BinaryTreeLinkedNodeP(_pNode)->pNext->pPrev = BinaryTreeLinkedNodeP(_pNode)->pPrev;
	// Then remove as usual
	AVLTree::Delete(_pNode);
}

// ******************************************************************************** //
// Fast inorder traverse
BinaryTreeLinkedNodeP OrE::ADT::LinkedAVLTree::GetNext(ADTElementP _pCurrent)
{
	return BinaryTreeLinkedNodeP(_pCurrent)->pNext;
}

// ******************************************************************************** //
// Fast inorder traverse
BinaryTreeLinkedNodeP OrE::ADT::LinkedAVLTree::GetPrevious(ADTElementP _pCurrent)
{
	return BinaryTreeLinkedNodeP(_pCurrent)->pPrev;
}

// *************************************EOF**************************************** //