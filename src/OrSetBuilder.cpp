// ******************************************************************************** //
// OrSetBuilder.cpp																	//
// ================																	//
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
#include "..\include\OrSetBuilder.h"
#include <cstdlib>

// ******************************************************************************** //
// Create one set per item (all items are there own set representants).
OrE::ADT::SetBuilder::SetBuilder(dword _dwMaxNumElements):
	m_dwNumElements(_dwMaxNumElements)
{
	dword dwSize = sizeof(OrE::ADT::SetBuilder::ElementInfo);
	m_pInfos = (OrE::ADT::SetBuilder::ElementInfo*)malloc(dwSize * _dwMaxNumElements);
	// Init each element as a set
	for(dword i=0;i<_dwMaxNumElements;++i)
		m_pInfos[i] = OrE::ADT::SetBuilder::ElementInfo(i, _dwMaxNumElements);
}

// ******************************************************************************** //
OrE::ADT::SetBuilder::~SetBuilder()
{
	free(m_pInfos);
}

// ******************************************************************************** //
// Union to of the subsets
// Input: two representants/set-IDs, do not use other elements
// Output: the new set representant (one of the two from input)
dword OrE::ADT::SetBuilder::Union(dword _dwRep1, dword _dwRep2)
{
	if(_dwRep1==_dwRep2) return _dwRep1;
	if(m_pInfos[_dwRep1].dwRank >= m_pInfos[_dwRep2].dwRank)
	{
		// Delete from set list
		m_pInfos[m_pInfos[_dwRep2].dwPreviousSet].dwNextSet = m_pInfos[_dwRep2].dwNextSet;
		m_pInfos[m_pInfos[_dwRep2].dwNextSet].dwPreviousSet = m_pInfos[_dwRep2].dwPreviousSet;
		// Meld the two sets with consistent rank
		m_pInfos[_dwRep2].bRepresentant = false;
		m_pInfos[_dwRep2].dwParentIdx = _dwRep1;
		if(m_pInfos[_dwRep1].dwRank == m_pInfos[_dwRep2].dwRank) ++m_pInfos[_dwRep1].dwRank;
		// Update other statistic information
		m_pInfos[_dwRep1].dwSetSize += m_pInfos[_dwRep2].dwSetSize;
		return _dwRep1;
	} else 
	{
		// Delete from set list
		m_pInfos[m_pInfos[_dwRep1].dwPreviousSet].dwNextSet = m_pInfos[_dwRep1].dwNextSet;
		m_pInfos[m_pInfos[_dwRep1].dwNextSet].dwPreviousSet = m_pInfos[_dwRep1].dwPreviousSet;
		// Meld the two sets, the rank of 2 > rank 1 -> no change
		m_pInfos[_dwRep1].bRepresentant = false;
		m_pInfos[_dwRep1].dwParentIdx = _dwRep2;
		// Update other statistic information
		m_pInfos[_dwRep2].dwSetSize += m_pInfos[_dwRep1].dwSetSize;
		return _dwRep2;
	}
}

// ******************************************************************************** //
// Finds the representant of the set, which contains the given element. 
dword OrE::ADT::SetBuilder::GetSet(dword _dwElementIndex) const
{
	// The Element can be the representant
	if(m_pInfos[_dwElementIndex].bRepresentant) return _dwElementIndex;
	
	// Already compressed path?
	dword dwParent = m_pInfos[_dwElementIndex].dwParentIdx;
	if(m_pInfos[dwParent].bRepresentant) return dwParent;

	// Otherwise do path compression rekursive
	return m_pInfos[_dwElementIndex].dwParentIdx = GetSet(dwParent);
}

// *************************************EOF**************************************** //