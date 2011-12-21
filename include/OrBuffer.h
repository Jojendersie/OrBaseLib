// ******************************************************************************** //
// OrBaseLib.h																		//
// ===========																		//
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
//																					//
// ******************************************************************************** //
// Different ADTs for buffering objects.											//
//	Buffer: a ring buffer of constant size. The buffer can be used as FIFO			//
//	(first in first out) buffer or as a stack (LIFO buffer - last in first out).	//
//																					//
//	Both have the same speed asymptoticly:											//
//	Search()					O(n)												//
//	Push()						O(1)												//
//	Pop()						O(1)												//
//	Dequeue()					O(1)												//
//	Extend()					O(n)												//
//																					//
// Objects with the same key can be added more than one time. You also could set	//
// the key to zero everytimes.														//
//																					//
// Everything except GetFirst, GetLast, GetNext, GetPrevious are multithread save.	//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

// ******************************************************************************** //
// The ring buffer class
class Buffer
{
	int				m_iHeadCursor;			// Pointer to the next "free" element (Head-1==newest instance)
	int				m_iTailCursor;			// Pointer to the last element (oldest)
	int				m_iMax;					// Size of the underlying array
	volatile dword	m_dwSemaphore;			// Locking for multithreads
	void**			m_Buffer;				// The storage array for all elements

public:
	Buffer(int _iSize);
	virtual ~Buffer();

	bool Push(void* _pObject);				// Insertion at m_iHeadCursor with a key value of 0
	void* Pop();							// Stack operation: removes the instance at m_iHeadCursor from the buffer and returns the Object reference
	void* Dequeue();						// FIFO operation: removes the instance at m_iTailCursor and returns the object instance

	void Extend(int iAdditionalSize);		// Increase the size of the memory

	int	 GetNumInstance()	{return (m_iHeadCursor>=m_iTailCursor)?(m_iHeadCursor-m_iTailCursor):(m_iMax+m_iHeadCursor-m_iTailCursor);}	// Gibt die aktuelle Instanzenzahl an
	bool IsEmpty()			{return m_iHeadCursor==m_iTailCursor;}
	bool IsFull()			{return (m_iHeadCursor+1)%m_iMax == m_iTailCursor%m_iMax;}

	// Iteration over the objects
	void* GetFirst();						// Head element
	void* GetLast();						// Tail element
};

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //