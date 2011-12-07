/*************************************************************************

	Trie.cpp
	==========
	Diese Datei ist Teil der Orkteck-Script-Engine.

	Zweck:
	Ein Trie dient der effizienten Erfassung von Wörtern aus einem
	Wörterbuch.
	An Jedes Wort sind in diesem Spezialfall Funktionspointer gebunden.

	Autor: Johannes Jendersie
	Verwendete Quellen: AuD II Vorlesung;Rösner;Sommersemester 2010

*************************************************************************/

// include for sprintf_s
#include <stdio.h>
// include for strcmp
#include <string.h>
// include for malloc
#include <stdlib.h>
#include <assert.h>

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrADTObjects.h"
#include "..\Include\OrTrie.h"
#include "..\Include\OrFastMath.h"

using namespace OrE::ADT;
using namespace OrE::Math;

// ******************************************************************** //
// Robust auxiliary function to check if *p1 != *p2
/*bool strneq(const char* p1, const char* p2)
{
	if(p1==p2) return false;
	if(p1==0) return true;	// p2 muss ja !=p1 sein und ist !=0
	if(p2==0) return true;
	return strcmp(p1, p2)!=0;
}*/

// ******************************************************************** //
OrE::ADT::TrieString::~TrieString()	{if(m_bDel) free(m_pcString);}

// ******************************************************************** //
// Auxiliary function to create a copy of a subsrting.
// Parameter:
//	_dwFrom - 0-indexed imdex of first char to copy (inclusive)
//	_dwTo - 0-indexed imdex of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix begining in _dwFrom
TrieString* OrE::ADT::TrieString::substr(const dword _dwFrom, dword _dwTo) const
{
	// Spezialfall: String bis zum Ende
	if(_dwTo == 0xffffffff) _dwTo = m_dwLen?(m_dwLen-1):0;

	// Definition: von 0 bis 0 bedeutet wir wollen ein Zeichen
	char* pcRet = (char*)malloc((_dwTo+1-_dwFrom)*sizeof(char));
	dword i=_dwFrom;
	for(;i<=_dwTo;++i)
		pcRet[i-_dwFrom] = m_pcString[i];

	return new TrieString(_dwTo+1-_dwFrom, pcRet);
}

// ******************************************************************** //
dword OrE::ADT::TrieString::match(const TrieString* _pStr) const
{
	dword dwMax = Minu(_pStr->m_dwLen, m_dwLen);
	dword i;
	for(i=0; (i<dwMax) && (_pStr->m_pcString[i]==m_pcString[i]); ++i);
	return i;
}

// ******************************************************************** //
int OrE::ADT::Trie::Add(TrieString _Name, void* _pData, bool _bOverrideData)
{
	// Name merken, falls Fehler
	//TrieString* pName = _pName;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=pCurrent->pSubString->match(&_Name);

		// Wenn min. ein Zeichen gleich, dann Unterbaum besuchen
		if(dwEqual)
		{
			// Index auf letzte gültige Stelle setzen
			dword dwI = dwEqual-1;
			// Fall 1: _pcName ist zu ende -> Funktion an diesen Knoten
			// anhängen. Überladen/Überschreiben wenn bereits Endknoten.
			if(_Name.m_dwLen == dwEqual)
			{
				// 1.1 Der Knoten hat hier noch einen längeren String, das
				// ist jetzt ungültig und wird abgetrennt.
				if(pCurrent->pSubString->m_dwLen > dwEqual)
				{
					// Neuen zwischenknoten erstellen.
					TrieNodeP pNew = new TrieNode;
					// String zerteilen und alten löschen
					TrieString* pTemp = pCurrent->pSubString;
					pNew->pSubString = pTemp->substr(dwEqual, 0xffffffff);
					pCurrent->pSubString = pTemp->substr(0, dwI);
					delete pTemp;
					// Neues Kind unter aktuellem Knoten, da so die
					// Parentobjekte gültig bleiben.
					pNew->pChild = pCurrent->pChild;
					pCurrent->pChild = pNew;
					pNew->pNext = 0;
					pNew->pData = pCurrent->pData;
					pCurrent->pData = _pData;
					return 0;
				} else
				{
					// Knoten identisch mit Restnamen -> Knoten beibehalten, Daten Neu/Überschreiben
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
				// Fall 4: Teilsequenz < als Länge der beiden -> 2 neue
				// Knoten (einen mit Rest des alten Knoten + Subbaum und
				// den anderen mit Rest des Namens.

				// Neue Kindsknoten erstellem
				TrieNodeP pNew_Name = new TrieNode;
				TrieNodeP pNew_Old = new TrieNode;
				pNew_Name->pSubString = _Name.substr(dwEqual, 0xffffffff);
				pNew_Name->pChild = 0;
				pNew_Name->pNext = pNew_Old;
				pNew_Name->pData = _pData;

				// String zerteilen und alten löschen
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
			// keine Gleichheit -> im nächsten Teilbaum suchen.
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

// ******************************************************************** //
TrieNodeP OrE::ADT::Trie::Match(TrieString& _Name)
{
	// Da größtmögliche Teilfunktion gesucht einen Merker einführen.
	TrieNodeP pLastPossibleData = 0;
	char* pcLastPossibleName = _Name.m_pcString;
	dword dwLastPossibleLen = _Name.m_dwLen;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=pCurrent->pSubString->match(&_Name);

		if(dwEqual)
		{
			_Name.m_pcString = _Name.m_pcString+dwEqual;
			_Name.m_dwLen -= dwEqual;

			// Baumknoten länger als Name, aber gleichen Anfangsteil
			if(pCurrent->pSubString->m_dwLen > dwEqual)
				return pCurrent;

			// 1.Fall: Name zu Ende und Fund.
			if(!_Name.m_dwLen) 
			{
				if(pCurrent->pData) return pCurrent;
				else{
					_Name.m_pcString = pcLastPossibleName;
					_Name.m_dwLen = dwLastPossibleLen;
					// Fehler: aktueller Knoten enthält keine Daten
					//OrSE_Error("Funktionsname enthält keine Daten.");
					return pLastPossibleData;
				}
			}

			// 2.Fall: Name geht weiter -> Subbaum durchsuchen.
			// Wenn der aktuelle Knoten daten enthält merken.
			if(pCurrent->pData)
			{
				pLastPossibleData = pCurrent;
				pcLastPossibleName = _Name.m_pcString;
				dwLastPossibleLen = _Name.m_dwLen;
			}
			pCurrent = pCurrent->pChild;
		} else
			// Auf dieser Ebene Suche Fortführen
			pCurrent = pCurrent->pNext;
	}
	// kein Fund
	assert(_Name.m_pcString == pcLastPossibleName);
	//(*_ppcName) = pcLastPossibleName;
	return pLastPossibleData;
}

// ******************************************************************** //
void OrE::ADT::Trie::Remove(TrieString& _Name, void* _pData)
{
	// Daten freigeben egal was passiert. Die Speicheradresse benötigen wir
	// aber noch zum vergleichen.
	if(_pData && m_pDeleteCallback) m_pDeleteCallback(_pData);

	// Ähnlich dem Suchen oder einfügen den Baum durchgehen.
	// Jeden Knoten, der auf die Daten zeigt markieren (Datenpointer löschen,
	// Zeiger auf Knoten auf den Stack packen.
	// Immer wenn eine Abhängigkeit eines Knotens zu anderen Namen/Daten
	// detektiert wird den Stack leeren.
	int iNumStacked = 0;
	// Am Ende alle auf dem Stack befindlichen Knoten löschen.

	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	TrieNodeP pCurrent = m_pFirstNode;
	TrieNodeP pPrevious = nullptr;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=pCurrent->pSubString->match(&_Name);

		if(dwEqual)
		{
			// Fehler, wenn Baumknoten länger als Name, aber gleichen Anfangsteil
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
					__asm push pPrevious;
					__asm push pCurrent;
					++iNumStacked;		// Ein potentiell zu löschender Knoten mehr
					// Wenn nichts mehr von diesem Knoten abhängt diesen und alle auf dem Stack löschen
					if(!pCurrent->pChild)
					{
						for(int i=0;i<iNumStacked;++i)
						{
							// Hole Knoten und Elternelement vom Stack
							__asm pop pCurrent;
							__asm pop pPrevious;
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
						iNumStacked = 0;
					}
					break;
				} else {
					// Aktueller Knoten enthält falsche Daten
					break;
				}
			}

			// 2.Fall: Name geht weiter -> Subbaum durchsuchen.
			// Wenn der aktuelle Knoten gleiche oder nulldaten hat merken.
			// Wenn Knoten unabhängig auf den Stack, sonst Stack leeren
			if(pCurrent->pData == _pData || !pCurrent->pData)
			{
				pCurrent->pData = nullptr;
				__asm push pPrevious;
				__asm push pCurrent;
				++iNumStacked;		// Ein potentiell zu löschender Knoten mehr
			} else
			{
				// Daten in den Skat drücken
				void* pDummy;
				for(int i=0;i<iNumStacked*2;++i) __asm pop pDummy;
				iNumStacked = 0;	// Neubeginn
			}
			pPrevious = pCurrent;
			_Name.m_pcString = &_Name.m_pcString[dwEqual];
			_Name.m_dwLen -= dwEqual;
			pCurrent = pCurrent->pChild;
		} else {
			assert(pCurrent->pData != _pData);
			// Auf dieser Ebene Suche Fortführen
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
	// kein Fund -> Stack aufräumen (_pData nur als sinnlos puffer misbrauchen
	for(int i=0;i<iNumStacked*2;++i) __asm pop _pData;
}

// ******************************************************************** //
void OrE::ADT::Trie::ReleaseTrie(TrieNodeP _pNode)
{
	// Rekursionsstart im obersten Knoten
	if(!_pNode)
	{
		if(!m_pFirstNode) return;
		_pNode = m_pFirstNode;
		m_pFirstNode = 0;
	}

	// Informationen über weitere Knoten verwenden.
	if(_pNode->pChild) ReleaseTrie(_pNode->pChild);
	if(_pNode->pNext) ReleaseTrie(_pNode->pNext);

	if(_pNode->pData && m_pDeleteCallback)
		m_pDeleteCallback(_pNode->pData);

	// jetzt brauchen wir den Knoten nicht mehr
	delete _pNode->pSubString;
	delete _pNode;
}

// ******************************************************************** //