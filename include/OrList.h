// ******************************************************************************** //
// OrList.h																			//
// ========																			//
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
//																					//
// ******************************************************************************** //
// Simple list types to store dynamic datasets.										//
// The list are double linked.														//
//																					//
//	Insert()					O(1)												//
//	SetInsert()					O(n)												//
//	Search()					O(n)												//
//	Delete(Key)					O(n)												//
//	Delete(Element)				O(1)												//
//	GetFirst()					O(1)												//
//	GetLast()					O(1)												//
//	GetNext()					O(1)												//
//	GetPrevious()				O(1)												//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

	// ******************************************************************************** //
	// Simple node for all double linked lists
	class ListNode: public ADTElement
	{
		// Make nodes uneditable for all users except the lists. So the lists kann return
		// references to non constant nodes without any (large) risk.
		friend class List;

		// Prevent copy constructor and operator = being generated.
		ListNode(const ListNode&);
		const ListNode& operator = (const ListNode&);
	protected:
		ListNode* pLeft;
		ListNode* pRight;

		ListNode(void* _pObj, const qword& _qwKey):ADTElement(_pObj, _qwKey), pLeft(nullptr),pRight(nullptr)	{}
	};
	typedef ListNode* ListNodeP;
	typedef ListNode const * ListNodePC;

	// ******************************************************************************** //
	// The double linked list
	class List: public ADT
	{
	private:
		void DeleteAll(ListNodeP _pNode);
		// Prevent copy constructor and operator = being generated.
		List(const List&);
		const List& operator = (const List&);
	protected:
		ListNodeP m_pFirst;
		ListNodeP m_pLast;

	public:
		List():m_pFirst(nullptr), m_pLast(nullptr) {}
		virtual ~List();

		ListNodeP Insert(void* _pObject, qword _qwKey) override;	// Standard operation insert
		ListNodeP SetInsert(void* _pObject, qword _qwKey);			// Insert and allow each key only to occure one time (insert sorted as well)
		void Delete(qword _qwKey) override;							// Slow delete with search
		void Delete(ADTElementP _pElement) override;				// Fast delete
		void Clear() override;										// Remove everything

		// Implementation of navigation
		ListNodeP Search(qword _qwKey) override;					// Standard search with a key
		ListNodeP GetFirst() override		{return m_pFirst;}		// List access for iterator
		ListNodeP GetLast() override		{return m_pLast;}		// List access for iterator
		ListNodeP GetNext(ADTElementP _pCurrent) override		{return _pCurrent?ListNodeP(_pCurrent)->pRight:nullptr;}	// Traverse
		ListNodeP GetPrevious(ADTElementP _pCurrent) override	{return _pCurrent?ListNodeP(_pCurrent)->pLeft:nullptr;}		// Traverse
	};
	typedef List* ListP;
};	// namespace ADT
};	// namespace OrE
// *************************************EOF**************************************** //