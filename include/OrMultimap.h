// ******************************************************************************** //
// OrMultimap.h																		//
// ============																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
//																					//
// ******************************************************************************** //
// The multimap maps a set of objects to one or more groups/properties.				//
// The groups are string identifiers. The Objects are of arbitary types (void*)		//
// The initial idea was the use as scene graph, but there are probably much more	//
// usecases.																		//
// For more than a few groups it is recommended to use a hierarchical approach. 	//
// E.g. a multimap with more multimap objects.										//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

class Multimap
{
private:
	// 8 to 16 Bytes per entry (depending on pointer size)
	struct GroupEntry
	{
		GroupEntry* pNext;
		void* pObject;			// Object address / hash value
	};

	class Group
	{
		GroupEntry* m_pFirst;
		int m_iNumEntries;
	};

	// A dynamical map of the objects. Each object is mapped to a reference
	// counter (int). The map is resized if two many objects are
	// in the map and reduced if the map is too empty. In case of a full
	// map a remapping is necessary.
	// Possible actions are: Is object in map? Should object be deleted?
	// How many iterators have currently access to the object (ref.)?
	HashMap m_ObjectMap;
	HashMap m_GroupMap;

	// Prevent copy constructor and operator = being generated.
	Multimap(const Multimap&);
	const Multimap& operator = (const Multimap&);
public:
	// Constructor creates an empty map of a choosen size.
	// Use a size slightly larger than the expected maximal amount.
	// A to large map slows the iteration over all items down, whereas a too
	// small one needs to be resized and therefore rehashed more often.
	Multimap( int _iSize = 256 );

	// Destruction deletes everything, but not the objects. Make sure you delete
	// your objects elsewhere. They must not be deleted from map, but it is
	// strongly recommended (dangerous - misleading pointers!).
	~Multimap();

	// The method to add new objects. Scince objects have to be mapped to at least one
	// group the method applies the object to one or more groups.
	// The groups are created if mentioned the first time.
	// To add the object to more than 3 groups use the Map command. This is a little
	// more expensive, but in standard case the function is a little bit faster.
	void Add( void* _pNewObject, const char* _pcGroup0 );
	void Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1 );
	void Add( void* _pNewObject, const char* _pcGroup0, const char* _pcGroup1, const char* _pcGroup2 );

	// Maps an object to an existing or new group. If the object is not Added before
	// nothing happens.
	void Map( void* _pObject, const char* _pcGroup );

	// Removes the object from a group. If this was the last group the object is removed
	// automaticly and cannot be Map(ped) without Add.
	void Unmap( void* _pObject, const char* _pcGroup );

	// Tests if an object is in a group. You can use 0/nullptr/"" to test
	// if the object is in the map anyway.
	void IsIn( void* _pObject, const char* _pcGroup );

	// Create an iterator for an group. The group 0/nullptr/"" is the default group of all
	// Objects.
};


}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //