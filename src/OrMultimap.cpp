// ******************************************************************************** //
// OrMultiMap.cpp																	//
// ==============																	//
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

#include <mutex>

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"
#include "..\include\OrMultimap.h"
#include "..\include\OrDebug.h"

// ******************************************************************************** //
// MULTIMAP::GROUP
// ******************************************************************************** //
void OrE::ADT::MultiMap::DeleteGroup( void* _pGroup )	{ delete (OrE::ADT::MultiMap::Group*)( _pGroup ); }


// ******************************************************************************** //
// Thread safe insert at front O(1)
void OrE::ADT::MultiMap::Group::Insert( ADTElementP _pObject )
{
	std::lock_guard<std::mutex> Guard( m_Lock );

	m_iNumEntries++;
	m_pFirst = new GroupEntry( m_pFirst, _pObject );
}

// ******************************************************************************** //
/*void OrE::ADT::MultiMap::Group::Delete( GroupEntry* _pPredecessor, GroupEntry* _pObject )
{
	if( _pPredecessor ) _pPredecessor->pNext = _pObject->pNext;
	else m_pFirst = _pObject->pNext;
	delete _pObject;

	--m_iNumEntries;
	Assert( m_iNumEntries>=0 );
}*/

// ******************************************************************************** //
// Helper to check, if a string is nullptr of ""
inline bool IsDefaultGroup( const char* _pcGroup )	{ return (_pcGroup==nullptr) || (_pcGroup[0]==0); }

// ******************************************************************************** //
// MULTIMAP PART
// ******************************************************************************** //
// Constructor creates an empty map of a chosen size.
OrE::ADT::MultiMap::MultiMap( int _iSize ) :
	m_GroupMap( 16, HashMapMode(HashMapMode::HM_NO_RESIZE | HashMapMode::HM_USE_STRING_MODE) ),
	m_ObjectMap( _iSize, HashMapMode::HM_PREFER_PERFORMANCE ),
	m_DefaultGroup( 0 ),
	m_iNumElements( 0 )
{
	// Deletion of objects is manual
	m_GroupMap.SetDeleteCallback( DeleteGroup );

	// Create default group
	ADTElementP pEl = m_GroupMap.Insert( &m_DefaultGroup, "" );
	Assert( pEl );
}

// ******************************************************************************** //
// Destruction deletes everything, but not the objects.
OrE::ADT::MultiMap::~MultiMap()
{
	m_GroupMap.Clear();
	m_ObjectMap.Clear();
}

// ******************************************************************************** //
// Deferred object deletion. This decrements the reference counter
// of the object and deletes it if necessary.
void OrE::ADT::MultiMap::UnrefObj( ADTElementP _pObject )
{
	// Last byte == reference counter
	int iRef = 0xff & (int)_pObject->pObject;
	if( --iRef <= 0 )
	{
		Assert( ((dword)_pObject->pObject)==1 );
		std::lock_guard<std::mutex> Guard( m_Lock );
		// Remove object from map.
		m_ObjectMap.Delete( _pObject );
	} else
		// The counter is > 0 -> -1 changes the counter in the least significant bits
		// but not the flag mask.
		_pObject->pObject = (byte*)_pObject->pObject - 1;
}

// ******************************************************************************** //
// The method to add new objects.
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup = nullptr;
	bool bNew = true;
	ADTElementP pOE;

	// Since the hash maps are not thread safe lock during insertion
	{ std::lock_guard<std::mutex> Guard( m_Lock );
		// Object address = hash, data = flags for all groups (setup later)
		pOE = m_ObjectMap.Insert( (void*)0, (qword)_pNewObject );
		Assert( pOE );
		if( pOE->GetRef() == 1 ) ++m_iNumElements;
		else { pOE->Release(); bNew = false; }		// Reference counter should be as high as the number of referencing groups -> double insertion occured -> ref is 1 too large

		// Register in group
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup0 );
		Assert( pEl );
		pGroup = (Group*)pEl->pObject;
	}

	// Insert into the group
	Assert( pGroup );
	// For each NEW group increase reference counter
	if( !( (uiptr)pOE->pObject & (1<<pGroup->m_ID) ) )
	{
		pOE->AddRef();
		pGroup->Insert( pOE );
	}
	if( bNew ) m_DefaultGroup.Insert( pOE );
	// Reset flag word in the object
	pOE->pObject = (void*)((uiptr)pOE->pObject | (1<<pGroup->m_ID) | 1 );
}

// ******************************************************************************** //
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup0 = nullptr;
	Group* pGroup1 = nullptr;
	bool bNew = true;
	ADTElementP pOE;

	// Since the hash maps are not thread safe lock during insertion
	{ std::lock_guard<std::mutex> Guard( m_Lock );
		// Object address = hash, data = flags for all groups (setup later)
		pOE = m_ObjectMap.Insert( (void*)0, (qword)_pNewObject );
		Assert( pOE );
		if( pOE->GetRef() == 1 ) ++m_iNumElements;
		else { pOE->Release(); bNew = false; }		// Reference counter should be as high as the number of referencing groups -> double insertion occured -> ref is 1 too large

		// Register in groups
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup0 );
		Assert( pEl );
		pGroup0 = (Group*)pEl->pObject;

		pEl = m_GroupMap.Search( _pcGroup1 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup1 );
		Assert( pEl );
		pGroup1 = (Group*)pEl->pObject;
	}

	// Insert into the groups
	Assert( pGroup0 );
	Assert( pGroup1 );
	// For each NEW group increase reference counter
	if( !( (uiptr)pOE->pObject & (1<<pGroup0->m_ID) ) )
	{
		pOE->AddRef();
		pGroup0->Insert( pOE );
	}
	if( !( (uiptr)pOE->pObject & (1<<pGroup1->m_ID) ) )
	{
		pOE->AddRef();
		pGroup1->Insert( pOE );
	}
	if( bNew ) m_DefaultGroup.Insert( pOE );
	// Reset flag word in the object
	pOE->pObject = (void*)((uiptr)pOE->pObject | (1<<pGroup0->m_ID) | (1<<pGroup1->m_ID) | 1 );
}

// ******************************************************************************** //
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1, const char* _pcGroup2 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup0 = nullptr;
	Group* pGroup1 = nullptr;
	Group* pGroup2 = nullptr;
	bool bNew = true;
	ADTElementP pOE;

	// Since the hash maps are not thread safe lock during insertion
	{ std::lock_guard<std::mutex> Guard( m_Lock );
		// Object address = hash, data = flags for all groups (setup later)
		pOE = m_ObjectMap.Insert( (void*)0, (qword)_pNewObject );
		Assert( pOE );
		if( pOE->GetRef() == 1 ) ++m_iNumElements;
		else { pOE->Release(); bNew = false; }		// Reference counter should be as high as the number of referencing groups -> double insertion occured -> ref is 1 too large

		// Register in groups
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup0 );
		Assert( pEl );
		pGroup0 = (Group*)pEl->pObject;

		pEl = m_GroupMap.Search( _pcGroup1 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup1 );
		Assert( pEl );
		pGroup1 = (Group*)pEl->pObject;

		pEl = m_GroupMap.Search( _pcGroup2 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup2 );
		Assert( pEl );
		pGroup2 = (Group*)pEl->pObject;
	}

	// Insert into the groups
	Assert( pGroup0 );
	Assert( pGroup1 );
	Assert( pGroup2 );
	// For each NEW group increase reference counter
	if( !( (uiptr)pOE->pObject & (1<<pGroup0->m_ID) ) )
	{
		pOE->AddRef();
		pGroup0->Insert( pOE );
	}
	if( !( (uiptr)pOE->pObject & (1<<pGroup1->m_ID) ) )
	{
		pOE->AddRef();
		pGroup1->Insert( pOE );
	}
	if( !( (uiptr)pOE->pObject & (1<<pGroup2->m_ID) ) )
	{
		pOE->AddRef();
		pGroup2->Insert( pOE );
	}
	m_DefaultGroup.Insert( pOE );
	// Reset flag word in the object
	pOE->pObject = (void*)((uiptr)pOE->pObject | (1<<pGroup0->m_ID) | (1<<pGroup1->m_ID) | (1<<pGroup2->m_ID) | 1 );
}

// ******************************************************************************** //
// Remove the object from all groups and from the map O(1). (Deferred)
void OrE::ADT::MultiMap::Remove( void* _pObject )
{
	// Test if object is in map and stop if not
	{ std::lock_guard<std::mutex> Guard( m_Lock );
		ADTElementP pOE;
		if( !(pOE=m_ObjectMap.Search( (qword)_pObject )) ) return;

		// Delete all flags == remove from all groups
		pOE->pObject = 0;
		--m_iNumElements;
	}
}

// ******************************************************************************** //
// Maps an object to an existing or new group.
void OrE::ADT::MultiMap::Map( void* _pObject, const char* _pcGroup )
{
	if( IsDefaultGroup( _pcGroup ) ) return;

	Group* pGroup = nullptr;
	ADTElementP pOE;

	// Test if object is in map and stop if not
	{ std::lock_guard<std::mutex> Guard( m_Lock );
		// Object is not in map
		if( !(pOE=m_ObjectMap.Search( (qword)_pObject )) ) return;

		ADTElementP pEl = m_GroupMap.Search( _pcGroup );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements() ), _pcGroup );
		pGroup = (Group*)pEl->pObject;
	}

	// The object is in map -> apply to new group
	Assert( pGroup );
	pGroup->Insert( pOE );
	// Reset flag word in the object
	pOE->pObject = (void*)((uiptr)pOE->pObject | (1<<pGroup->m_ID) );
}

// ******************************************************************************** //
// Removes the object from a group.
void OrE::ADT::MultiMap::Unmap( void* _pObject, const char* _pcGroup )
{
	if( IsDefaultGroup( _pcGroup ) ) return;

	{ std::lock_guard<std::mutex> Guard( m_Lock );

		// Search group and element. The group has to be found to get its ID, the
		// Object to unmap it.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup );
		if( !pEl ) return;

		ADTElementP pOE = m_ObjectMap.Search( (qword)_pObject );
		if( !pOE ) return;

		// Delete flag from Object
		uiptr uFlags = (uiptr)pOE->pObject & ~(1<<((Group*)pEl->pObject)->m_ID);
		// If the current group was the last one, remove from default group too
		if( uFlags == 1 )
		{
			pOE->pObject = (void*)( 0 );
			--m_iNumElements;
		}
	}

	// The real unmapping is deferred. It is only done, iff the group iterator passes the
	// element.
}

// ******************************************************************************** //
// Tests if an object is in a group.
bool OrE::ADT::MultiMap::IsIn( void* _pObject, const char* _pcGroup )
{
	std::lock_guard<std::mutex> Guard( m_Lock );

	// Does object exists?
	ADTElementP pOE = m_ObjectMap.Search( (qword)_pObject );
	// The object could be found if deletion not complete, but requested.
	// But then the flag word is already 0.
	if( !pOE || ( (uiptr)pOE->pObject <= 1 ) ) return false;
	// IsIn map general case.
	if( IsDefaultGroup( _pcGroup ) ) return true;

	// Search group to get its ID.
	ADTElementP pEl = m_GroupMap.Search( _pcGroup );
	if( !pEl ) return false;

	// Is the group flag set?
	uiptr uMask = 1<<((Group*)pEl->pObject)->m_ID;
	return ((uiptr)pOE->pObject & uMask) == uMask;
}



// ******************************************************************************** //
// Override ++ to navigate
OrE::ADT::MultiMap::Iterator& OrE::ADT::MultiMap::Iterator::operator++()
{
	// ++ sets m_pCurrentElement to the next valid element. If the next element is
	// not valid it is deleted and the search is repeated. Therefore pNext is temporary
	// and if 0 it denotes the end of the search.
	GroupEntry* pNext = nullptr;
	do
	{
		{ std::lock_guard<std::mutex> Guard( m_pGroup->m_Lock );
		SearchNext:
			pNext = m_pCurrentElement ? m_pCurrentElement->pNext : m_pGroup->m_pFirst;
			// Check if the next element has to be removed from the list (deferred deletion)
			if( !pNext || ((1<<m_pGroup->m_ID) & (uiptr)pNext->pObjectEntry->pObject) )
			{
				if( m_pCurrentElement ) --m_pCurrentElement->iRef;
				m_pCurrentElement = pNext;
				if( m_pCurrentElement ) ++m_pCurrentElement->iRef;
				// Leave outer loop
				pNext = nullptr;
			} else
			{
				// Problem: another iterator could point to that entry before
				// this one reaches the locked area. -> Look for iRef and jump
				// deletion - it is done later by an other iterator.
				if( pNext->iRef )
				{
					if( m_pCurrentElement ) --m_pCurrentElement->iRef;
					m_pCurrentElement = pNext;
					++m_pCurrentElement->iRef;
					goto SearchNext;
				}
				// Delete now.
				if( m_pCurrentElement ) m_pCurrentElement->pNext = pNext->pNext;
				else m_pGroup->m_pFirst = pNext->pNext;
				// Remove one reference (they were increased for each group)
				m_pObjectMap->Delete( pNext->pObjectEntry );
				--m_pGroup->m_iNumEntries;
				Assert( m_pGroup->m_iNumEntries >= 0 );
			}
		}

		// During deletion no lock is necessary, because all links to that entry are
		// removed. If no deletion is necessary the pNext was set to nullptr.
		if( pNext ) delete pNext;
	} while( pNext );

	if( m_pCurrentElement ) Assert( m_pCurrentElement->iRef>0 );

	return *this;
}

// ******************************************************************************** //
// Create an iterator for an group. The group 0/nullptr/"" is the default group of all
// Objects.
OrE::ADT::MultiMap::Iterator OrE::ADT::MultiMap::GetIterator( const char* _pcGroup )
{
	// nullptr -> default group
	if( IsDefaultGroup( _pcGroup ) ) return Iterator( &m_ObjectMap, &m_DefaultGroup );

	{ // Lock for group search
		std::lock_guard<std::mutex> Guard( m_Lock );

		ADTElementP pEl = m_GroupMap.Search( _pcGroup );

		// Your group does not exists!
		// Don't try to get Iterators for non existing groups, because this cannot be
		// handled by the iterator (would slow down iterators, if implemented).
		Assert( pEl );

		return Iterator( &m_ObjectMap, (Group*)pEl->pObject );
	}
}
