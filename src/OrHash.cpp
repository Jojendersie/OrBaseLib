// ******************************************************************************** //
// OrHash.cpp																		//
// ==========																		//
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
#include "..\include\OrFastMath.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"

// Do not use in the file! This causes endless loops in debug garbage collection.
// #include "..\Include\OrDebug.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace OrE::Algorithm;
using namespace OrE::ADT;
using namespace OrE::Math;

// ******************************************************************************** //
// Create a 32 bit hash value of a data set
dword OrE::Algorithm::CreateHash32(const void* pData, const int iSize)
{
	qword qwHash = CreateHash64(pData, iSize);
	return ((dword)qwHash) ^ ((dword)(qwHash >> 32));
}

// ******************************************************************************** //
// Create a 64 bit hash value of a data set
qword OrE::Algorithm::CreateHash64(const void* pData, const int iSize)
{
	qword qwHash = 0x84222325cbf29ce4ULL;

	byte* pFirst = (byte*)(pData),
		* pLast  = pFirst + iSize;

	while( pFirst < pLast )
	{
		qwHash ^= *pFirst++;
		qwHash *= 0x00000100000001b3ULL;
	}

	return qwHash;
}

// ******************************************************************************** //
// Create a 32 bit hash value of a 0-terminated data set (e.g. strings)
dword OrE::Algorithm::CreateHash32(const void* pData)
{
	return ((dword)CreateHash64(pData)) ^ (((dword)CreateHash64(pData) >> 16) >> 16);
}

// ******************************************************************************** //
// Create a 64 bit hash value of a 0-terminated data set (e.g. strings)
qword OrE::Algorithm::CreateHash64(const void* pData)
{
	qword qwHash = 0x84222325cbf29ce4ULL;

	byte* pFirst = (byte*)(pData);

	while( *pFirst != 0 )	// Termination if byte is zero
	{
		qwHash ^= *pFirst++;
		qwHash *= 0x00000100000001b3ULL;
	}

	return qwHash;
}

// ******************************************************************************** //
// CRC - Error proving hash (cyclic redundancy check)
// Creates the hash with a arbitrary polynomial with a degree less than 32
dword OrE::Algorithm::CreateCRCHash(dword dwPolynom, void* pData, dword dwSize)
{
	dword dwCRC = 0;	// Shift register
	for(dword i=0; i<dwSize; ++i)
	{
		// For each bit
		for(dword j=7; j>=0; --j)
		{
			// Compare first bit of data with current check sum
			if((dwCRC>>31) != (dword)((((byte*)pData)[i]>>j) & 1))
				dwCRC = (dwCRC << 1) ^ dwPolynom;
			else
				dwCRC <<= 1;
		}
	}

	return dwCRC;
}


// ******************************************************************************** //
// Hash map																			//
// ******************************************************************************** //

// ******************************************************************************** //
// The hash function used for names ...
static dword OrStringHash(const char* _pcString)
{
	/*dword dwHash = 5381;

	while(int c = *_pcString++)
		dwHash = ((dwHash << 5) + dwHash) + c; // dwHash * 33 + c
		// Alternative dwHash * 33 ^ c

	return dwHash;//*/
	dword dwHash = 208357;

	while(int c = *_pcString++)
		dwHash = ((dwHash << 5) + (dwHash << 1) + dwHash) ^ c; 

	return dwHash;//*/
	/*dword dwHash = 208357;

	while(int c = *_pcString++)
		dwHash = (dwHash*17) ^ c + ~(dwHash >> 7) + (c >> 3);

	return dwHash;//*/
}

// ******************************************************************************** //
void OrE::ADT::HashMap::RecursiveReAdd(Bucket* _pBucket)
{
	// During resize it is necessary to re-add every thing into the new copy
	if(_pBucket->pObject) Insert(_pBucket->pObject, _pBucket->qwKey);	// If the key contains a char* pointer it is unique and not touched through reinsertion
	if(_pBucket->pLeft) RecursiveReAdd(_pBucket->pLeft);
	if(_pBucket->pRight) RecursiveReAdd(_pBucket->pRight);
}

// ******************************************************************************** //
// Initialization to given start size
OrE::ADT::HashMap::HashMap( dword _dwSize, HashMapMode _Mode ) :
	m_apBuckets( nullptr )
{
	// Resize allocates memory too
	Resize( _dwSize );
	m_Mode = _Mode;

#ifdef _DEBUG
	m_dwCollsionCounter = 0;
#endif
}

// ******************************************************************************** //
// Delete data from user and (owned) identifier if in string mode
void OrE::ADT::HashMap::RemoveData(BucketP _pBucket)
{
	if(m_pDeleteCallback && _pBucket->pObject) m_pDeleteCallback(_pBucket->pObject);
	if(m_Mode & HM_USE_STRING_MODE)
	{
		char* pcString = (char*)(_pBucket->qwKey>>32);
		if(pcString) free(pcString);
	}
}

// ******************************************************************************** //
// Release all resources
void OrE::ADT::HashMap::RecursiveRelease(BucketP _pBucket)
{
	// Delete user data
	RemoveData(_pBucket);
	// Recursion
	if(_pBucket->pLeft) RecursiveRelease(_pBucket->pLeft);
	if(_pBucket->pRight) RecursiveRelease(_pBucket->pRight);
	// Delete bucket/tree node itself. It is not part of the array.
	delete _pBucket;
}

// ******************************************************************************** //
OrE::ADT::HashMap::~HashMap()
{
	// Delete anything except the map
	Clear();

	// Delete table itself
	free( m_apBuckets );
}

// ******************************************************************************** //
// Remove everything
void OrE::ADT::HashMap::Clear()
{
	// Remove all binary trees and data.
	for(dword i=0;i<m_dwSize;++i)
	{
		// Precondition of RecursiveRelease: Bucket exists (not proven in method
		// for speed up)
		if( m_apBuckets[i] )
			RecursiveRelease(m_apBuckets[i]);
		m_apBuckets[i] = nullptr;
	}
	m_dwNumElements = 0;
#ifdef _DEBUG
	m_dwCollsionCounter = 0;
#endif
}

// ******************************************************************************** //
// Recreate the table and reinsert all elements
void OrE::ADT::HashMap::Resize(const dword _dwSize)
{
	BucketP* pOldList = m_apBuckets;
	dword dwOldSize = m_dwSize;
	// Allocate a new larger? table and make it empty
	m_apBuckets = (BucketP*)malloc(sizeof(BucketP)*_dwSize);
	if(!m_apBuckets) return;	// TODO: report error
	memset(m_apBuckets, 0, sizeof(BucketP)*_dwSize);
	// Set back properties to empty map
	m_dwSize = _dwSize;
	m_dwNumElements = 0;
	
	// This method is used for initialization. In that case the old list does not
	// exists and re insertion is useless.
	if( pOldList )
	{
		// Reinsert all elements
		for(dword i=0;i<dwOldSize;++i)
			if(pOldList[i]) RecursiveReAdd(pOldList[i]);

		// Delete old list
		free(pOldList);
	}
}

// ******************************************************************************** //
// Checks if a resize is required in the current mode
void OrE::ADT::HashMap::TestSize()
{
	// To improve performance extend
	switch( m_Mode & 3 )
	{
		// Slow growth
		case HM_PREFER_SIZE: 
			if(m_dwNumElements >= m_dwSize*3)
				Resize(m_dwSize+3*(dword)Sqrt((float)m_dwSize));
			break;
		// Medium growth
		case HM_RESIZE_MODERATE:
			if(m_dwNumElements >= (dword)(m_dwSize*1.5f))
				Resize(m_dwSize+Max(6*(dword)Sqrt((float)m_dwSize), (dword)128));
			break;
		// Fast growth, scarce resize
		case HM_PREFER_PERFORMANCE:
			if(m_dwNumElements >= m_dwSize)
				Resize((dword)((m_dwSize+100)*1.5f));
			break;
	}
}

// ******************************************************************************** //
// If _pBucket points to the initial list this will calculate the index, and -1 if not
int OrE::ADT::HashMap::ListIndex(BucketP _pBucket)
{
	int iIndex = int((BucketP*)_pBucket - m_apBuckets);
	if((iIndex >= (int)m_dwSize) || iIndex < 0) return -1;
	return iIndex;
}

// ******************************************************************************** //
// Standard operation insert
ADTElementP OrE::ADT::HashMap::Insert(void* _pObject, qword _qwKey)
{
#ifdef _DEBUG
	if(!m_apBuckets) return nullptr;		// TODO report error
#endif

	TestSize();

	if(m_Mode & HM_USE_STRING_MODE)
		_qwKey &= 0xffffffff;
	
	// Sort element into binary tree
	dword dwHash = _qwKey%m_dwSize;
	if(m_apBuckets[dwHash])
	{
		BucketP pBucket = m_apBuckets[dwHash];
		while(true)
		{
#ifdef _DEBUG
			++m_dwCollsionCounter;
#endif
			// compare key -> tree search
			if(_qwKey < pBucket->qwKey)
				if(pBucket->pLeft) pBucket = pBucket->pLeft;	// traverse
				else {pBucket->pLeft = new Bucket(_pObject, _qwKey, pBucket); ++m_dwNumElements; return pBucket->pLeft;}
			else if(_qwKey > pBucket->qwKey)
				if(pBucket->pRight) pBucket = pBucket->pRight;	// traverse
				else {pBucket->pRight = new Bucket(_pObject, _qwKey, pBucket); ++m_dwNumElements; return pBucket->pRight;}
			else {
#ifdef _DEBUG
				--m_dwCollsionCounter;
#endif
				pBucket->AddRef();
				return pBucket;
			}			// This data already exists. It is obvious that this element collides with itself.
		}
	} else
	{
		m_apBuckets[dwHash] = new Bucket(_pObject, _qwKey, BucketP(m_apBuckets+dwHash));
		++m_dwNumElements;
		return m_apBuckets[dwHash];
	}
}

// ******************************************************************************** //
// Standard operation delete
void OrE::ADT::HashMap::Delete(qword _qwKey)
{
	Delete(Search(_qwKey));
}

// ******************************************************************************** //
// Faster operation delete (no search)
void OrE::ADT::HashMap::Delete( ADTElementP _pElement )
{
	if(!_pElement) return;
	// Are there more references?
	if( _pElement->Release() > 0 ) return;

	// Repair tree
	// If _pElement has children search for a replacement of the _pElement node.
	if( (BucketP(_pElement))->pLeft )
	{
		// There is a left subtree - use maximum, it preserves tree properties
		BucketP pBuck = (BucketP(_pElement))->pLeft;
		while( pBuck->pRight ) pBuck = pBuck->pRight;
		// Attach right subtree at the left one (all elements larger than in the chosen node)
		pBuck->pRight = (BucketP(_pElement))->pRight;
		if( pBuck->pRight )
			pBuck->pRight->pParent = pBuck;
		// Remove reference from old node and set to left tree instead
		(BucketP(_pElement))->pLeft->pParent = (BucketP(_pElement))->pParent;
		// Update a parent or the real list entry.
		int iI;
		if( (iI = ListIndex((BucketP(_pElement))->pParent))==-1)
		{
			// Case: no list element
			if((BucketP(_pElement))->pParent->pLeft == BucketP(_pElement))
				(BucketP(_pElement))->pParent->pLeft = (BucketP(_pElement))->pLeft;
			else (BucketP(_pElement))->pParent->pRight = (BucketP(_pElement))->pLeft;
		} else m_apBuckets[iI] = (BucketP(_pElement))->pLeft;
	} else if( (BucketP(_pElement))->pRight )
	{
		// There is only a right subtree - use minimum, it preserves tree properties
		BucketP pBuck = (BucketP(_pElement))->pRight;
		while(pBuck->pLeft) pBuck = pBuck->pLeft;

		assert( !(BucketP(_pElement))->pLeft );

//		pBuck->pLeft = (BucketP(_pElement))->pLeft;
//		if(pBuck->pLeft)
//			pBuck->pLeft->pParent = pBuck;
	
		// Remove reference from old node and set to right tree instead
		(BucketP(_pElement))->pRight->pParent = (BucketP(_pElement))->pParent;
		// Update a parent or the real list entry.
		int iI;
		if( (iI = ListIndex((BucketP(_pElement))->pParent))==-1)
		{
			// Case: no list element
			if((BucketP(_pElement))->pParent->pLeft == BucketP(_pElement))
				(BucketP(_pElement))->pParent->pLeft = (BucketP(_pElement))->pRight;
			else (BucketP(_pElement))->pParent->pRight = (BucketP(_pElement))->pRight;
		} else m_apBuckets[iI] = (BucketP(_pElement))->pRight;
	} else
	{
		// No subtrees needs to be realigned.
		// Update a parent or the real list entry.
		int iI;
		if( (iI = ListIndex((BucketP(_pElement))->pParent)) == -1)
		{
			if((BucketP(_pElement))->pParent->pLeft == BucketP(_pElement))
				(BucketP(_pElement))->pParent->pLeft = nullptr;
			else (BucketP(_pElement))->pParent->pRight = nullptr;
		} else m_apBuckets[iI] = nullptr;
	}
	// Delete data
	RemoveData((BucketP)_pElement);
	delete (BucketP)_pElement;
	// Now it is removed
	--m_dwNumElements;
}

// ******************************************************************************** //
// Standard search with a key
ADTElementP OrE::ADT::HashMap::Search( qword _qwKey )
{
	// Find correct bucked with hashing
	dword dwHash = (m_Mode & HM_USE_STRING_MODE) ? (_qwKey&0xffffffff)%m_dwSize : _qwKey%m_dwSize;
	BucketP pBucket = m_apBuckets[dwHash];
	//if(pBucket) return nullptr;
	// Tree search
	while(pBucket)
	{
		if(_qwKey==pBucket->qwKey) return pBucket;
		else if(_qwKey<pBucket->qwKey) pBucket = pBucket->pLeft;
		else pBucket = pBucket->pRight;
	}
	return nullptr;
}

// ******************************************************************************** //
// TODO equality for data is other then in the upper date (last step) -> everything reimplement
// String-Mode functions
// insert using strings
ADTElementP OrE::ADT::HashMap::Insert( void* _pObject, const char* _pcKey )
{
	if(!(m_Mode & HM_USE_STRING_MODE)) return nullptr;
	#ifdef _DEBUG
	if(!m_apBuckets) return nullptr;		// TODO report error
#endif

	TestSize();
	
	// Insert new element now
	dword dwH = OrStringHash(_pcKey);
	dword dwHash = dwH%m_dwSize;
	if(m_apBuckets[dwHash])
	{
		BucketP pBucket = m_apBuckets[dwHash];
		bool bAdded = false;
		while(!bAdded)
		{
#ifdef _DEBUG
			++m_dwCollsionCounter;
#endif
			// Compare strings -> Tree search
			char* pcBucketString = (char*)(pBucket->qwKey>>32);
			// In string mode the upper 32 bit of the 64 Bit key represents a
			// pointer to the string. It could be that the map contains elements
			// without a string.
			// TODO: 64 Bit Portierung - fehler in pointerarithmetric
			if( !pcBucketString ) 
			{
				if(pBucket->pRight)
					pBucket = pBucket->pRight;
				else {pBucket->pRight = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pRight; bAdded = true;}
			} else
			{
				int iCmp = strcmp(_pcKey,pcBucketString);
				if(iCmp < 0)
					if(pBucket->pLeft) pBucket = pBucket->pLeft;	// traverse
					else {pBucket->pLeft = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pLeft; bAdded = true;}
				else if(iCmp > 0)
					if(pBucket->pRight) pBucket = pBucket->pRight;	// traverse
					else {pBucket->pRight = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pRight; bAdded = true;}
				else {
#ifdef _DEBUG
					--m_dwCollsionCounter;
#endif
					pBucket->AddRef();
					return pBucket;
				}			// This data already exists. It is obvious that this element collides with itself.
			}
		}
		// The loop terminates iff pBucket points to the new node.
		// Copy string.
		int iLen = (int)strlen(_pcKey)+1;
		void* pStr = malloc(iLen);
		memcpy(pStr, _pcKey, iLen);
		pBucket->qwKey = ((((qword)pStr)<<16)<<16) | dwH;
		++m_dwNumElements;
		return pBucket;
	} else
	{
		// Empty bucket
		m_apBuckets[dwHash] = new Bucket(_pObject, 0, BucketP(m_apBuckets+dwHash));
		// Copy the string
		int iLen = (int)strlen(_pcKey)+1;
		void* pStr = malloc(iLen);
		memcpy(pStr, _pcKey, iLen);
		m_apBuckets[dwHash]->qwKey = ((((qword)pStr)<<16)<<16) | dwH;
		++m_dwNumElements;
		return m_apBuckets[dwHash];
	}
}

// ******************************************************************************** //
// Standard operation delete for strings
void OrE::ADT::HashMap::Delete( const char* _pcKey )
{
	if(m_Mode & HM_USE_STRING_MODE)
		Delete( Search( _pcKey ) );
}

// ******************************************************************************** //
// search using strings
ADTElementP OrE::ADT::HashMap::Search( const char* _pcKey )
{
	if(!(m_Mode & HM_USE_STRING_MODE) || !_pcKey) return nullptr;
	// Find bucket with hashing
	dword dwHash = OrStringHash(_pcKey)%m_dwSize;
	BucketP pBucket = m_apBuckets[dwHash];
	// tree search with strings
	while(pBucket)
	{
		char* pcBucketString = (char*)((pBucket->qwKey>>16)>>16);
		if( pcBucketString )
		{
			int iCmp = strcmp(_pcKey,pcBucketString);
			if(iCmp==0) return pBucket;
			else if(iCmp<0) pBucket = pBucket->pLeft;
			else pBucket = pBucket->pRight;
		} else pBucket = pBucket->pRight;
	}
	return nullptr;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetFirst()
{
	for(dword i=0;i<m_dwSize;++i)
		// Skip all empty buckets.
		if(m_apBuckets[i])
		{
			BucketP pBuck = m_apBuckets[i];
			// Goto the smallest element in the bucket
			while(pBuck->pLeft) pBuck = pBuck->pLeft;
			return pBuck;
		}

	return nullptr;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetLast()
{
	for(dword i=m_dwSize-1;i>=0;--i)
		// Skip all empty buckets.
		if(m_apBuckets[i])
		{
			BucketP pBuck = m_apBuckets[i];
			// Goto the largest element in the bucket
			while(pBuck->pRight) pBuck = pBuck->pRight;
			return pBuck;
		}

	return nullptr;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetNext(ADTElementP _pCurrent)
{
	BucketP pBuck = (BucketP)_pCurrent;
	// Inorder traverse -> left site was seen before
	if(pBuck->pRight) 
	{
		pBuck = pBuck->pRight;
		while(pBuck->pLeft) pBuck = pBuck->pLeft;
		return pBuck;
	} else {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		int iIndex = ListIndex(pBuck->pParent);
		while(pBuck->pParent->pRight == pBuck && iIndex==-1)
		{
			pBuck = pBuck->pParent;
			iIndex = ListIndex(pBuck->pParent);
		}
		if(iIndex!=-1)
		{
			++iIndex;
			if(iIndex == (int)m_dwSize) return nullptr;			// Reached the end - no next element
			while(!m_apBuckets[iIndex])
				if(++iIndex == (int)m_dwSize) return nullptr;	// Reached the end - no next element
			// Smallest element in bucket
			pBuck = m_apBuckets[iIndex];
			while(pBuck->pLeft) pBuck = pBuck->pLeft;
			return pBuck;
		} else
			return pBuck->pParent;
	}
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetPrevious(ADTElementP _pCurrent)
{
	BucketP pBuck = (BucketP)_pCurrent;
	// Inorder traverse -> left site was seen before
	if(pBuck->pLeft) 
	{
		pBuck = pBuck->pLeft;
		while(pBuck->pRight) pBuck = pBuck->pRight;
		return pBuck;
	} else {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		int iIndex = ListIndex(pBuck->pParent);
		while(pBuck->pParent->pLeft == pBuck && iIndex==-1) 
		{
			pBuck = pBuck->pParent;
			iIndex = ListIndex(pBuck->pParent);
		}
		if(iIndex != -1)
		{
			--iIndex;
			if(iIndex == -1) return nullptr;		// Reached the end - no next element
			while(!m_apBuckets[iIndex])
				if(--iIndex == -1) return nullptr;	// Reached the end - no next element
			// Largest element in bucket
			pBuck = m_apBuckets[iIndex];
			while(pBuck->pRight) pBuck = pBuck->pRight;
			return pBuck;
		} else
			return pBuck->pParent;
	}
}

// *************************************EOF**************************************** //