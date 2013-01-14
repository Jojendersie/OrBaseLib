// ******************************************************************************** //
// OrMultiMap.h																		//
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
//																					//
// ******************************************************************************** //
//									CONTENTS										//
// The multimap maps a set of objects to one or more groups/properties.				//
// The groups are string identifiers. The Objects are of arbitrary types (void*)	//
// The initial idea was the use as scene graph, but there are probably much more	//
// use cases.																		//
// For more than 31 groups it is necessary to use a hierarchical approach. 			//
// E.g. a multimap with more multimap objects.										//
// To get very fast timings some object treatments are deferred. Everything is		//
// implemented in O(1) beside the traversing by the iterator. During iteration		//
// objects are removed from the groups and deleted if Remove() or Unmap() was called//
// for the object before. So for the iteration of n existing elements a total		//
// time of O(n+k) is necessary. Or calling ++It once needs O(1+k/n) time, where k	//
// is the number of objects removed since last traversing.							//
// If you don't traverse all groups the object is never really deleted and occupies	//
// space. Furthermore the real object is not deleted from the map. Only its mapping	//
// is removed. DO DELETE YOUR OBJECTS MANUAL. You can do this the same moment you	//
// delete it from map. It is not referenced afterwards.								//
// Do not use too many threads(>4) accessing the map, locking slows down the system!//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

class MultiMap
{
public: class Iterator;
private:

	// 8 to 16 Bytes per entry (depending on pointer size)
	struct GroupEntry
	{
		GroupEntry* pNext;			// Next entry in the same group
		GroupEntry* pPrev;			// Previous entry in the same group
		ADTElementP pObjectEntry;	// Address of Object in the main map
		int iRef;					// How many iterators references the object

		GroupEntry( GroupEntry* _pNext, ADTElementP _pObject ) : pNext(_pNext), pPrev(nullptr), pObjectEntry(_pObject), iRef(0) {}
	};

	class Group
	{
		GroupEntry* m_pFirst;
		int m_iNumEntries;

		// A mutex to lock during changes of this group
		std::mutex m_Lock;

		friend class OrE::ADT::MultiMap::Iterator;
	public:
		// ID managed from MultiMap = number of groups at insertion time
		int m_ID;

		// Construction and destruction of a single linked list
		Group( int _iID ) : m_iNumEntries(0), m_Lock(), m_pFirst(nullptr), m_ID(_iID) {}
		~Group()
		{
			// Halt if somebody accesses
			m_Lock.lock();
			GroupEntry* pNext = m_pFirst;
			while( pNext )
			{
				pNext = m_pFirst->pNext;
				delete m_pFirst;
				m_pFirst = pNext;
			}
			m_Lock.unlock();
		}

		// Thread safe insert at front O(1) 
		// Returns address of new entry
		GroupEntry* Insert( ADTElementP _pObject );

		// Thread safe Delete O(1).
		void Delete( GroupEntry* _pObject );

		const GroupEntry* GetFirst()	{ return m_pFirst; }
	};

	// Structure saved in the object map.
	struct GroupIndex
	{
		// Each pointer can be 0 or the entry in the group with associated
		// index.
		// pGroupE[0] is the default-group entry, which is always defined.
		GroupEntry* pGroupE[32];
		int iNumInGroups;

		// TODO: speed test
//		GroupIndex() : iNumInGroups(0)	{ for( int i=0; i<32; ++i) pGroupE[i] = 0; }
		GroupIndex() : iNumInGroups(0)	{ for( int i=0; i<16; ++i) ((uint64*)pGroupE)[i] = 0; }
	};

	// A dynamical map of the objects. Each object is mapped to an array
	// of links to the group entries. The map is resized if two many objects are
	// in the map. In case of a full map a remapping is necessary.
	// Possible actions are: Is object in map? Should object be deleted?
	// How many iterators have currently access to the object (ref.)?
	HashMap m_ObjectMap;
	HashMap m_GroupMap;
	Group m_DefaultGroup;

	// A mutex to lock whole MultiMap, if changing one of the two hash maps.
	// TODO: make hash map itself thread safe
	std::mutex m_Lock;
	std::mutex m_LockGroupMap;

	// Deferred object deletion. This decrements the reference counter
	// of the object and deletes it if necessary.
	//void UnrefObj( ADTElementP _pObject );


	// Prevent copy constructor and operator = being generated.
	MultiMap(const MultiMap&);
	const MultiMap& operator = (const MultiMap&);

	static void _cdecl OrE::ADT::MultiMap::DeleteGroup( void* _pGroup );
	static void _cdecl OrE::ADT::MultiMap::DeleteObject( void* _pGroup );
public:
	// Constructor creates an empty map of a chosen size.
	// Use a size slightly larger than the expected maximal amount.
	// A to large map slows the iteration over all items down, whereas a too
	// small one needs to be resized and therefore rehashed more often.
	MultiMap( int _iSize = 256 );

	// Destruction deletes everything, but not the objects. Make sure you delete
	// your objects elsewhere. They must not be deleted from map, but it is
	// strongly recommended (dangerous - misleading pointers!).
	~MultiMap();

	// The method to add new objects. Since objects have to be mapped to at least one
	// group the method applies the object to one or more groups.
	// The groups are created if mentioned the first time.
	// To add the object to more than 3 groups use the Map command. This is a little
	// more expensive, but in standard case the function is a little bit faster.
	// The method does nothing, if group is an empty string.
	void Add( void* _pNewObject, const char* _pcGroup0 );
	void Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1 );
	void Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1, const char* _pcGroup2 );

	// Remove the object from all groups and from the map O(#groups)=O(1).
	void Remove( void* _pObject );

	// Maps an object to an existing or new group. If the object is not Added before
	// nothing happens.
	// If the object is already in the group, it is in the group twice afterwards.
	void Map( void* _pObject, const char* _pcGroup );

	// Removes the object from a group. If this was the last group the object is removed
	// automatically and cannot be Map(ped) without a new Add.
	// Unmap is the slowest method O(k), where k denotes the group size!
	void Unmap( void* _pObject, const char* _pcGroup );

	// Tests if an object is in a group. You can use 0/nullptr/"" to test
	// if the object is in the map anyway.
	bool IsIn( void* _pObject, const char* _pcGroup = nullptr );

	int GetNumElements()				{ return m_ObjectMap.GetNumElements(); }
	//int GetNumDeletionMarkedElements()	{ return m_ObjectMap.GetNumElements() - m_iNumElements; }



	// The ADT::Iterator<> is not aviable, because the Multimap does not have the ADT interface
	// and more than one operator is possible for one map ( one for each group )
	class Iterator
	{
	private:
		// The map, whose group is traversed and the group.
		//HashMap*				m_pObjectMap;
		MultiMap::Group*		m_pGroup;

		// One element of the data set (current access goes to this element).
		MultiMap::GroupEntry*	m_pCurrentElement;

		// Creates an Iterator. This can only be done by the Multimap::GetIterator method
		Iterator( /*HashMap* _pMap,*/ MultiMap::Group* _pG ) :
			/*m_pObjectMap(_pMap),*/ m_pGroup(_pG), m_pCurrentElement(nullptr) {
		}

		friend class MultiMap;
	public:

		// Jumps to the first or the last element defined by the data structure.
		// Iterator<Multimap> It = MM.GetIterator();
		//	It->...				// access would cause an error
		//	It.SeekToBegin();	// ++It and It++ would have the same effect here
		//	It->...				// access now possible
		void SeekToBegin()	{ m_pCurrentElement = m_pGroup->m_pFirst; }

		// Override boolean operator that if(Iterator) can be asked
		operator bool () const { return m_pCurrentElement != nullptr; }

		// Override implicit conversation to the element type
		operator void* () const { return (void*)m_pCurrentElement->pObjectEntry->qwKey; }
		operator uiptr () const { return (uiptr)m_pCurrentElement->pObjectEntry->qwKey; }

		// Comparison operators
		bool operator==(const Iterator& T) const { return m_pCurrentElement == T.m_pCurrentElement; }
		bool operator!=(const Iterator& T) const { return m_pCurrentElement != T.m_pCurrentElement; }

		// Casting operators
		void* operator->() const {return (void*)m_pCurrentElement->pObjectEntry->qwKey;}
		void* operator&() const {return (void*)m_pCurrentElement->pObjectEntry->qwKey;}

		// Override ++ to navigate (thread safe)
		Iterator& operator++()
		{
			std::lock_guard<std::mutex> Guard( m_pGroup->m_Lock );

			// "Unlock" element
			if( m_pCurrentElement ) --m_pCurrentElement->iRef;
			m_pCurrentElement = m_pCurrentElement ? m_pCurrentElement->pNext : m_pGroup->m_pFirst;
			// "Lock" element
			if( m_pCurrentElement ) ++m_pCurrentElement->iRef;

			return *this;
		}
		// Postfix
		const Iterator operator++(int) { Iterator temp = *this; ++*this; return temp; }
	};

	// Create an iterator for an group. The group 0/nullptr/"" is the default group of all
	// Objects.
	//Iterator<Bucket> GetMapIterator()	{return Iterator<Bucket>(&m_GroupMap);}
	Iterator GetIterator( const char* _pcGroup = nullptr );
};

typedef MultiMap* MultiMapP;

// ******************************************************************************** //
// The iterator for the multimap is a complete reimplementation due to the number of
// different possible iterators.
/*template <> */

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //