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
#include <thread>

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"
#include "..\include\OrMultimap.h"
#include "..\include\OrDebug.h"


#define LOCK std::lock_guard<std::mutex> Guard( m_Lock );

// ******************************************************************************** //
// MULTIMAP::GROUP
// ******************************************************************************** //
void OrE::ADT::MultiMap::DeleteGroup( void* _pGroup )	{ delete (OrE::ADT::MultiMap::Group*)( _pGroup ); }
void OrE::ADT::MultiMap::DeleteObject( void* _pObject )	{ delete (OrE::ADT::MultiMap::GroupIndex*)( _pObject ); }


// ******************************************************************************** //
// Thread safe insert at front O(1)
OrE::ADT::MultiMap::GroupEntry* OrE::ADT::MultiMap::Group::Insert( ADTElementP _pObject )
{
	LOCK

	m_iNumEntries++;
	m_pFirst = new GroupEntry( m_pFirst, _pObject );
	if( m_pFirst->pNext ) m_pFirst->pNext->pPrev = m_pFirst;

	return m_pFirst;
}

// ******************************************************************************** //
void OrE::ADT::MultiMap::Group::Delete( GroupEntry* _pObject )
{
	// Stop if element locked. Do not lock the list until object can be accessed otherwise
	// the referencing iterator will have a deadlock.
Begin:
	while( _pObject->iRef ) std::this_thread::yield();// sleep_for( std::chrono::microseconds( 0 ) );//yield();

	{ LOCK
		// Test if object is really unreferenced - otherwise wait again
		if( _pObject->iRef )
			goto Begin;

		// Remove from double linked list
		if( _pObject->pPrev ) _pObject->pPrev->pNext = _pObject->pNext;
		else m_pFirst = _pObject->pNext;
		if( _pObject->pNext ) _pObject->pNext->pPrev = _pObject->pPrev;

		delete _pObject;

		--m_iNumEntries;
		Assert( m_iNumEntries>=0 );
	}
}

// ******************************************************************************** //
// Helper to check, if a string is nullptr of ""
inline bool IsDefaultGroup( const char* _pcGroup )	{ return (_pcGroup==nullptr) || (_pcGroup[0]==0); }

// ******************************************************************************** //
// MULTIMAP PART
// ******************************************************************************** //
// Constructor creates an empty map of a chosen size.
OrE::ADT::MultiMap::MultiMap( int _iSize ) :
	m_GroupMap( 23, HashMapMode(HashMapMode::HM_NO_RESIZE | HashMapMode::HM_USE_STRING_MODE) ),
	m_ObjectMap( _iSize, HashMapMode::HM_PREFER_PERFORMANCE ),
	m_DefaultGroup( 0 )
{
	m_GroupMap.SetDeleteCallback( DeleteGroup );
	m_ObjectMap.SetDeleteCallback( DeleteObject );
}

// ******************************************************************************** //
// Destruction deletes everything, but not the objects.
OrE::ADT::MultiMap::~MultiMap()
{
	m_GroupMap.Clear();
	m_ObjectMap.Clear();
}

// ******************************************************************************** //
// The method to add new objects.
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup0;
	bool bNew = true;
	ADTElementP pOE;

	// Since groups can only be added the reference+ID is persistent
	{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
		// Search group to get its ID.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup0 );
		Assert( pEl );
		pGroup0 = (Group*)pEl->pObject;
		Assert( pGroup0 );
	}

	// Since the hash maps are not thread safe lock during insertion
	{ LOCK
		// Object address = hash, data = array with pointers to the group entries (setup later)
		pOE = m_ObjectMap.Insert( nullptr, (qword)_pNewObject );
		Assert( pOE );
		// Reference counter should be one
		// -> double insertion occurred -> ref is 1 too large
		if( pOE->pObject ) { pOE->Release(); bNew = false; }

		// Insert into the group
		GroupIndex* pIndex = pOE->pObject ? (GroupIndex*)pOE->pObject : new GroupIndex();
		pOE->pObject = pIndex;
		// For each NEW group insert to this group - no double insertion
		if( !pIndex->pGroupE[pGroup0->m_ID] )
		{
			pIndex->pGroupE[pGroup0->m_ID] = pGroup0->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( bNew ) pIndex->pGroupE[0] = m_DefaultGroup.Insert( pOE );
	}
}

// ******************************************************************************** //
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup0;
	Group* pGroup1;
	bool bNew = true;
	ADTElementP pOE;

	// Since groups can only be added the reference+ID is persistent
	{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
		// Search group to get its ID.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup0 );
		Assert( pEl );
		pGroup0 = (Group*)pEl->pObject;
		Assert( pGroup0 );

		pEl = m_GroupMap.Search( _pcGroup1 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup1 );
		Assert( pEl );
		pGroup1 = (Group*)pEl->pObject;
		Assert( pGroup1 );
	}

	// Since the hash maps are not thread safe lock during insertion
	{ LOCK
		// Object address = hash, data = array with pointers to the group entries (setup later)
		pOE = m_ObjectMap.Insert( nullptr, (qword)_pNewObject );
		Assert( pOE );
		// Reference counter should be one
		// -> double insertion occurred -> ref is 1 too large
		if( pOE->pObject ) { pOE->Release(); bNew = false; }

		// Insert into the groups
		GroupIndex* pIndex = pOE->pObject ? (GroupIndex*)pOE->pObject : new GroupIndex();
		pOE->pObject = pIndex;
		// For each NEW group insert to this group - no double insertion
		if( !pIndex->pGroupE[pGroup0->m_ID] )
		{
			pIndex->pGroupE[pGroup0->m_ID] = pGroup0->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( !pIndex->pGroupE[pGroup1->m_ID] )
		{
			pIndex->pGroupE[pGroup1->m_ID] = pGroup1->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( bNew ) pIndex->pGroupE[0] = m_DefaultGroup.Insert( pOE );
	}
}

// ******************************************************************************** //
void OrE::ADT::MultiMap::Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1, const char* _pcGroup2 )
{
	// Break if no group defined
	if( !_pcGroup0 || _pcGroup0[0]==0 ) return;

	Group* pGroup0;
	Group* pGroup1;
	Group* pGroup2;
	bool bNew = true;
	ADTElementP pOE;

	// Since groups can only be added the reference+ID is persistent
	{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
		// Search group to get its ID.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup0 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup0 );
		Assert( pEl );
		pGroup0 = (Group*)pEl->pObject;
		Assert( pGroup0 );

		pEl = m_GroupMap.Search( _pcGroup1 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup1 );
		Assert( pEl );
		pGroup1 = (Group*)pEl->pObject;
		Assert( pGroup1 );

		pEl = m_GroupMap.Search( _pcGroup2 );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup2 );
		Assert( pEl );
		pGroup2 = (Group*)pEl->pObject;
		Assert( pGroup2 );
	}

	// Since the hash maps are not thread safe lock during insertion
	{ LOCK
		// Object address = hash, data = array with pointers to the group entries (setup later)
		pOE = m_ObjectMap.Insert( nullptr, (qword)_pNewObject );
		Assert( pOE );
		// Reference counter should be one
		// -> double insertion occurred -> ref is 1 too large
		if( pOE->pObject ) { pOE->Release(); bNew = false; }

		// Insert into the groups
		GroupIndex* pIndex = pOE->pObject ? (GroupIndex*)pOE->pObject : new GroupIndex();
		pOE->pObject = pIndex;
		// For each NEW group insert to this group - no double insertion
		if( !pIndex->pGroupE[pGroup0->m_ID] )
		{
			pIndex->pGroupE[pGroup0->m_ID] = pGroup0->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( !pIndex->pGroupE[pGroup1->m_ID] )
		{
			pIndex->pGroupE[pGroup1->m_ID] = pGroup1->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( !pIndex->pGroupE[pGroup2->m_ID] )
		{
			pIndex->pGroupE[pGroup2->m_ID] = pGroup2->Insert( pOE );
			++pIndex->iNumInGroups;
		}
		if( bNew ) pIndex->pGroupE[0] = m_DefaultGroup.Insert( pOE );
	}
}

// ******************************************************************************** //
// Remove the object from all groups and from the map O(1).
void OrE::ADT::MultiMap::Remove( void* _pObject )
{
	// Test if object is in map and stop if not
	{ LOCK
		ADTElementP pOE;
		if( !(pOE=m_ObjectMap.Search( (qword)_pObject )) ) return;

		// Remove from all groups
		GroupIndex* pIndex = (GroupIndex*)pOE->pObject;
		HashMap::Iterator It( &m_GroupMap );
		while( ++It )
		{
			int i = ((Group*)It->pObject)->m_ID;
			if( pIndex->pGroupE[ i ] )
			{
				((Group*)It->pObject)->Delete( pIndex->pGroupE[ i ] );
				pIndex->pGroupE[ i ] = nullptr;
			}
		}
		// Remove from map
		m_DefaultGroup.Delete( pIndex->pGroupE[ 0 ] );
		m_ObjectMap.Delete( pOE );
	}
}

// ******************************************************************************** //
// Maps an object to an existing or new group.
void OrE::ADT::MultiMap::Map( void* _pObject, const char* _pcGroup )
{
	if( IsDefaultGroup( _pcGroup ) ) return;

	ADTElementP pOE;

	// Search group to get its ID and to call insert
	Group* pGroup;
	// Since groups can only be added the reference+ID is persistent
	{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
		// Search group to get its ID.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup );
		if( !pEl ) pEl = m_GroupMap.Insert( new Group( m_GroupMap.GetNumElements()+1 ), _pcGroup );
		pGroup = (Group*)pEl->pObject;
	}
	Assert( pGroup );

	// Test if object is in map and stop if not
	{ LOCK
		// Object is not in map
		if( !(pOE=m_ObjectMap.Search( (qword)_pObject )) ) return;

		// The object is in map -> apply to new group
		GroupIndex* pIndex = (GroupIndex*)(pOE->pObject);
		if( !pIndex->pGroupE[pGroup->m_ID] )
		{
			pIndex->pGroupE[pGroup->m_ID] = pGroup->Insert( pOE );
			++pIndex->iNumInGroups;
		}
	}
}

// ******************************************************************************** //
// Removes the object from a group.
void OrE::ADT::MultiMap::Unmap( void* _pObject, const char* _pcGroup )
{
	if( IsDefaultGroup( _pcGroup ) ) return;

	// Search group to get its ID and to call delete
	Group* pGroup;
	// Since groups can only be added the reference+ID is persistent
	{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
		// Search group to get its ID.
		ADTElementP pEl = m_GroupMap.Search( _pcGroup );
		if( !pEl ) return;	// Group does not exist
		pGroup = (Group*)pEl->pObject;
	}

	{ LOCK
		// Search element.
		ADTElementP pOE = m_ObjectMap.Search( (qword)_pObject );
		if( !pOE ) return;

		// Delete from group and from index
		GroupIndex* pIndex = (GroupIndex*)pOE->pObject;
		if( pIndex->pGroupE[ pGroup->m_ID ] )
		{
			pGroup->Delete( pIndex->pGroupE[ pGroup->m_ID ] );
			pIndex->pGroupE[ pGroup->m_ID ] = nullptr;
			// If the current group was the last one, remove from default group too
			if( --pIndex->iNumInGroups == 0 )
			{
				// Remove from map
				m_DefaultGroup.Delete( pIndex->pGroupE[ 0 ] );
				m_ObjectMap.Delete( pOE );
			}
		}
	}
}

// ******************************************************************************** //
// Tests if an object is in a group.
bool OrE::ADT::MultiMap::IsIn( void* _pObject, const char* _pcGroup )
{
	// IsIn map general case.
	if( IsDefaultGroup( _pcGroup ) )
	{
		LOCK
		return m_ObjectMap.Search( (qword)_pObject ) != nullptr;
	} else {
		int ID;
		// Since groups can only be added the index is persistent
		{ std::lock_guard<std::mutex> Lock( m_LockGroupMap );
			// Search group to get its ID.
			ADTElementP pEl = m_GroupMap.Search( _pcGroup );
			if( !pEl ) return false;	// Group does not exist
			ID = ((Group*)pEl->pObject)->m_ID;
		}

		{ LOCK
			// Does object exists?
			ADTElementP pOE = m_ObjectMap.Search( (qword)_pObject );
			// The object can be found but not in group.
			// Easy case: not found.
			if( !pOE ) return false;

			// Is the group entry pointer set?
			return ((GroupIndex*)pOE->pObject)->pGroupE[ ID ] != nullptr;
		}
	}
}

// ******************************************************************************** //
// Create an iterator for an group. The group 0/nullptr/"" is the default group of all
// Objects.
OrE::ADT::MultiMap::Iterator OrE::ADT::MultiMap::GetIterator( const char* _pcGroup )
{
	// nullptr -> default group
	if( IsDefaultGroup( _pcGroup ) ) return Iterator( &m_DefaultGroup );

	{ // Lock for group search
		LOCK

		ADTElementP pEl = m_GroupMap.Search( _pcGroup );

		// Your group does not exists!
		// Don't try to get Iterators for non existing groups, because this cannot be
		// handled by the iterator (would slow down iterators, if implemented).
		Assert( pEl );

		return Iterator( (Group*)pEl->pObject );
	}
}
