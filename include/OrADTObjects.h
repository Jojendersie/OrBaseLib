#pragma once

// ******************************************************************************** //
// Since Objects are created by the user they have to be destroyed the same way (use callback)
typedef void (*OrADTDeleteObjectCallbackP)(void* _pObject);

// ******************************************************************************** //
// Interface for all nodes,... of ADTs
struct OrADTElement
{
private:
	int iRef;					// Reference counter: how often was this element insertet?
public:
	qword qwKey;				// 64 Key value
	void* pObject;				// Stored data
	OrADTElement(void* _pObj, const qword& _qwKey):pObject(_pObj), iRef(1), qwKey(_qwKey) {}

	void AddRef()	{++iRef;}
	int Release()	{return --iRef;}
};
typedef OrADTElement* OrADTElementP;

// ******************************************************************************** //
// Interface class for all ADTs of the OrBaseLib
class OrADT
{
public:
	virtual OrADTElementP Insert(void* _pObject, qword _qwKey) = 0;	// Standard operation insert
	virtual void Delete(qword _qwKey) = 0;							// Standard operation delete
	virtual void Delete(OrADTElementP _pElement) = 0;				// Sometimes faster operation delete (no search)
	virtual OrADTElementP Search(qword _qwKey) = 0;					// Standard search with a key

	virtual OrADTElementP GetFirst() = 0;
	virtual OrADTElementP GetLast() = 0;
	virtual OrADTElementP GetNext(OrADTElementP _pCurrent) = 0;
	virtual OrADTElementP GetPrevious(OrADTElementP _pCurrent) = 0;
};
typedef OrADT* OrADTP;

// ******************************************************************************** //
template <typename Type> class OrIterator
{
private:
	OrADTP			m_pADT;
	OrADTElementP	m_pCurrentElement;
	friend class OrIterator;
public:
	// Creates an Iterator for an arbitary ADT Object and set it to begin
	OrIterator(OrADTP _pADT) {
		m_pADT = _pADT;
		m_pCurrentElement = nullptr;
	}

	void SeekToEnd()	{m_pCurrentElement = m_pADT->GetLast();}
	void SeekToBegin()	{m_pCurrentElement = m_pADT->GetFirst();}

	// Override boolean operator that if(OrIterator) can be asked
	operator bool () const {return m_pCurrentElement != 0;}

	// Casting operators
	bool operator==(const OrIterator& T) const {return m_pCurrentElement == T.m_pCurrentElement;}
	bool operator!=(const OrIterator& T) const {return m_pCurrentElement != T.m_pCurrentElement;}
	Type& operator*() const {return *(Type*)m_pCurrentElement;}
	Type* operator->() const {return (Type*)m_pCurrentElement;}
	Type* operator&() const {return (Type*)m_pCurrentElement;}

	// Override ++ and -- to navigate
	OrIterator& operator++() {m_pCurrentElement = m_pCurrentElement ? m_pADT->GetNext(m_pCurrentElement) : (m_pADT->GetFirst()); return *this;}
	OrIterator& operator--() {m_pCurrentElement = m_pCurrentElement ? m_pADT->GetPrevious(m_pCurrentElement) : (m_pADT->GetLast()); return *this;}
	// Postfix
	const OrIterator operator++(int) { OrIterator temp = *this; ++*this; return temp; }
	const OrIterator operator--(int) { OrIterator temp = *this; --*this; return temp; }
};

// *************************************EOF**************************************** //