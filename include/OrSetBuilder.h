// ******************************************************************************** //
// OrSetBuilder.h																	//
// ==============																	//
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
// ******************************************************************************** //
// Using an indexed finite data set the SetBuilder can be used to create partitions.//
// This can be used for clustering and graph algorithms (e.g. Kruskal's MST).		//
// Create()		O(n)																//
// Union()		O(1)																//
// GetSet()		~O(a)	- proportional to the inverse ackermann function ->			//
//						praktikaly constant.										//	
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {
	// ******************************************************************************** //
	// Each set is represented by an arbitrary element from the set. This is a kind
	// of set-ID. The set-IDs can change due to Union, but GetSet() returns the same
	// representant for all items in one set.
	class SetBuilder
	{
#pragma pack(push, 1)
		struct ElementInfo {
			bool bRepresentant;		// Is this element an representant?
			union {
				dword dwParentIdx;	// All non representants know a maybe repr.
				dword dwNextSet;	// Double linked list of sets (together with dwPreviousSet)
			};
			dword dwPreviousSet;	// Undifined if not the representant
			dword dwSetSize:24;		// Undifined if not the representant
			dword dwRank:8;			// Rank statistik for path compression

			ElementInfo(dword _dwIdx, dword _dwNum):bRepresentant(true),dwParentIdx((_dwIdx+1)%_dwNum),dwPreviousSet((_dwIdx+_dwNum-1)%_dwNum),dwRank(0),dwSetSize(1)	{}
		};
#pragma pack(pop)

		// Each index has additional informations to determine set infos.
		ElementInfo* m_pInfos;
		dword m_dwNumElements;

		// Prevent copy constructor and operator = being generated.
		SetBuilder(const SetBuilder&);
		const SetBuilder& operator = (const SetBuilder&);
	public:
		// Create one set per item (all items are there own set representants).
		SetBuilder(dword _dwMaxNumElements);
		~SetBuilder();

		// Union to of the subsets
		// Input: two representants/set-IDs, do not use other elements
		// Output: the new set representant (one of the two from input)
		dword Union(dword _dwRep1, dword _dwRep2);

		// Finds the representant of the set, which contains the given element. 
		dword GetSet(dword _dwElementIndex) const;

		// Replace the first element with the last one, i.e. the first element is deleted
		// and the second changes its index.
		//void Delete(dword _dwElementIndex)			{m_pInfos[_dwElementIndex] = m_pInfos[--m_dwNumElements];}

		dword GetSetSize(dword _dwRep) const		{return m_pInfos[_dwRep].dwSetSize;}
		dword GetNumElements() const				{return m_dwNumElements;}

		// Iterator options
		dword GetNextSet(dword _dwRep) const		{return m_pInfos[_dwRep].dwNextSet;}
		dword GetPreviousSet(dword _dwRep) const	{return m_pInfos[_dwRep].dwPreviousSet;}
	};
	typedef SetBuilder* SetBuilderP;
}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //