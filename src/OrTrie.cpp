/*************************************************************************

	OrTrie.cpp
	==========
	Diese Datei ist Teil der Orkteck-Script-Engine.

	Zweck:
	Ein Trie dient der effizienten Erfassung von W�rtern aus einem
	W�rterbuch.
	An Jedes Wort sind in diesem Spezialfall Funktionspointer gebunden.

	Autor: Johannes Jendersie
	Verwendete Quellen: AuD II Vorlesung;R�sner;Sommersemester 2010

*************************************************************************/

// include for sprintf_s
#include <stdio.h>
// include for strcmp
#include <string.h>
// include for malloc
#include <stdlib.h>

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrADTObjects.h"
#include "..\Include\OrTrie.h"

// ******************************************************************** //
// Robust auxiliary function to check if *p1 != *p2
bool strneq(const char* p1, const char* p2)
{
	if(p1==p2) return false;
	if(p1==0) return true;	// p2 muss ja !=p1 sein und ist !=0
	if(p2==0) return true;
	return strcmp(p1, p2)!=0;
}

// ******************************************************************** //
// Auxiliary function to create a copy of a subsrting.
// Parameter:
//	_dwFrom - 0-indexed imdex of first char to copy (inclusive)
//	_dwTo - 0-indexed imdex of last char to copy (inclusive)
//			or 0xffffffff to copy the whole postfix begining in _dwFrom
char* substr(const dword _dwFrom, dword _dwTo, const char* pcIn)
{
	// Spezialfall: String bis zum Ende
	if(_dwTo == 0xffffffff) _dwTo = strlen(pcIn)-1;

	// Definition: von 0 bis 0 bedeutet wir wollen ein Zeichen+schlie�ende 0
	char* pcRet = (char*)malloc(((2+_dwTo)-_dwFrom)*sizeof(char));
	dword i=_dwFrom;
	for(;i<=_dwTo;++i)
		pcRet[i-_dwFrom] = pcIn[i];

	pcRet[i-_dwFrom] = 0;

	return pcRet;
}

// ******************************************************************** //
int OrTrie::Add(char* _pcName, void* _pData, bool _bOverrideData)
{
	// Name merken, falls Fehler
	char* pcName = _pcName;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	OrTrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=0;
		// Bei Stringende oder ungleichet sofort anhalten
		while((pCurrent->pcSubString[dwEqual]==_pcName[dwEqual]) &&
			(_pcName[dwEqual]) && (pCurrent->pcSubString[dwEqual])) ++dwEqual;
		// Wenn min. ein Zeichen gleich, dann Unterbaum besuchen
		if(dwEqual)
		{
			// Index auf letzte g�ltige Stelle setzen
			dword dwI = dwEqual-1;
			// Fall 1: _pcName ist zu ende -> Funktion an diesen Knoten
			// anh�ngen. �berladen/�berschreiben wenn bereits Endknoten.
			if(_pcName[dwEqual]==0)
			{
				// 1.1 Der Knoten hat hier noch einen l�ngeren String, das
				// ist jetzt ung�ltig und wird abgetrennt.
				if(pCurrent->pcSubString[dwEqual])
				{
					// Neuen zwischenknoten erstellen.
					OrTrieNodeP pNew = new OrTrieNode;
					// String zerteilen und alten l�schen
					char* pcTemp = pCurrent->pcSubString;
					pNew->pcSubString = substr(dwEqual, 0xffffffff, pcTemp);
					pCurrent->pcSubString = substr(0, dwI, pcTemp);
					free(pcTemp);
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
						|| _bOverrideData) // Do not notify the override if _bOverrideData==true
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
			if(pCurrent->pcSubString[dwEqual]==0)
			{
				if(_bOverrideData) pCurrent->pData = _pData;
				if(pCurrent->pChild) {
					pCurrent = pCurrent->pChild;
					_pcName = &_pcName[dwEqual];
				} else {
					// Fall 3: Baum zu Ende -> neues Kind mit Restnamen.
					pCurrent->pChild = new OrTrieNode;
					pCurrent->pChild->pcSubString = substr(dwEqual, 0xffffffff, _pcName);
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
				OrTrieNodeP pNew_Name = new OrTrieNode;
				OrTrieNodeP pNew_Old = new OrTrieNode;
				pNew_Name->pcSubString = substr(dwEqual, 0xffffffff, _pcName);
				pNew_Name->pChild = 0;
				pNew_Name->pNext = pNew_Old;
				pNew_Name->pData = _pData;

				// String zerteilen und alten l�schen
				char* pcTemp = pCurrent->pcSubString;
				pNew_Old->pChild = pCurrent->pChild;
				pNew_Old->pcSubString = substr(dwEqual, 0xffffffff, pcTemp);
				pNew_Old->pNext = 0;
				pNew_Old->pData = pCurrent->pData;

				pCurrent->pChild = pNew_Name;
				pCurrent->pcSubString = substr(0, dwI, pcTemp);
				if(_bOverrideData) pCurrent->pData = _pData;
				else pCurrent->pData = 0;
				free(pcTemp);
				return 0;
			}
		} else {
			// keine Gleichheit -> im n�chsten Teilbaum suchen.
			if(pCurrent->pNext)
				pCurrent = pCurrent->pNext;
			else {
				// Baum zu Ende -> neuer Unterknoten.
				pCurrent->pNext = new OrTrieNode;
				pCurrent->pNext->pcSubString = substr(0, 0xffffffff, _pcName);
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
		m_pFirstNode = new OrTrieNode;
		m_pFirstNode->pcSubString = substr(0, 0xffffffff, _pcName);
		m_pFirstNode->pChild = 0;
		m_pFirstNode->pNext = 0;
		m_pFirstNode->pData = _pData;
		return 0;
	}

	return 2;
}

// ******************************************************************** //
OrTrieNodeP OrTrie::Match(const char** _ppcName)
{
	// Da gr��tm�gliche Teilfunktion gesucht einen Merker einf�hren.
	OrTrieNodeP pLastPossibleData = 0;
	const char* pcLastPossibleName = *_ppcName;
	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	OrTrieNodeP pCurrent = m_pFirstNode;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=0;
		// Bei Stringende oder ungleichet sofort anhalten
		while((pCurrent->pcSubString[dwEqual]==(*_ppcName)[dwEqual]) &&
			((*_ppcName)[dwEqual]) && (pCurrent->pcSubString[dwEqual])) ++dwEqual;

		if(dwEqual)
		{
			(*_ppcName) = (*_ppcName)+dwEqual;

			// Fehler, wenn Baumknoten l�nger als Name, aber gleichen Anfangsteil
			if(pCurrent->pcSubString[dwEqual])
			{
				(*_ppcName) = pcLastPossibleName;
				//OrSE_Error("Funktionsname zu kurz (unbekannter Teilname einer Funktion).");
				return pLastPossibleData;
			}

			// 1.Fall: Name zu Ende und Fund.
			if(!*(*_ppcName)) if(pCurrent->pData) return pCurrent;
			else{
				// Fehler: aktueller Knoten enth�lt keine Funktion
				//OrSE_Error("Funktionsname enth�lt keine Funktion.");
				return pLastPossibleData;
			}

			// 2.Fall: Name geht weiter -> Subbaum durchsuchen.
			// Wenn der aktuelle Knoten eine g�ltige Funktion ist merken.
			if(pCurrent->pData)
			{
				pLastPossibleData = pCurrent;
				pcLastPossibleName = (*_ppcName);
			}
			pCurrent = pCurrent->pChild;
		} else
			// Auf dieser Ebene Suche Fortf�hren
			pCurrent = pCurrent->pNext;
	}
	// kein Fund
	(*_ppcName) = pcLastPossibleName;
	return pLastPossibleData;
}

// ******************************************************************** //
void OrTrie::Remove(const char* _pcName, void* _pData)
{
	// Daten freigeben egal was passiert. Die Speicheradresse ben�tigen wir
	// aber noch zum vergleichen.
	if(_pData && m_pDeleteCallback) m_pDeleteCallback(_pData);

	// �hnlich dem Suchen oder einf�gen den Baum durchgehen.
	// Jeden Knoten, der auf die Daten zeigt markieren (Datenpointer l�schen,
	// Zeiger auf Knoten auf den Stack packen.
	// Immer wenn eine Abh�ngigkeit eines Knotens zu anderen Namen/Daten
	// detektiert wird den Stack leeren.
	int iNumStacked = 0;
	// Am Ende alle auf dem Stack befindlichen Knoten l�schen.

	// Bei der Wurzel geht die Suche los und iteriert durch den Baum
	OrTrieNodeP pCurrent = m_pFirstNode;
	OrTrieNodeP pPrevious = nullptr;
	while(pCurrent)
	{
		// Strings vergleichen
		dword dwEqual=0;
		// Bei Stringende oder ungleichet sofort anhalten
		while((pCurrent->pcSubString[dwEqual]==_pcName[dwEqual]) &&
			(_pcName[dwEqual]) && (pCurrent->pcSubString[dwEqual])) ++dwEqual;

		if(dwEqual)
		{
			// Fehler, wenn Baumknoten l�nger als Name, aber gleichen Anfangsteil
			if(pCurrent->pcSubString[dwEqual])
			{
				//OrSE_Error("Funktionsname zu kurz (unbekannter Teilname einer Funktion).");
				return;
			}

			// 1.Fall: Name zu Ende und Fund.
			if(!_pcName[dwEqual])
			{
				if(pCurrent->pData == _pData || !pCurrent->pData)
				{
					// Aktuellen Knoten auch noch auf den Stack packen (zur einheitlichen Verarbeitung)
					__asm push pPrevious;
					__asm push pCurrent;
					++iNumStacked;		// Ein potentiell zu l�schender Knoten mehr
					// Wenn nichts mehr von diesem Knoten abh�ngt diesen und alle auf dem Stack l�schen
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
							free(pCurrent->pcSubString);
							delete pCurrent;
						}
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
				__asm push pPrevious;
				__asm push pCurrent;
				++iNumStacked;		// Ein potentiell zu l�schender Knoten mehr
			} else
			{
				// Daten in den Skat dr�cken
				void* pDummy;
				for(int i=0;i<iNumStacked*2;++i) __asm pop pDummy;
				iNumStacked = 0;	// Neubeginn
			}
			pPrevious = pCurrent;
			pCurrent = pCurrent->pChild;
		} else {
			// Auf dieser Ebene Suche Fortf�hren
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
	// kein Fund -> Stack aufr�umen (_pData nur als sinnlos puffer misbrauchen
	for(int i=0;i<iNumStacked*2;++i) __asm pop _pData;
}

// ******************************************************************** //
void OrTrie::ReleaseTrie(OrTrieNodeP _pNode)
{
	// Rekursionsstart im obersten Knoten
	if(!_pNode)
	{
		_pNode = m_pFirstNode;
		m_pFirstNode = 0;
	}

	// Informationen �ber weitere Knoten verwenden.
	if(_pNode->pChild) ReleaseTrie(_pNode->pChild);
	if(_pNode->pNext) ReleaseTrie(_pNode->pNext);

	if(_pNode->pData && m_pDeleteCallback)
		m_pDeleteCallback(_pNode->pData);

	// jetzt brauchen wir den Knoten nicht mehr
	free(_pNode->pcSubString);
	delete _pNode;
}

// ******************************************************************** //