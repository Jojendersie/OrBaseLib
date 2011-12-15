// ******************************************************************************** //
// OrHeap.h																			//
// ========																			//
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
// Implementing a priority queue (Fibonacci Heap).									//
//																					//
// HashMap:																			//
//	Insert()					O(1)												//
//	Search()					O() - not supported									//
//	Delete(Key)					O()	- not supported									//
//	Delete(Element)				O(log n)											//
//	DeleteMin(Element)			O(log n)											//
//	Min							O(1)												//
//	DecreaseKey(Element)		O(1)												//
//	GetFirst()					O(1)												//
//	GetLast()					O(1)												//
//	GetNext()					O(1)												//
//	GetPrevious()				O(1)												//
//																					//
// It would be no problem to provide Search and key-Delete with O(n) but a key can	//
// occure multiple times in a heap. Therefore such an operation would be unsecurly.	//
// To do some searching you can use the iterator.									//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

	// ******************************************************************************** //
	// One node in the fibonacci heap
	struct HeapNode: public ADTElement
	{
		// Make nodes uneditable for all users except the heap.
		friend class Heap;
	protected:
		// Linked list of nodes which are siblings
		HeapNode* pLeft;
		HeapNode* pRight;
		// The parent node in the tree structure
		HeapNode* pParent;
		// One arbitary child. (The other children can be reached through the linked list)
		HeapNode* pChild;
		//int iParam;					// Statistic information to mark nodes (marking = a child of this marked node was cut before)
		int iDegree;				// Number of children

		// Constructor inserting in the structur
		HeapNode(HeapNode* _pParent, HeapNode* _pSibling, void* _pObj, const qword& _qwKey):
			ADTElement(_pObj, _qwKey), pChild(nullptr), pParent(_pParent), iDegree(0)
		{
			if(_pSibling)
			{
				// Double linked list (ring)
				pLeft = _pSibling;
				pRight = _pSibling->pRight;
				_pSibling->pRight->pLeft = this;
				_pSibling->pRight = this;
			} else pLeft = pRight = this;
			if(_pParent) ++_pParent->iDegree;
		}
	};
	typedef HeapNode* HeapNodeP;
	typedef HeapNode const * HeapNodePC;

	// ******************************************************************************** //
	class Heap: public ADT
	{
	protected:
		HeapNodeP m_pRoot;

		void Meld(HeapNodeP _pPartition);										// Meld / Merge of a part of a heap into this heap
		void Consolidate();														// Ensure that now two roots have the same degree
		void Cut(HeapNodeP _pElement);											// Cuts one element and insert it to the root list. (Similar to meld, but do not affekt the siblings of the element)
	public:
		Heap():m_pRoot(nullptr) {}
		~Heap();

		// Heap operations
		void* DeleteMin();														// Delete minimum element and return the data (element is deleted)
		HeapNodeP Min();														// Show the minimum element
		void ChangeKey(HeapNodeP _pElement, qword _qwNewKey);					// Change the key value and reorder the elements if necessary

		// general ADT operations
		virtual HeapNodeP Insert(void* _pObject, qword _qwKey) override;		// Standard operation insert
		virtual void Delete(qword _qwKey) override;								// Unsupported function (doing nothing)
		virtual void Delete(ADTElementP _pElement) override;					// The only arbitary delete operation for the heap
		virtual HeapNodeP Search(qword _qwKey) override;						// Unsupported function (returning 0)
		virtual HeapNodeP GetFirst() override;									// First element = min element
		virtual HeapNodeP GetLast() override;
		virtual HeapNodeP GetNext(ADTElementP _pCurrent) override;				// Preorder traverse
		virtual HeapNodeP GetPrevious(ADTElementP _pCurrent) override;			// Preorder traverse
	};

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //