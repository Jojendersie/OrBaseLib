// ******************************************************************************** //
// OrTrie.cpp																		//
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

// include for sprintf_s
#include <cstdio>
// include for malloc
#include <cstdlib>

#include "../include/OrTypeDef.h"
#include "../include/OrAssert.h"
#include "../include/OrADTObjects.h"
#include "../include/OrTrie.h"
#include "../include/OrFastMath.h"
#include "../include/OrBuffer.h"
#include "../include/OrUtils.h"

#include "../include/OrDebug.h"

using namespace OrE::ADT;
using namespace OrE::Math;


// ******************************************************************************** //
int OrE::ADT::Trie::Add( const char* _pcName, void* _pData, bool _bOverrideData )
{
	// Remember names in case of error
	//TrieString* pName = _pName;
	// The search starts at the root
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Compare strings
		uint32 dwEqual = OrE::Utils::Match(pCurrent->pSubString, _pcName);

		// If at least one character is equal go to subtree
		if(dwEqual)
		{
			// Set index to the last valid location
			uint32 dwI = dwEqual-1;
			// Case 1: _pcName ends -> Attach data to this node.
			// Overwrites data if already leave node.
			if(!_pcName[dwEqual])
			{
				// 1.1 The current node has a longer label. Cut the end.
				if(pCurrent->pSubString[dwEqual])
				{
					// Create new node on between.
					TrieNodeP pNew = new TrieNode;
					// Cut string and delete old one
					char* pcTemp = pCurrent->pSubString;
					pNew->pSubString = OrE::Utils::Substr(pcTemp, dwEqual, 0xffffffff);
					pCurrent->pSubString = OrE::Utils::Substr(pcTemp, 0, dwI);
					free(pcTemp);
					// Append new child to current node. The remaining tree needs no changes.
					pNew->pChild = pCurrent->pChild;
					pCurrent->pChild = pNew;
					pNew->pNext = 0;
					pNew->pData = pCurrent->pData;
					pCurrent->pData = _pData;
					return 0;
				} else
				{
					// Node label identical with searched one -> overwrite
					if(!pCurrent->pData
						|| _bOverrideData) // Do not notify the override (in return) if _bOverrideData==true
					{
						pCurrent->pData = _pData;
						return 0;
					} else
					{
						pCurrent->pData = _pData;
						return 1;
					}	// if(!pCurrent->pData)
				}	// if(pCurrent->pcSubString[dwEqual])
			}	// if(dwEqual)

			// Case 2: Node label is equal to the beginning of the searched string.
			// Search recursive.
			if(!pCurrent->pSubString[dwEqual])
			{
				if(_bOverrideData) pCurrent->pData = _pData;
				if(pCurrent->pChild) {
					pCurrent = pCurrent->pChild;
					_pcName = &_pcName[dwEqual];
				} else {
					// Case 3: End of tree -> new leave.
					pCurrent->pChild = new TrieNode;
					pCurrent->pChild->pSubString = OrE::Utils::Substr(_pcName, dwEqual, 0xffffffff);
					pCurrent->pChild->pChild = 0;
					pCurrent->pChild->pNext = 0;
					pCurrent->pChild->pData = _pData;
					return 0;
				}
			} else {
				// Case 4: equally sequence length < length of node and searched string -> 2 new
				// nodes (one with the equally sequence + 2 Subtree with the both remaining parts.

				// Create new nodes
				TrieNodeP pNew_Name = new TrieNode;
				TrieNodeP pNew_Old = new TrieNode;
				pNew_Name->pSubString = OrE::Utils::Substr(_pcName, dwEqual, 0xffffffff);
				pNew_Name->pChild = 0;
				pNew_Name->pNext = pNew_Old;
				pNew_Name->pData = _pData;

				// Split string and delete old one.
				char* pcTemp = pCurrent->pSubString;
				pNew_Old->pChild = pCurrent->pChild;
				pNew_Old->pSubString = OrE::Utils::Substr(pcTemp, dwEqual, 0xffffffff);
				pNew_Old->pNext = 0;
				pNew_Old->pData = pCurrent->pData;

				pCurrent->pChild = pNew_Name;
				pCurrent->pSubString = OrE::Utils::Substr(pcTemp, 0, dwI);
				if(_bOverrideData) pCurrent->pData = _pData;
				else pCurrent->pData = 0;
				free(pcTemp);
				return 0;
			}
		} else {
			// no equality - search in other subtree on same level
			if(pCurrent->pNext)
				pCurrent = pCurrent->pNext;
			else {
				// End of Tree -> new leave.
				pCurrent->pNext = new TrieNode;
				pCurrent->pNext->pSubString = OrE::Utils::Substr(_pcName, 0, 0xffffffff);
				pCurrent->pNext->pChild = 0;
				pCurrent->pNext->pNext = 0;
				pCurrent->pNext->pData = _pData;
				return 0;
			}
		}
	}

	if(!m_pFirstNode)
	{
		// Tree is empty -> Create first node
		m_pFirstNode = new TrieNode;
		m_pFirstNode->pSubString = OrE::Utils::Substr(_pcName, 0, 0xffffffff);
		m_pFirstNode->pChild = 0;
		m_pFirstNode->pNext = 0;
		m_pFirstNode->pData = _pData;
		return 0;
	}

	return 2;
}

// ******************************************************************************** //
TrieNodeP OrE::ADT::Trie::Match( const char** _ppcName )
{
	// Introduce a marker to remember biggest matching subsequence
	TrieNodeP pLastPossibleData = 0;
	const char* pcLastPossibleName = *_ppcName;
//	uint32 dwLastPossibleLen = _Name.m_dwLen;
	// Search from root
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Compare strings
		uint32 dwEqual = OrE::Utils::Match(pCurrent->pSubString, *_ppcName);

		if(dwEqual)
		{
			*_ppcName = *_ppcName+dwEqual;

			// The current node has a longer label.
			if(pCurrent->pSubString[dwEqual])
			{
				*_ppcName = pcLastPossibleName;
				return nullptr;
			}

			// 1. Case: End of name and complete match
			if(!(*_ppcName)[dwEqual]) 
			{
				if(pCurrent->pData) return pCurrent;
				else{
					(*_ppcName) = pcLastPossibleName;
					return pLastPossibleData;
				}
			}

			// 2. Case: Name continues -> Search subtree.
			// If the current node contains data set the marker.
			if(pCurrent->pData)
			{
				pLastPossibleData = pCurrent;
				pcLastPossibleName = *_ppcName;
			}
			pCurrent = pCurrent->pChild;
		} else
			// Search on the same level
			pCurrent = pCurrent->pNext;
	}
	// No complete match
	//assert(*_ppcName == pcLastPossibleName);
	(*_ppcName) = pcLastPossibleName;
	return pLastPossibleData;
}

// ******************************************************************************** //
void OrE::ADT::Trie::Remove( const char* _pcName, void* _pData )
{
	// Remove data independent of the rest, even if tree conflicted.
	// Don't set _pData to 0 - address is used to find the node
	if( _pData && m_pDeleteCallback ) m_pDeleteCallback( _pData );

	// Go through the tree similar to insertion and search.
	// Delete whole path to the node if the removed data was the last one in that
	// path. Collect path on stack and clear if other data is found.
	// Delete all nodes on the stack at the end of search.
	OrE::ADT::Buffer Stack(60);	// Hopefully enough

	// Search and iterate over tree
	TrieNodeP pCurrent = m_pFirstNode;
	TrieNodeP pPrevious = nullptr;
	while(pCurrent)
	{
		// Compare strings
		uint32 dwEqual = OrE::Utils::Match(pCurrent->pSubString, _pcName);

		if(dwEqual)
		{
			// Error if label in tree node bigger than remaining name
			if(pCurrent->pSubString[dwEqual])
			{
				Assert(pCurrent->pData != _pData);
				break;
			}

			// 1. Case: Name found
			if(!_pcName[dwEqual])
			{
				if(pCurrent->pData == _pData || !pCurrent->pData)
				{
					// Current node to stack for uniform handling
					pCurrent->pData = nullptr;
					Stack.Push(pPrevious);
					Stack.Push(pCurrent);	// A potential deleted node
					// If nothing depends on the node delete it and all others from the stack.
					if(!pCurrent->pChild)
					{
						while(!Stack.IsEmpty())
						{
							// Look up element + parent node
							pCurrent = (TrieNodeP)Stack.Pop();
							pPrevious = (TrieNodeP)Stack.Pop();
							// Repair tree structure
							if(!pPrevious) m_pFirstNode = pCurrent->pNext;
							else if(pPrevious->pChild == pCurrent)
								pPrevious->pChild = pCurrent->pNext;
							else
								pPrevious->pNext = pCurrent->pNext;

							// Delete node
							free(pCurrent->pSubString);
							delete pCurrent;
						}
					}
					break;
				} else {
					// Wrong data in current node
					break;
				}
			}

			// 2. Case: Name continues -> Search subtree.
			// If node has so correct data or no data remember for deletion.
			// If node has some other data the path up to know is independent of
			// the current node. Do not delete the path -> clear stack.
			if(pCurrent->pData == _pData || !pCurrent->pData)
			{
				pCurrent->pData = nullptr;
				Stack.Push(pPrevious);
				Stack.Push(pCurrent);
			} else
			{
				// Forgot data
				Stack.Clear();
			}
			pPrevious = pCurrent;
			_pcName = &_pcName[dwEqual];
			pCurrent = pCurrent->pChild;
		} else {
			Assert(pCurrent->pData != _pData);
			// Search on the same level
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
	// No match -> straighten up the stack
	Stack.Clear();
}

// ******************************************************************************** //
void OrE::ADT::Trie::ReleaseTrie(TrieNodeP _pNode)
{
	// Start recursion in root node
	if(!_pNode)
	{
		if(!m_pFirstNode) return;
		_pNode = m_pFirstNode;
		m_pFirstNode = 0;
	}

	// Subtrees or other node on the same level
	if( _pNode->pChild ) ReleaseTrie( _pNode->pChild );
	if( _pNode->pNext ) ReleaseTrie( _pNode->pNext );

	if(_pNode->pData && m_pDeleteCallback)
		m_pDeleteCallback(_pNode->pData);

	// delete the node
	free( _pNode->pSubString );
	delete _pNode;
}





// ******************************************************************************** //
/*OrE::ADT::TrieString::~TrieString()	{if(m_bDel) free(const_cast<char*>(m_pcString));}

// ******************************************************************************** //
// Auxiliary function to create a copy of a subsrting.
// Parameter:
//	_dwFrom - 0-indexed imdex of first char to copy (inclusive)
//	_dwTo - 0-indexed imdex of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix begining in _dwFrom
TrieString* OrE::ADT::TrieString::substr(const uint32 _dwFrom, uint32 _dwTo) const
{
	// Spezialfall: String bis zum Ende
	if(_dwTo == 0xffffffff) _dwTo = m_dwLen?(m_dwLen-1):0;

	// Definition: von 0 bis 0 bedeutet wir wollen ein Zeichen
	char* pcRet = (char*)malloc((_dwTo+1-_dwFrom)*sizeof(char));
	uint32 i=_dwFrom;
	for(;i<=_dwTo;++i)
		pcRet[i-_dwFrom] = m_pcString[i];

	return new TrieString(_dwTo+1-_dwFrom, pcRet);
}

// ******************************************************************************** //
uint32 OrE::ADT::TrieString::match(const TrieString* _pStr) const
{
	uint32 dwMax = Min(_pStr->m_dwLen, m_dwLen);
	uint32 i;
	for(i=0; (i<dwMax) && (_pStr->m_pcString[i]==m_pcString[i]); ++i);
	return i;
}

// ******************************************************************************** //
int OrE::ADT::Trie::Add(TrieString _Name, void* _pData, bool _bOverrideData)
{
	// Name merken, falls Fehler
	//TrieString* pName = _pName;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		uint32 dwEqual=pCurrent->pSubString->match(&_Name);

		// Wenn min. ein Zeichen gleich, dann Unterbaum besuchen
		if(dwEqual)
		{
			// Index auf letzte g�ltige Stelle setzen
			uint32 dwI = dwEqual-1;
			// Fall 1: _pcName ist zu ende -> Funktion an diesen Knoten
			// anh�ngen. �berladen/�berschreiben wenn bereits Endknoten.
			if(_Name.m_dwLen == dwEqual)
			{
				// 1.1 Der Knoten hat hier noch einen l�ngeren String, das
				// ist jetzt ung�ltig und wird abgetrennt.
				if(pCurrent->pSubString->m_dwLen > dwEqual)
				{
					// Neuen zwischenknoten erstellen.
					TrieNodeP pNew = new TrieNode;
					// String zerteilen und alten l�schen
					TrieString* pTemp = pCurrent->pSubString;
					pNew->pSubString = pTemp->substr(dwEqual, 0xffffffff);
					pCurrent->pSubString = pTemp->substr(0, dwI);
					delete pTemp;
					// Neues Kind unter aktuellem Knoten, da so die
					// Parentobjekte g�ltig bleiben.
					pNew->pChild = pCurrent->pChild;
					pCurrent->pChild = pNew;
					pNew->pNext = 0;
					pNew->pData = pCurrent->pData;
					pCurrent->pData = _pData;
					return 0;
				} else
				{
					// Knoten identisch mit Restnamen -> Knoten beibehalten, Daten Neu/�berschreiben
					if(!pCurrent->pData
						|| _bOverrideData) // Do not notify the override (in return) if _bOverrideData==true
					{
						pCurrent->pData = _pData;
						return 0;
					} else
					{
						pCurrent->pData = _pData;
						return 1;
					}	// if(!pCurrent->pData)
				}	// if(pCurrent->pcSubString[dwEqual])
			}	// if(dwEqual)

			// Fall 2: Gleichheit Knoten mit Anfang des Namens ->
			// Restnamen in Unterbaum suchen (rekursiv)
			if(pCurrent->pSubString->m_dwLen == dwEqual)
			{
				if(_bOverrideData) pCurrent->pData = _pData;
				if(pCurrent->pChild) {
					pCurrent = pCurrent->pChild;
					_Name.m_pcString = &_Name.m_pcString[dwEqual];
					_Name.m_dwLen -= dwEqual;
				} else {
					// Fall 3: Baum zu Ende -> neues Kind mit Restnamen.
					pCurrent->pChild = new TrieNode;
					pCurrent->pChild->pSubString = _Name.substr(dwEqual, 0xffffffff);
					pCurrent->pChild->pChild = 0;
					pCurrent->pChild->pNext = 0;
					pCurrent->pChild->pData = _pData;
					return 0;
				}
			} else {
				// Fall 4: Teilsequenz < als L�nge der beiden -> 2 neue
				// Knoten (einen mit Rest des alten Knoten + Subbaum und
				// den anderen mit Rest des Namens.

				// Neue Kindsknoten erstellem
				TrieNodeP pNew_Name = new TrieNode;
				TrieNodeP pNew_Old = new TrieNode;
				pNew_Name->pSubString = _Name.substr(dwEqual, 0xffffffff);
				pNew_Name->pChild = 0;
				pNew_Name->pNext = pNew_Old;
				pNew_Name->pData = _pData;

				// String zerteilen und alten l�schen
				TrieString* pTemp = pCurrent->pSubString;
				pNew_Old->pChild = pCurrent->pChild;
				pNew_Old->pSubString = pTemp->substr(dwEqual, 0xffffffff);
				pNew_Old->pNext = 0;
				pNew_Old->pData = pCurrent->pData;

				pCurrent->pChild = pNew_Name;
				pCurrent->pSubString = pTemp->substr(0, dwI);
				if(_bOverrideData) pCurrent->pData = _pData;
				else pCurrent->pData = 0;
				delete pTemp;
				return 0;
			}
		} else {
			// keine Gleichheit -> im n�chsten Teilbaum suchen.
			if(pCurrent->pNext)
				pCurrent = pCurrent->pNext;
			else {
				// Baum zu Ende -> neuer Unterknoten.
				pCurrent->pNext = new TrieNode;
				pCurrent->pNext->pSubString = _Name.substr(0, 0xffffffff);
				pCurrent->pNext->pChild = 0;
				pCurrent->pNext->pNext = 0;
				pCurrent->pNext->pData = _pData;
				return 0;
			}
		}
	}

	if(!m_pFirstNode)
	{
		// Baum noch leer -> ersten Kindsknoten anlegen
		m_pFirstNode = new TrieNode;
		m_pFirstNode->pSubString = _Name.substr(0, 0xffffffff);
		m_pFirstNode->pChild = 0;
		m_pFirstNode->pNext = 0;
		m_pFirstNode->pData = _pData;
		return 0;
	}

	return 2;
}

// ******************************************************************************** //
TrieNodeP OrE::ADT::Trie::Match(TrieString& _Name)
{
	// Da gr��tm�gliche Teilfunktion gesucht einen Merker einf�hren.
	TrieNodeP pLastPossibleData = 0;
	const char* pcLastPossibleName = _Name.m_pcString;
	uint32 dwLastPossibleLen = _Name.m_dwLen;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		uint32 dwEqual=pCurrent->pSubString->match(&_Name);

		if(dwEqual)
		{
			_Name.m_pcString = _Name.m_pcString+dwEqual;
			_Name.m_dwLen -= dwEqual;

			// Baumknoten l�nger als Name, aber gleichen Anfangsteil
			if(pCurrent->pSubString->m_dwLen > dwEqual)
				return pCurrent;

			// 1.Fall: Name zu Ende und Fund.
			if(!_Name.m_dwLen) 
			{
				if(pCurrent->pData) return pCurrent;
				else{
					_Name.m_pcString = pcLastPossibleName;
					_Name.m_dwLen = dwLastPossibleLen;
					// Fehler: aktueller Knoten enth�lt keine Daten
					//OrSE_Error("Funktionsname enth�lt keine Daten.");
					return pLastPossibleData;
				}
			}

			// 2.Fall: Name geht weiter -> Subbaum durchsuchen.
			// Wenn der aktuelle Knoten daten enth�lt merken.
			if(pCurrent->pData)
			{
				pLastPossibleData = pCurrent;
				pcLastPossibleName = _Name.m_pcString;
				dwLastPossibleLen = _Name.m_dwLen;
			}
			pCurrent = pCurrent->pChild;
		} else
			// Auf dieser Ebene Suche Fortf�hren
			pCurrent = pCurrent->pNext;
	}
	// kein Fund
	assert(_Name.m_pcString == pcLastPossibleName);
	//(*_ppcName) = pcLastPossibleName;
	return pLastPossibleData;
}

// ******************************************************************************** //
void OrE::ADT::Trie::Remove(TrieString& _Name, void* _pData)
{
	// Daten freigeben egal was passiert. Die Speicheradresse ben�tigen wir
	// aber noch zum vergleichen.
	if(_pData && m_pDeleteCallback) m_pDeleteCallback(_pData);

	// �hnlich dem Suchen oder einf�gen den Baum durchgehen.
	// Jeden Knoten, der auf die Daten zeigt markieren (Datenpointer l�schen,
	// Zeiger auf Knoten auf den Stack packen.
	// Immer wenn eine Abh�ngigkeit eines Knotens zu anderen Namen/Daten
	// detektiert wird den Stack leeren.
	//int iNumStacked = 0;
	// Am Ende alle auf dem Stack befindlichen Knoten l�schen.
	OrE::ADT::Buffer Stack(60);	// Hopely enough

	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	TrieNodeP pPrevious = nullptr;
	while(pCurrent)
	{
		// Strings vergleichen
		uint32 dwEqual=pCurrent->pSubString->match(&_Name);

		if(dwEqual)
		{
			// Fehler, wenn Baumknoten l�nger als Name, aber gleichen Anfangsteil
			if(pCurrent->pSubString->m_dwLen > dwEqual)
			{
				assert(pCurrent->pData != _pData);
				//OrSE_Error("Funktionsname zu kurz (unbekannter Teilname einer Funktion).");
				break;
			}

			// 1.Fall: Name zu Ende und Fund.
			if(_Name.m_dwLen == dwEqual)
			{
				if(pCurrent->pData == _pData || !pCurrent->pData)
				{
					// Aktuellen Knoten auch noch auf den Stack packen (zur einheitlichen Verarbeitung)
					pCurrent->pData = nullptr;
					Stack.Push(pPrevious);
					Stack.Push(pCurrent);
//					__asm push pPrevious;
//					__asm push pCurrent;
//					++iNumStacked;		// Ein potentiell zu l�schender Knoten mehr
					// Wenn nichts mehr von diesem Knoten abh�ngt diesen und alle auf dem Stack l�schen
					if(!pCurrent->pChild)
					{
						while(!Stack.IsEmpty())
						//for(int i=0;i<iNumStacked;++i)
						{
							// Hole Knoten und Elternelement vom Stack
							pCurrent = (TrieNodeP)Stack.Pop();
							pPrevious = (TrieNodeP)Stack.Pop();
//							__asm pop pCurrent;
//							__asm pop pPrevious;
							// Repair tree structure
							if(!pPrevious) m_pFirstNode = pCurrent->pNext;
							else if(pPrevious->pChild == pCurrent)
								pPrevious->pChild = pCurrent->pNext;
							else
								pPrevious->pNext = pCurrent->pNext;

							// Delete node
							delete pCurrent->pSubString;
							delete pCurrent;
						}
						//iNumStacked = 0;
					}
					break;
				} else {
					// Aktueller Knoten enth�lt falsche Daten
					break;
				}
			}

			// 2.Fall: Name geht weiter -> Subbaum durchsuchen.
			// Wenn der aktuelle Knoten gleiche oder nulldaten hat merken.
			// Wenn Knoten unabh�ngig auf den Stack, sonst Stack leeren
			if(pCurrent->pData == _pData || !pCurrent->pData)
			{
				pCurrent->pData = nullptr;
				Stack.Push(pPrevious);
				Stack.Push(pCurrent);
			} else
			{
				// Daten in den Skat dr�cken
				Stack.Clear();
			}
			pPrevious = pCurrent;
			_Name.m_pcString = &_Name.m_pcString[dwEqual];
			_Name.m_dwLen -= dwEqual;
			pCurrent = pCurrent->pChild;
		} else {
			assert(pCurrent->pData != _pData);
			// Auf dieser Ebene Suche Fortf�hren
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
	// kein Fund -> Stack aufr�umen
	Stack.Clear();
}

// ******************************************************************************** //
void OrE::ADT::Trie::ReleaseTrie(TrieNodeP _pNode)
{
	// Rekursionsstart im obersten Knoten
	if(!_pNode)
	{
		if(!m_pFirstNode) return;
		_pNode = m_pFirstNode;
		m_pFirstNode = 0;
	}

	// Informationen �ber weitere Knoten verwenden.
	if(_pNode->pChild) ReleaseTrie(_pNode->pChild);
	if(_pNode->pNext) ReleaseTrie(_pNode->pNext);

	if(_pNode->pData && m_pDeleteCallback)
		m_pDeleteCallback(_pNode->pData);

	// jetzt brauchen wir den Knoten nicht mehr
	delete _pNode->pSubString;
	delete _pNode;
}*/

// *************************************EOF**************************************** //