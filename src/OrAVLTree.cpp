#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"
#include "..\include\OrAVLTree.h"
#include "..\include\OrFastMath.h"
#include <assert.h>

// ******************************************************************************** //
// Rebuild AVL property, if and only if _pNode changed it's height by one.
// (_pNode has to be the parent of the inserted or deleted node)
void OrAVLTree::Rebalance(OrBinaryTreeNodeP _pNode)
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
		if(_pNode->pLeft) _pNode->pLeft->iParam = OrMax(GetHeight(_pNode->pLeft->pLeft), GetHeight(_pNode->pLeft->pRight))+1;
		if(_pNode->pRight) _pNode->pRight->iParam = OrMax(GetHeight(_pNode->pRight->pLeft), GetHeight(_pNode->pRight->pRight))+1;
		int iNewHeight = OrMax(GetHeight(_pNode->pLeft), GetHeight(_pNode->pRight))+1;
		bChanged = (iNewHeight!=_pNode->iParam);
		_pNode->iParam = iNewHeight;

		_pNode = _pNode->pParent;
	}
}

// ******************************************************************************** //
// Search with a key (searches for nearest element)
OrBinaryTreeNodeP OrAVLTree::SearchNode(qword _qwKey)
{
	if(!m_pRoot) return nullptr;
	// Search normal
	OrBinaryTreeNodeP pCurrent = m_pRoot;
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
OrBinaryTreeNodeP OrAVLTree::Insert(void* _pObject, qword _qwKey)
{
	// Search for the element. If already in tree just increase the reference counter.
	OrBinaryTreeNodeP pParent = SearchNode(_qwKey);
	OrBinaryTreeNodeP pNewNode = nullptr;
	if(!pParent)
	{
		// Insert first node
		m_pRoot = new OrBinaryTreeNode(nullptr, _pObject, _qwKey);
		m_pRoot->iParam = 1;
		return m_pRoot;
	} else if(_qwKey == pParent->qwKey)
	{
		// Result can be returned instantly because the balancing does not change
		pParent->AddRef();
		return pParent;
	} else {
		// Otherwise we now know the parent for the new node.
		pNewNode = new OrBinaryTreeNode(pParent, _pObject, _qwKey);
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
void OrAVLTree::Delete(qword _qwKey)
{
	// Search the node
	OrBinaryTreeNodeP pNode = Search(_qwKey);
	Delete(pNode);
}

// ******************************************************************************** //
// Faster delete operation without search
void OrAVLTree::Delete(OrADTElementP _pNode)
{
	if(!_pNode) return;
	if(_pNode->Release()) return;	// Return if there is still an reference

	OrBinaryTreeNodeP pNode = (OrBinaryTreeNodeP)_pNode;
DeleteNow:
	// If node is an half-leaf/leaf delete
	if(!pNode->pRight)
	{
		// Remove references
		if(IsLeftChild(pNode)) pNode->pParent->pLeft = pNode->pLeft;
		else pNode->pParent->pRight = pNode->pLeft;
		if(pNode->pLeft) pNode->pLeft->pParent = pNode->pParent;
		OrBinaryTreeNodeP pParent = pNode->pParent;
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
		OrBinaryTreeNodeP pParent = pNode->pParent;
		// Delete now
		if(m_pDeleteCallback) m_pDeleteCallback(pNode->pObject);
		delete pNode;
		// Rebuild AVL property
		Rebalance(pParent);
	} else {
		// Replace the node with the largest of the left site (half leaf guaranteed)
		OrBinaryTreeNodeP _pLMax = Max(pNode->pLeft);
		Swap(pNode, _pLMax);

		// Delete the half-leaf
		goto DeleteNow;
	}
}

// ******************************************************************************** //
// Insert operation
OrBinaryTreeLinkedNodeP OrLinkedAVLTree::Insert(void* _pObject, qword _qwKey)
{
	// Insert as usual
	// Search for the element. If already in tree just increase the reference counter.
	OrBinaryTreeLinkedNodeP pParent = (OrBinaryTreeLinkedNodeP)SearchNode(_qwKey);
	OrBinaryTreeLinkedNodeP pNewNode = nullptr;
	if(!pParent)
	{
		// Insert first node
		m_pRoot = new OrBinaryTreeLinkedNode(nullptr, _pObject, _qwKey);
		m_pRoot->iParam = 1;
		return (OrBinaryTreeLinkedNodeP)m_pRoot;
	} else if(_qwKey == pParent->qwKey)
	{
		// Result can be returned instantly because the balancing does not change
		pParent->AddRef();
		return pParent;
	} else {
		// Otherwise we now know the parent for the new node.
		pNewNode = new OrBinaryTreeLinkedNode(pParent, _pObject, _qwKey);
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
		OrBinaryTreeLinkedNodeP(pNewNode->pParent)->pPrev = pNewNode;
	} else
	{
		pNewNode->pNext = pParent->pNext;
		pNewNode->pPrev = pParent;
		if(pNewNode->pNext) pNewNode->pNext->pPrev = pNewNode;
		OrBinaryTreeLinkedNodeP(pNewNode->pParent)->pNext = pNewNode;
	}

	// Rebuild AVL property
	Rebalance(pParent);
	return pNewNode;
}

// ******************************************************************************** //
// Faster delete operation without search
void OrLinkedAVLTree::Delete(OrADTElementP _pNode)
{
	// First repair the linked list
	if(OrBinaryTreeLinkedNodeP(_pNode)->pPrev) OrBinaryTreeLinkedNodeP(_pNode)->pPrev->pNext = OrBinaryTreeLinkedNodeP(_pNode)->pNext;
	if(OrBinaryTreeLinkedNodeP(_pNode)->pNext) OrBinaryTreeLinkedNodeP(_pNode)->pNext->pPrev = OrBinaryTreeLinkedNodeP(_pNode)->pPrev;
	// Then remove as usual
	OrAVLTree::Delete(_pNode);
}

// ******************************************************************************** //
// Fast inorder traverse
OrBinaryTreeLinkedNodeP OrLinkedAVLTree::GetNext(OrADTElementP _pCurrent)
{
	return OrBinaryTreeLinkedNodeP(_pCurrent)->pNext;
}

// ******************************************************************************** //
// Fast inorder traverse
OrBinaryTreeLinkedNodeP OrLinkedAVLTree::GetPrevious(OrADTElementP _pCurrent)
{
	return OrBinaryTreeLinkedNodeP(_pCurrent)->pPrev;
}

// *************************************EOF**************************************** //