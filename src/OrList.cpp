#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrList.h"

using namespace OrE::ADT;

// ******************************************************************************** //
void OrE::ADT::List::DeleteAll(ListNodeP _pNode)
{
	if(_pNode->pRight) DeleteAll(_pNode->pRight);

	// Release resources ...
	if(m_pDeleteCallback) m_pDeleteCallback(_pNode->pObject);
	delete _pNode;
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
// Slow delete with search
void OrE::ADT::List::Delete(qword _qwKey)
{
	Iterator<ListNode> It(this);
	while(++It)
		if(It->qwKey == _qwKey)
		{
			Delete(It);
			return;
		}
}

// ******************************************************************************** //
// Fast delete
void OrE::ADT::List::Delete(ADTElementP _pElement)
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

// ******************************************************************************** //
// Standard search with a key
ListNodeP OrE::ADT::List::Search(qword _qwKey)
{
	Iterator<ListNode> It(this);
	while(++It)
		if(It->qwKey == _qwKey) return &It;
	return nullptr;
}

// ******************************************************************************** //