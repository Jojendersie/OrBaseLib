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
	struct ListNode: public ADTElement
	{
		// Make nodes uneditable for all users except the lists. So the lists kann return
		// references to non constant nodes without any (large) risk.
		friend class List;
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
	protected:
		ListNodeP m_pFirst;
		ListNodeP m_pLast;

	public:
		List():m_pFirst(nullptr), m_pLast(nullptr) {}
		~List();

		virtual ListNodeP Insert(void* _pObject, qword _qwKey) override;	// Standard operation insert
		ListNodeP SetInsert(void* _pObject, qword _qwKey);					// Insert and allow each key only to occure one time (insert sorted as well)
		virtual void Delete(qword _qwKey) override;							// Slow delete with search
		virtual void Delete(ADTElementP _pElement) override;				// Fast delete

		// Implementation of navigation
		virtual ListNodeP Search(qword _qwKey) override;					// Standard search with a key
		virtual ListNodeP GetFirst() override		{return m_pFirst;}		// List access for iterator
		virtual ListNodeP GetLast() override		{return m_pLast;}		// List access for iterator
		virtual ListNodeP GetNext(ADTElementP _pCurrent) override		{return _pCurrent?ListNodeP(_pCurrent)->pRight:nullptr;}	// Traverse
		virtual ListNodeP GetPrevious(ADTElementP _pCurrent) override	{return _pCurrent?ListNodeP(_pCurrent)->pLeft:nullptr;}		// Traverse
	};
	typedef List* ListP;
};	// namespace ADT
};	// namespace OrE