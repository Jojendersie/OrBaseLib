#pragma once

namespace OrE {
namespace ADT {

// ******************************************************************************** //
// Since Objects are created by the user they have to be destroyed the same way (use callback)
typedef void (*ADTDeleteObjectCallbackP)(void* _pObject);

// ******************************************************************************** //
// Interface for all nodes,... of ADTs
// The ADTs will not make copies of the data. It will just hold the pointer and eventually
// destroy the resource.
struct ADTElement
{
private:
	int iRef;					// Reference counter: how often was this element insertet?
public:
	qword qwKey;				// 64 Key value
	void* pObject;				// Stored data
	ADTElement(void* _pObj, const qword& _qwKey):pObject(_pObj), iRef(1), qwKey(_qwKey) {}

	void AddRef()	{++iRef;}
	int Release()	{return --iRef;}
};
typedef ADTElement* ADTElementP;

// ******************************************************************************** //
// Interface class for all ADTs of the OrBaseLib
class ADT
{
protected:
	ADTDeleteObjectCallbackP m_pDeleteCallback;
public:
	ADT():m_pDeleteCallback(nullptr)	{}
	virtual ADTElementP Insert(void* _pObject, qword _qwKey) = 0;	// Standard operation insert
	virtual void Delete(qword _qwKey) = 0;							// Standard operation delete
	virtual void Delete(ADTElementP _pElement) = 0;				// Sometimes faster operation delete (no search)
	virtual ADTElementP Search(qword _qwKey) = 0;					// Standard search with a key

	virtual ADTElementP GetFirst() = 0;
	virtual ADTElementP GetLast() = 0;
	virtual ADTElementP GetNext(ADTElementP _pCurrent) = 0;
	virtual ADTElementP GetPrevious(ADTElementP _pCurrent) = 0;

	void SetDeleteCallback(ADTDeleteObjectCallbackP _pDeleteCallback)	{m_pDeleteCallback = _pDeleteCallback;}
};
typedef ADT* ADTP;

// ******************************************************************************** //
// Type should be the ADTElement type or a derivate from this.
// Usage example:
//		Iterator<BinaryTreeNode> It(&MyAVLTree);
//		while(--It) oder while(++It)
//			std::cout << It->qwKey << "\n";
template <typename Type> class Iterator
{
private:
	ADTP			m_pADT;
	ADTElementP	m_pCurrentElement;
	friend class Iterator;
public:
	// Creates an Iterator for an arbitary ADT Object and set it to begin
	Iterator(ADTP _pADT) {
		m_pADT = _pADT;
		m_pCurrentElement = nullptr;
	}

	void SeekToEnd()	{m_pCurrentElement = m_pADT->GetLast();}
	void SeekToBegin()	{m_pCurrentElement = m_pADT->GetFirst();}

	// Override boolean operator that if(Iterator) can be asked
	operator bool () const {return m_pCurrentElement != 0;}

	// Casting operators
	bool operator==(const Iterator& T) const {return m_pCurrentElement == T.m_pCurrentElement;}
	bool operator!=(const Iterator& T) const {return m_pCurrentElement != T.m_pCurrentElement;}
	Type& operator*() const {return *(Type*)m_pCurrentElement;}
	Type* operator->() const {return (Type*)m_pCurrentElement;}
	Type* operator&() const {return (Type*)m_pCurrentElement;}

	// Override ++ and -- to navigate
	Iterator& operator++() {m_pCurrentElement = m_pCurrentElement ? m_pADT->GetNext(m_pCurrentElement) : (m_pADT->GetFirst()); return *this;}
	Iterator& operator--() {m_pCurrentElement = m_pCurrentElement ? m_pADT->GetPrevious(m_pCurrentElement) : (m_pADT->GetLast()); return *this;}
	// Postfix
	const Iterator operator++(int) { Iterator temp = *this; ++*this; return temp; }
	const Iterator operator--(int) { Iterator temp = *this; --*this; return temp; }
};

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //