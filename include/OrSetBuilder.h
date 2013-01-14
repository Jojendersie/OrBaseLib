// ******************************************************************************** //
// OrSetBuilder.h																	//
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
// Using an indexed finite data set the SetBuilder can be used to create partitions.//
// This can be used for clustering and graph algorithms (e.g. Kruskal's MST).		//
// Create()		O(n)																//
// Union()		O(1)																//
// GetSet()		~O(a)	- proportional to the inverse ackermann function ->			//
//						practically constant.										//	
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {
	// ******************************************************************************** //
	// Each set is represented by an arbitrary element from the set. This is a kind
	// of set-ID. The set-IDs can change due to Union, but GetSet() returns the same
	// representative for all items in one set.
	class SetBuilder
	{
#pragma pack(push, 1)
		struct ElementInfo {
			bool bRepresentant;		// Is this element an representative?
			union {
				uint32 dwParentIdx;	// All non representatives know a maybe repr.
				uint32 dwNextSet;	// Double linked list of sets (together with dwPreviousSet)
			};
			uint32 dwPreviousSet;	// Undefined if not the representative
			uint32 dwSetSize:24;		// Undefined if not the representative
			uint32 dwRank:8;			// Rank statistic for path compression

			ElementInfo(uint32 _dwIdx, uint32 _dwNum):bRepresentant(true),dwParentIdx((_dwIdx+1)%_dwNum),dwPreviousSet((_dwIdx+_dwNum-1)%_dwNum),dwRank(0),dwSetSize(1)	{}
		};
#pragma pack(pop)

		// Each index has additional informations to determine set infos.
		ElementInfo* m_pInfos;
		uint32 m_dwNumElements;

		// Prevent copy constructor and operator = being generated.
		SetBuilder(const SetBuilder&);
		const SetBuilder& operator = (const SetBuilder&);
	public:
		// Create one set per item (all items are there own set representatives).
		SetBuilder(uint32 _dwMaxNumElements);
		~SetBuilder();

		// Union to of the subsets
		// Input: two representatives/set-IDs, do not use other elements
		// Output: the new set representative (one of the two from input)
		uint32 Union(uint32 _dwRep1, uint32 _dwRep2);

		// Finds the representative of the set, which contains the given element. 
		uint32 GetSet(uint32 _dwElementIndex) const;

		// Replace the first element with the last one, i.e. the first element is deleted
		// and the second changes its index.
		//void Delete(uint32 _dwElementIndex)			{m_pInfos[_dwElementIndex] = m_pInfos[--m_dwNumElements];}

		uint32 GetSetSize(uint32 _dwRep) const		{return m_pInfos[_dwRep].dwSetSize;}
		uint32 GetNumElements() const				{return m_dwNumElements;}

		// Iterator options
		uint32 GetNextSet(uint32 _dwRep) const		{return m_pInfos[_dwRep].dwNextSet;}
		uint32 GetPreviousSet(uint32 _dwRep) const	{return m_pInfos[_dwRep].dwPreviousSet;}
	};
	typedef SetBuilder* SetBuilderP;
}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //