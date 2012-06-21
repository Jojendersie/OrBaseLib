// ******************************************************************************** //
// OrList.cpp																		//
// ==========																		//
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
#include "..\include\OrList.h"
#include "..\include\OrDebug.h"


using namespace OrE::ADT;

// ******************************************************************************** //
void OrE::ADT::List::DeleteAll(ListNodeP _pNode)
{
	while(_pNode)
	{
		// Save next one
		ListNodeP pNext = _pNode->pRight;

		// Release resources ...
		if(m_pDeleteCallback) m_pDeleteCallback(_pNode->pObject);
		delete _pNode;

		// Go to the next one
		_pNode = pNext;
	}
}

// ******************************************************************************** //
OrE::ADT::List::~List()
{
	DeleteAll(m_pFirst);
}

// ******************************************************************************** //
// Standard operation insert
ListNodeP OrE::ADT::List::Insert(void* _pObject, qword _qwKey)
{
	// Create Node
	ListNodeP pNew = new ListNode(_pObject, _qwKey);
	// Insert at end of the list
	if(m_pLast)
	{
		m_pLast->pRight = pNew;
		pNew->pLeft = m_pLast;
		m_pLast = pNew;
	} else
	{
		// If there is no last element there is no element
		m_pFirst = pNew;
		m_pLast = pNew;
	}

	return pNew;
}

// ******************************************************************************** //
// Insert and allow each key only to occure one time (insert sorted as well)
ListNodeP OrE::ADT::List::SetInsert(void* _pObject, qword _qwKey)
{
	// Search
	ListNodeP pCurrent = m_pFirst;
	while(pCurrent && pCurrent->qwKey < _qwKey) pCurrent = pCurrent->pRight;

	// List empty or the new element has to be the last one
	if(!pCurrent) return Insert(_pObject, _qwKey);

	// If the current element has the same key do nothing (set property)
	if(pCurrent->qwKey != _qwKey)
	{
		// Insert left of the current elmenent (which has a larger key value)
		// Create Node
		ListNodeP pNew = new ListNode(_pObject, _qwKey);
		if(pCurrent->pLeft) pCurrent->pLeft->pRight = pNew;
		else m_pFirst = pNew;
		pNew->pLeft = pCurrent->pLeft;
		pNew->pRight = pCurrent;
		pCurrent->pLeft = pNew;
		return pNew;
	}

	pCurrent->AddRef();
	return pCurrent;
}

// ******************************************************************************** //
// Slow delete with search
void OrE::ADT::List::Delete(qword _qwKey)
{
	Iterator<ListNode> It(this);
	while(++It)
		if(It->qwKey == _qwKey)
		{
			Delete(ADTElementP(It));
			return;
		}
}

// ******************************************************************************** //
// Fast delete
void OrE::ADT::List::Delete(ADTElementP _pElement)
{
	if(_pElement->Release()<=0)
	{
		// Left neighbor or if this is the first element the m_pFirst element has to be corrected
		if(ListNodeP(_pElement)->pLeft) ListNodeP(_pElement)->pLeft->pRight = ListNodeP(_pElement)->pRight;
		else m_pFirst = ListNodeP(_pElement)->pRight;
		// The same on the right side
		if(ListNodeP(_pElement)->pRight) ListNodeP(_pElement)->pRight->pLeft = ListNodeP(_pElement)->pLeft;
		else m_pLast = ListNodeP(_pElement)->pLeft;

		// Release resources ...
		if(m_pDeleteCallback) m_pDeleteCallback(_pElement->pObject);
		delete _pElement;
	}
}

// ******************************************************************************** //
// Standard search with a key
ListNodeP OrE::ADT::List::Search(qword _qwKey)
{
	Iterator<ListNode> It(this);
	while(++It)
		if(It->qwKey == _qwKey) return &It;
	return nullptr;
}

// *************************************EOF**************************************** //