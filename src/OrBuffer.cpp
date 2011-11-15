#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBuffer.h"

#include <stdlib.h>

// TODO test with threads
// ******************************************************************************** //
OrBuffer::OrBuffer(int _iSize)
{
	m_Buffer = (OrADTElementP)malloc(sizeof(OrADTElement)*_iSize);
	// Make buffer unuseable if out of memory (IsFull() == IsEmpty() == false);
	m_iMax = m_Buffer ? _iSize: 1;
	m_iHeadCursor = 0;
	m_iTailCursor = 0;
}

OrBuffer::~OrBuffer()
{
	// Delete without warning even if there are still elements in buffer
	// TODO warn (Logsystem)
	free(m_Buffer);
}

// ******************************************************************************** //
/*OrADTElementP OrBuffer::Insert(void* _pObject, qword _qwKey)
{
	OrADTElementP pRet = nullptr;
	OrLock(&m_dwSemaphore);
	if(!IsFull())
	{
		m_Buffer[m_iHeadCursor].pObject = _pObject;
		m_Buffer[m_iHeadCursor].qwKey = _qwKey;
		pRet = &m_Buffer[m_iHeadCursor];
		m_iHeadCursor = (m_iHeadCursor+1)%m_iMax;
	}
	OrUnlock(&m_dwSemaphore);
	return pRet;
}

// ******************************************************************************** //
OrADTElementP OrBuffer::Search(qword _qwKey)
{
	OrLock(&m_dwSemaphore);
	for(int i=m_iTailCursor; m_iTailCursor!=m_iHeadCursor; m_iTailCursor = (m_iTailCursor+1)%m_iMax)
	{
		if(m_Buffer[i].qwKey == _qwKey)
		{
			OrUnlock(&m_dwSemaphore);
			return &m_Buffer[i];
		}
	}
	OrUnlock(&m_dwSemaphore);
	return nullptr;
}
*/
// ******************************************************************************** //
// Insertion at m_iHeadCursor with a key value of 0
bool OrBuffer::Push(void* _pObject)
{
	bool bRet = false;
	OrLock(&m_dwSemaphore);
	if(!IsFull())
	{
		m_Buffer[m_iHeadCursor].pObject = _pObject;
		m_Buffer[m_iHeadCursor++].qwKey = 0;
		m_iHeadCursor = (m_iHeadCursor+1)%m_iMax;
		bRet = true;
	}
	OrUnlock(&m_dwSemaphore);
	return bRet;
}

// ******************************************************************************** //
// Stack operation: removes the instance at m_iHeadCursor from the buffer and returns the Object reference
void* OrBuffer::Pop()
{
	void* pObj = nullptr;
	OrLock(&m_dwSemaphore);
	if(!IsEmpty())
	{
		m_iHeadCursor = (m_iHeadCursor+m_iMax-1)%m_iMax;
		pObj = m_Buffer[m_iHeadCursor].pObject;
	}
	OrUnlock(&m_dwSemaphore);
	return pObj;
}

// ******************************************************************************** //
// FIFO operation: removes the instance at m_iTailCursor and returns the object instance
void* OrBuffer::Dequeue()
{
	void* pObj = nullptr;
	OrLock(&m_dwSemaphore);
	if(!IsEmpty())
	{
		pObj = m_Buffer[m_iTailCursor].pObject;
		m_iTailCursor = (m_iTailCursor+1)%m_iMax;
	}
	OrUnlock(&m_dwSemaphore);
	return pObj;
}

// ******************************************************************************** //
// Speicher vergrößern
void OrBuffer::Extend(int iAdditionalSize)
{
	OrLock(&m_dwSemaphore);

	m_iMax += iAdditionalSize;
	m_Buffer = (OrADTElementP)realloc(m_Buffer, sizeof(OrADTElement)*m_iMax);

	OrUnlock(&m_dwSemaphore);
}

// ******************************************************************************** //
// Iteration over the objects
// Head element
OrADTElementP OrBuffer::GetFirst()
{
	return IsEmpty() ? nullptr : &m_Buffer[(m_iHeadCursor+m_iMax-1)%m_iMax];
}

// ******************************************************************************** //
// Tail element
OrADTElementP OrBuffer::GetLast()
{
	return IsEmpty() ? nullptr : &m_Buffer[m_iTailCursor];
}

// ******************************************************************************** //
// Moving to the tail
OrADTElementP OrBuffer::GetNext(OrADTElementP _pCurrent)
{
	// TODO test if index possible
	int iIndex = _pCurrent-m_Buffer;
	if(iIndex == m_iTailCursor) return nullptr;
	else return &m_Buffer[(iIndex+m_iMax-1)%m_iMax];
}

// ******************************************************************************** //
// Moving to the head
OrADTElementP OrBuffer::GetPrevious(OrADTElementP _pCurrent)
{
	int iIndex = _pCurrent-m_Buffer;
	iIndex = (iIndex+1)%m_iMax;
	if(iIndex == m_iHeadCursor) return nullptr;
	else return &m_Buffer[iIndex];
}

// *************************************EOF**************************************** //