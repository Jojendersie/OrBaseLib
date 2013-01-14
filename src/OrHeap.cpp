// ******************************************************************************** //
// OrHeap.cpp																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quite easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrAssert.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHeap.h"
#include "..\include\OrDebug.h"
//#include <assert.h>

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
#ifdef _DEBUG
	// Checks all invariants of a heap node. Call in debug only!
	void OrE::ADT::HeapNode::CheckNode()
	{
		// Check of the value of all children is larger than the current one
		HeapNode* pCurrent = pChild;
		int iNum = 0;
		if( pChild )
		{
			do
			{
				// Min-Heap property
				Assert( pCurrent->qwKey >= qwKey );
				iNum++;
				pCurrent = pCurrent->pRight;
			} while( pCurrent && pCurrent != pChild );
			// invariant of closed rings implicit, otherwise endless,... runs
		}
		// Degree invariant
		Assert( iDegree == iNum );
	}
#endif

// ******************************************************************************** //
OrE::ADT::Heap::~Heap()
{
	if(m_pDeleteCallback)
		while(m_pRoot) m_pDeleteCallback( DeleteMin() );
	else
		while(m_pRoot) DeleteMin();
}

// ******************************************************************************** //
// Merge of a part of a heap into the root list
void OrE::ADT::Heap::CutChildren(HeapNodeP _pPartition)
{
	// Removing old connections
	if(_pPartition->pParent) {_pPartition->pParent->pChild = nullptr; --_pPartition->pParent->iDegree;}

	// Set all parent pointers to 0
	// pPartR is used later - do not change into pPartR = _pPartition;
	HeapNodeP pPartR = _pPartition->pRight;
	do {
		pPartR->pParent = nullptr;
		pPartR = pPartR->pRight;
	} while(pPartR != _pPartition->pRight);

	// Adding to the root list
	if(m_pRoot)
	{
		// A <-> Root | <-> B <-> C <-> A
		// D <-> Part | <-> E <-> F <-> D
		// Cut open the two ring lists and connect them
		_pPartition->pRight = m_pRoot->pRight;	// Part -> B
		m_pRoot->pRight->pLeft = _pPartition;	// B -> Part
		m_pRoot->pRight = pPartR;				// Root -> E
		pPartR->pLeft = m_pRoot;				// Root <- E
	} else m_pRoot = _pPartition;

	// Min update can only occur if the partition is of an other heap
	//if(_pPartition->qwKey < m_pRoot->qwKey) m_pRoot = _pPartition;
}

// ******************************************************************************** //
// Ensure that now two roots have the same degree
void OrE::ADT::Heap::Consolidate()
{
	// Array to save trees with respect to there degree. This static form only
	// allows a certain number of elements in the heap. (TODO: could be dynamic)
	// Maximum degree is O(log n) (Theorem - can be proven).
	// -> O(exp(maxdegree)) nodes -> maxdegree=64 is high enough for everything
	// (constant factor is < 2)
	HeapNodeP aDegrees[64] = {nullptr};
	// Traverse root list
	HeapNodeP pCurrent = m_pRoot;
	do {
		Assert(!pCurrent->pParent);
		// Is there an other node with the same degree?
		if(aDegrees[pCurrent->iDegree] && (aDegrees[pCurrent->iDegree] != pCurrent))
		{
		//	assert(aDegrees[pCurrent->iDegree] != pCurrent);
			do {
				// Remove the bigger one from the root list and insert this
				// as a new child from the smaller one.
				HeapNodeP pRef = aDegrees[pCurrent->iDegree];
				if(pRef->qwKey < pCurrent->qwKey)
				{
					// Assert that the root element is always in the root list
					if(m_pRoot == pCurrent) m_pRoot = pCurrent->pRight;

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

					pCurrent = pRef;	// Skip back in traversing (degree of the smaller one will be updated after this if-else statement)
				} else {
					// Assert that the root element is always in the root list
					if(m_pRoot == pRef) m_pRoot = pRef->pRight;

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
HeapNodeP OrE::ADT::Heap::Insert(void* _pObject, uint64 _qwKey)
{
	HeapNodeP pNew = new HeapNode(m_pRoot, _pObject, _qwKey);
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
	// Set the root pointer to an arbitrary other node
	if(m_pRoot->pLeft != m_pRoot)
	{
		m_pRoot->pLeft->pRight = m_pRoot->pRight;
		m_pRoot->pRight->pLeft = m_pRoot->pLeft;
		m_pRoot = m_pRoot->pLeft;
	} else m_pRoot = nullptr;

	// Meld the children and the now consistent remaining root list
	if(pNode->pChild) CutChildren(pNode->pChild);

	if(m_pRoot)
	{
		// Restructure the heap
		Consolidate();

		// Find new minimum
		HeapNodeP pStart = m_pRoot;
		HeapNodeP pCurrent = m_pRoot;
		do {
			Assert(!pCurrent->pParent);
			if(pCurrent->qwKey < m_pRoot->qwKey)
				m_pRoot = pCurrent;
			pCurrent = pCurrent->pRight;
		} while(pCurrent != pStart);
	}

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
// do not affect the siblings of the element.)
void OrE::ADT::Heap::Cut(HeapNodeP _pElement)
{
	// Remove from sibling list
	_pElement->pLeft->pRight = _pElement->pRight;
	_pElement->pRight->pLeft = _pElement->pLeft;

	// Remove from parent
	--_pElement->pParent->iDegree;
	if(_pElement->pParent->pChild == _pElement) _pElement->pParent->pChild = _pElement->pLeft;
	// Since we safe ring buffers the left sibling could also be the node itself ->
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
void OrE::ADT::Heap::ChangeKey( HeapNodeP _pElement, uint64 _qwNewKey )
{
	// Set new value, it is changed anyway
	bool bIncrease = _pElement->qwKey<_qwNewKey;
	_pElement->qwKey = _qwNewKey;

	// Check if heap violated (decreased key)
	if( _pElement->pParent && (_pElement->qwKey < _pElement->pParent->qwKey) )
	{
		// Cut simple (cascading in original tree, but I don't understand the advantage of the cascading)
		Cut( _pElement );
	}
	
	// Check violation if key value was increased
	if( bIncrease && _pElement->pChild )
	{
		// Compare all child values (unsorted and independent)
		HeapNodeP pChild = _pElement->pChild;
		HeapNodeP pStop = _pElement->pChild;
		do {
			HeapNodeP pNextChild = pChild->pRight;
			if(pChild->qwKey < _qwNewKey)
				// Send child to root list
				Cut(pChild);
			pChild = pNextChild;
		} while( pChild != pStop );
	}

	// Check for new minimum
	if( _qwNewKey <= m_pRoot->qwKey )
	{
		Assert(!_pElement->pParent);
		m_pRoot = _pElement;
	}
}

// ******************************************************************************** //
// Unsupported function
void OrE::ADT::Heap::Delete(uint64 _qwKey) {}

// ******************************************************************************** //
// The only arbitrary delete operation for the heap
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
// Remove everything
void OrE::ADT::Heap::RecursiveDelete(HeapNodeP _pCurrent)
{
	// Recursive deletion
	if( _pCurrent->pChild )
		RecursiveDelete( _pCurrent->pChild );
	// Don't hang up in the Ring.
	_pCurrent->pLeft->pRight = nullptr;
	if( _pCurrent->pRight )
		RecursiveDelete( _pCurrent->pRight );

	// Delete the resources
	if( m_pDeleteCallback ) m_pDeleteCallback( _pCurrent->pObject );
	delete _pCurrent;
}

void OrE::ADT::Heap::Clear()
{
	if(m_pDeleteCallback)
		while(m_pRoot) m_pDeleteCallback( DeleteMin() );
	else
		while(m_pRoot) DeleteMin();
//	RecursiveDelete( m_pRoot );
	m_pRoot = nullptr;
}

// ******************************************************************************** //
// Unsupported function
HeapNodeP OrE::ADT::Heap::Search(uint64 _qwKey)
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
	// End of traversing in a ring: reached first element
	if(pNode->pParent && (pNode->pRight == pNode->pParent->pChild))
		// Parent was seen before going to the children.
		// We need the pNode->pParent->pRight. Changing pNode
		// results in pRight, because this is the only case after
		// this line. Advantage: we have not to check the end of recursion here.
		pNode = pNode->pParent;
	// End of traversing we reached the root (start)
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
	// End of traversing in a ring: reached first element
	if(pNode->pParent && (pNode->pLeft == pNode->pParent->pChild))
		// Parent was seen before going to the children.
		// We need the pNode->pParent->pLeft. Changing pNode
		// results in pLeft, because this is the only case after
		// this line. Advantage: we have not to check the end of recursion here.
		pNode = pNode->pParent;
	// End of traversing we reached the root (start)
	if(pNode->pLeft == m_pRoot) return nullptr;
	// Default in a simple list - next one
	return pNode->pLeft;
}

// *************************************EOF**************************************** //