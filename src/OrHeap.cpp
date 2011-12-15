// ******************************************************************************** //
// OrHeap.cpp																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project. You can do this by a comment at	https://github.com/Jojendersie/.		//
// Futhermore you have to state this project as a source of your project.			//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHeap.h"
#include <assert.h>

using namespace OrE::ADT;

// ******************************************************************************** //
// Assertions for all methods:														//
//		- The degree of a node is the real number of children.						//
//		- The heap order is given (all children have larger keys).					//
//		- m_pRoot is always the min element.										//
//		- The pLeft and pRight pointers create the same ring buffer each. But they	//
//			go through this ring in the opposite direction.							//
// ******************************************************************************** //

// ******************************************************************************** //
OrE::ADT::Heap::~Heap()
{
	if(m_pDeleteCallback)
		while(m_pRoot) m_pDeleteCallback( DeleteMin() );
	else
		while(m_pRoot) DeleteMin();
}

// ******************************************************************************** //
// Meld / Merge of a part of a heap into this heap
void OrE::ADT::Heap::Meld(HeapNodeP _pPartition)
{
	// Removing old connections
	if(_pPartition->pParent) {_pPartition->pParent->pChild = nullptr; --_pPartition->pParent->iDegree;}

	// Adding to the root list
	if(m_pRoot)
	{
		// Set all parent pointers to 0
		HeapNodeP pPartR = _pPartition->pRight;
		do {
			pPartR->pParent = nullptr;
			pPartR = pPartR->pRight;
		} while(pPartR != _pPartition->pRight);
		// A <-> Root | <-> B <-> C <-> A
		// D <-> Part | <-> E <-> F <-> D
		// Cut open the two ring lists and connect them
		_pPartition->pRight = m_pRoot->pRight;	// Part -> B
		m_pRoot->pRight->pLeft = _pPartition;	// B -> Part
		m_pRoot->pRight = pPartR;				// Root -> E
		pPartR->pLeft = m_pRoot;				// Root <- E
	} else m_pRoot = _pPartition;

	// Min update can only occure if the partition is of an other heap
	if(_pPartition->qwKey < m_pRoot->qwKey) m_pRoot = _pPartition;
}

// ******************************************************************************** //
// Ensure that now two roots have the same degree
void OrE::ADT::Heap::Consolidate()
{
	// Array to save trees with respect to there degree. This staticaly form only
	// allows a certain number of elments in the heap. (TODO: could be dynamic)
	// Maximum degree is O(log n) (Theorem - can be prooven).
	// -> O(exp(maxdegree)) nodes -> maxdegree=32 is heigh enough for everything
	// (constant factor is > 2)
	HeapNodeP aDegrees[32] = {nullptr};
	// Traverse root list
	HeapNodeP pCurrent = m_pRoot;
	do {
		assert(!pCurrent->pParent);
		// Is there an other node with the same degree?
		if(aDegrees[pCurrent->iDegree])
		{
			do {
				// Remove the bigger one from the root list and insert this
				// as a new child from the smaller one.
				HeapNodeP pRef = aDegrees[pCurrent->iDegree];
				if(pRef->qwKey < pCurrent->qwKey)
				{
					pCurrent->pParent = pRef;
					pCurrent->pLeft->pRight = pCurrent->pRight;
					pCurrent->pRight->pLeft = pCurrent->pLeft;
					if(pRef->pChild)
					{
						pCurrent->pRight = pRef->pChild->pRight;
						pRef->pChild->pRight->pLeft = pCurrent;
						pCurrent->pLeft = pRef->pChild;
						pRef->pChild->pRight = pCurrent;
					} else
					{
						pRef->pChild = pCurrent;
						pCurrent->pLeft = pCurrent->pRight = pCurrent;
					}

					pCurrent = pRef;	// Skip back in traversation (degree of the smaller one will be updated after this if-else statement)
				} else {
					pRef->pParent = pCurrent;
					pRef->pLeft->pRight = pRef->pRight;
					pRef->pRight->pLeft = pRef->pLeft;
					if(pCurrent->pChild)
					{
						pRef->pRight = pCurrent->pChild->pRight;
						pCurrent->pChild->pRight->pLeft = pRef;
						pRef->pLeft = pCurrent->pChild;
						pCurrent->pChild->pRight = pRef;
					} else
					{
						pCurrent->pChild = pRef;
						pRef->pLeft = pRef->pRight = pRef;
					}
				}
				aDegrees[pCurrent->iDegree] = nullptr;
				++pCurrent->iDegree;

				// Repeat if new degree is occupied too
			} while(aDegrees[pCurrent->iDegree]);
		} else
			aDegrees[pCurrent->iDegree] = pCurrent;

		// Traverse
		pCurrent = pCurrent->pRight;
	} while(pCurrent != m_pRoot);
}

// ******************************************************************************** //
// Standard operation insert
HeapNodeP OrE::ADT::Heap::Insert(void* _pObject, qword _qwKey)
{
	HeapNodeP pNew = new HeapNode(nullptr, m_pRoot, _pObject, _qwKey);
	// Min update
	if((!m_pRoot) || (pNew->qwKey<m_pRoot->qwKey))
		m_pRoot = pNew;
	return pNew;
}

// ******************************************************************************** //
// Delete minimum element and return the data (element is deleted)
void* OrE::ADT::Heap::DeleteMin()
{
	if(!m_pRoot) return nullptr;
	// Remember data
	void* pData = m_pRoot->pObject;
	HeapNodeP pNode = m_pRoot;
	// Set the root pointer to an arbitary other node
	if(m_pRoot->pLeft != m_pRoot)
	{
		m_pRoot->pLeft->pRight = m_pRoot->pRight;
		m_pRoot->pRight->pLeft = m_pRoot->pLeft;
		m_pRoot = m_pRoot->pLeft;
		// Meld the children and the now consistent remaining root list
		if(m_pRoot->pChild) Meld(m_pRoot->pChild);
		// Restructur the heap
		Consolidate();

		// Find new minimum
		HeapNodeP pStart = m_pRoot;
		HeapNodeP pCurrent = m_pRoot;
		do {
			if(pCurrent->qwKey < m_pRoot->qwKey)
				m_pRoot = pCurrent;
			pCurrent = pCurrent->pRight;
		} while(pCurrent != pStart);
	} else m_pRoot = m_pRoot->pChild;	// Meld and consolidate is now just copy

	// Delete and return the element
	delete pNode;
	return pData;
}

// ******************************************************************************** //
// Show the minimum element
HeapNodeP OrE::ADT::Heap::Min()
{
	return m_pRoot;
}

// ******************************************************************************** //
// Cuts one element and insert it to the root list. (Similar to meld, but
// do not affekt the siblings of the element.)
void OrE::ADT::Heap::Cut(HeapNodeP _pElement)
{
	// Remove from sibling list
	_pElement->pLeft->pRight = _pElement->pRight;
	_pElement->pRight->pLeft = _pElement->pLeft;

	// Remove from parent
	--_pElement->pParent->iDegree;
	if(_pElement->pParent->pChild == _pElement) _pElement->pParent->pChild = _pElement->pLeft;
	// Scince we safe ring buffers the left sibling could also be the node itself ->
	// check again -> this is/was the only child.
	if(_pElement->pParent->pChild == _pElement) _pElement->pParent->pChild = nullptr;
	_pElement->pParent = nullptr;

	// Insert to root list
	_pElement->pLeft = m_pRoot->pLeft;
	_pElement->pRight = m_pRoot;
	m_pRoot->pLeft->pRight = _pElement;
	m_pRoot->pLeft = _pElement;
}

// ******************************************************************************** //
// Change the key value and reorder the elements if necessary
void OrE::ADT::Heap::ChangeKey(HeapNodeP _pElement, qword _qwNewKey)
{
	// Set new value, it is changed anyway
	bool bIncrease = _pElement->qwKey<_qwNewKey;
	_pElement->qwKey = _qwNewKey;

	// Check if heap violated (decresed key)
	if(_pElement->pParent && (_pElement->qwKey < _pElement->pParent->qwKey))
	{
		// Cut simple (cascading in original tree, but I don't understand the advantage of the cascading)
		Cut(_pElement);
	}
	
	// Check violation if key value was increased
	if(bIncrease && _pElement->pChild)
	{
		// Compare all child values (unsorted and independent)
		HeapNodeP pChild = _pElement->pChild;
		do {
			HeapNodeP pNextChild = pChild->pRight;
			if(pChild->qwKey < _qwNewKey)
				// Send child to root list
				Cut(pChild);
			pChild = pNextChild;
		} while(pChild != _pElement->pChild);
	}

	// Check for new minimum
	if(_qwNewKey <= m_pRoot->qwKey)
	{
		assert(!_pElement->pParent);
		m_pRoot = _pElement;
	}
}

// ******************************************************************************** //
// Unsupported function
void OrE::ADT::Heap::Delete(qword _qwKey) {}

// ******************************************************************************** //
// The only arbitary delete operation for the heap
void OrE::ADT::Heap::Delete(ADTElementP _pElement)
{
	if(_pElement->Release() <= 0)
	{
		ChangeKey((HeapNodeP)_pElement, 0);
		// Removing node AND data
		if(m_pDeleteCallback)
			m_pDeleteCallback( DeleteMin() );
		else DeleteMin();
	}
}

// ******************************************************************************** //
// Unsupported function
HeapNodeP OrE::ADT::Heap::Search(qword _qwKey)
{
/*	Iterator<HeapNode> It(this);
	while(++It)
		if(*/
	return nullptr;
}

// ******************************************************************************** //
// First element = min element
HeapNodeP OrE::ADT::Heap::GetFirst()
{
	return m_pRoot;
}

// ******************************************************************************** //
// Traversing order goes right -> left is the last one in each ring
HeapNodeP OrE::ADT::Heap::GetLast()
{
	return m_pRoot->pLeft;
}

// ******************************************************************************** //
// Preorder traverse
HeapNodeP OrE::ADT::Heap::GetNext(ADTElementP _pCurrent)
{
	HeapNodeP pNode = HeapNodeP(_pCurrent);
	// Go to each child before traversing this level complete
	if(pNode->pChild) return pNode->pChild;
	// End of traversion in a ring: reached first element
	if(pNode->pParent && (pNode->pRight == pNode->pParent->pChild))
		// Parent was seen before going to the children.
		// We need the pNode->pParent->pRight. Changing pNode
		// results in pRight, because this is the only case after
		// this line. Advantage: we have not to check the end of recursion here.
		pNode = pNode->pParent;
	// End of traversion we reached the root (start)
	if(pNode->pRight == m_pRoot) return nullptr;
	// Default in a simple list - next one
	return pNode->pRight;
}

// ******************************************************************************** //
// Preorder traverse
HeapNodeP OrE::ADT::Heap::GetPrevious(ADTElementP _pCurrent)
{
	HeapNodeP pNode = HeapNodeP(_pCurrent);
	// Go to each child before traversing this level complete
	if(pNode->pChild) return pNode->pChild;
	// End of traversion in a ring: reached first element
	if(pNode->pParent && (pNode->pLeft == pNode->pParent->pChild))
		// Parent was seen before going to the children.
		// We need the pNode->pParent->pLeft. Changing pNode
		// results in pLeft, because this is the only case after
		// this line. Advantage: we have not to check the end of recursion here.
		pNode = pNode->pParent;
	// End of traversion we reached the root (start)
	if(pNode->pLeft == m_pRoot) return nullptr;
	// Default in a simple list - next one
	return pNode->pLeft;
}

// *************************************EOF**************************************** //