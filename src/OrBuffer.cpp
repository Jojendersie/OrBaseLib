// ******************************************************************************** //
// OrBuffer.cpp																		//
// ============																		//
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
#include "..\include\OrBuffer.h"

#include <stdlib.h>
// #include <mutex.h> TODO insert if vc11 released

// TODO test with threads
// ******************************************************************************** //
OrE::ADT::Buffer::Buffer(int _iSize)
{
	m_Buffer = (void**)malloc(sizeof(void*)*_iSize);
	// Make buffer unusable if out of memory (IsFull() == IsEmpty() == false);
	m_iMax = m_Buffer ? _iSize: 1;
	m_iHeadCursor = 0;
	m_iTailCursor = 0;
}

OrE::ADT::Buffer::~Buffer()
{
	// Delete without warning even if there are still elements in buffer
	// TODO warn (Log system)
	free(m_Buffer);
}

// ******************************************************************************** //
// Insertion at m_iHeadCursor with a key value of 0
bool OrE::ADT::Buffer::Push(void* _pObject)
{
	bool bRet = false;
	//OrLock(&m_dwSemaphore);
	if(!IsFull())
	{
		m_Buffer[m_iHeadCursor] = _pObject;
		m_iHeadCursor = (m_iHeadCursor+1)%m_iMax;
		bRet = true;
	}
	//OrUnlock(&m_dwSemaphore);
	return bRet;
}

// ******************************************************************************** //
// Stack operation: removes the instance at m_iHeadCursor from the buffer and returns the Object reference
void* OrE::ADT::Buffer::Pop()
{
	void* pObj = nullptr;
	//OrLock(&m_dwSemaphore);
	if(!IsEmpty())
	{
		m_iHeadCursor = (m_iHeadCursor+m_iMax-1)%m_iMax;
		pObj = m_Buffer[m_iHeadCursor];
	}
	//OrUnlock(&m_dwSemaphore);
	return pObj;
}

// ******************************************************************************** //
// Removes all data
void OrE::ADT::Buffer::Clear()
{
	m_iHeadCursor = 0;
	m_iTailCursor = 0;
}

// ******************************************************************************** //
// FIFO operation: removes the instance at m_iTailCursor and returns the object instance
void* OrE::ADT::Buffer::Dequeue()
{
	void* pObj = nullptr;
//	OrLock(&m_dwSemaphore);
	if(!IsEmpty())
	{
		pObj = m_Buffer[m_iTailCursor];
		m_iTailCursor = (m_iTailCursor+1)%m_iMax;
	}
	//OrUnlock(&m_dwSemaphore);
	return pObj;
}

// ******************************************************************************** //
// Enlarge memory
void OrE::ADT::Buffer::Extend(int iAdditionalSize)
{
	//OrLock(&m_dwSemaphore);

	m_iMax += iAdditionalSize;
	m_Buffer = (void**)realloc(m_Buffer, sizeof(void*)*m_iMax);

	//OrUnlock(&m_dwSemaphore);
}

// ******************************************************************************** //
// Iteration over the objects
// Head element
void* OrE::ADT::Buffer::GetFirst()
{
	return IsEmpty() ? nullptr : m_Buffer[(m_iHeadCursor+m_iMax-1)%m_iMax];
}

// ******************************************************************************** //
// Tail element
void* OrE::ADT::Buffer::GetLast()
{
	return IsEmpty() ? nullptr : m_Buffer[m_iTailCursor];
}

// *************************************EOF**************************************** //