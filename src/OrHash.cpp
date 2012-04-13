// ******************************************************************************** //
// OrHash.cpp																		//
// ==========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project. You can do this by a comment at	https://github.com/Jojendersie/.		//
// Futhermore you have to state this project as a source of your project.			//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrFastMath.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"

#include <stdlib.h>
#include <string.h>

using namespace OrE::Algorithm;
using namespace OrE::ADT;
using namespace OrE::Math;

// ******************************************************************************** //
// erstellt einen 32 Bit-Hashwert aus einem Datensatz
dword OrE::Algorithm::CreateHash32(const void* pData, const int iSize)
{
	qword qwHash = CreateHash64(pData, iSize);
	return ((dword)qwHash) ^ ((dword)(qwHash >> 32));
}

// ******************************************************************************** //
// erstellt einen 64 Bit-Hashwert aus einem Datensatz
qword OrE::Algorithm::CreateHash64(const void* pData, const int iSize)
{
	qword qwHash = 0x84222325cbf29ce4ULL;

	byte* pFirst = (byte*)(pData),
		* pLast  = pFirst + iSize;

	while( pFirst < pLast )
	{
		qwHash ^= *pFirst++;
		qwHash *= 0x00000100000001b3ULL;
	}

	return qwHash;
}

// ******************************************************************************** //
// erstellt einen 32 Bit-Hashwert aus einem nullterminierten Datensatz
dword OrE::Algorithm::CreateHash32(const void* pData)
{
	return ((dword)CreateHash64(pData)) ^ (((dword)CreateHash64(pData) >> 16) >> 16);
}

// ******************************************************************************** //
// erstellt einen 64 Bit-Hashwert aus einem nullterminierten Datensatz
qword OrE::Algorithm::CreateHash64(const void* pData)
{
	qword qwHash = 0x84222325cbf29ce4ULL;

	byte* pFirst = (byte*)(pData);

	while( *pFirst != 0 )	// Abbruchbedingung ein Nullbyte
	{
		qwHash ^= *pFirst++;
		qwHash *= 0x00000100000001b3ULL;
	}

	return qwHash;
}

// ******************************************************************************** //
// CRC - Fehlerüberprüfender Hash
// Erzeugt einen CRC Hash mit beliebigen Polynom bis zum Grad 31
dword OrE::Algorithm::CreateCRCHash(dword dwPolynom, void* pData, dword dwSize)
{
	dword dwCRC = 0;	// Das Schieberegister
	for(dword i=0; i<dwSize; ++i)
	{
		// Jedes Bit einzeln durchgehen
		for(dword j=7; j<8; --j)
		{
			// Erstes Bit mit dem Currentbit vergleichen
			if((dwCRC>>31) != (dword)((((byte*)pData)[i]>>j) & 1))
				dwCRC = (dwCRC << 1) ^ dwPolynom;
			else
				dwCRC <<= 1;
		}
	}

	return dwCRC;
}


// ******************************************************************************** //
// Hashmap																			//
// ******************************************************************************** //

// ******************************************************************************** //
// Die für alle Namen verwendete Hash-Funktion
static dword OrStringHash(const char* _pcString)
{
	/*dword dwHash = 5381;

	while(int c = *_pcString++)
		dwHash = ((dwHash << 5) + dwHash) + c; // dwHash * 33 + c
		// Alternativ dwHash * 33 ^ c

	return dwHash;//*/
	dword dwHash = 208357;

	while(int c = *_pcString++)
		dwHash = ((dwHash << 5) + (dwHash << 1) + dwHash) ^ c; 

	return dwHash;//*/
	/*dword dwHash = 208357;

	while(int c = *_pcString++)
		dwHash = (dwHash*17) ^ c + ~(dwHash >> 7) + (c >> 3);

	return dwHash;//*/
}

// ******************************************************************************** //
void OrE::ADT::HashMap::RecursiveReAdd(Bucket* _pBucket)
{
	// During resize it is nessecary to re-add every thing into the new copie
	if(_pBucket->pObject) Insert(_pBucket->pObject, _pBucket->qwKey);	// If the key contains a char* pointer it is unique and not touched through reinsertion
	if(_pBucket->pLeft) RecursiveReAdd(_pBucket->pLeft);
	if(_pBucket->pRight) RecursiveReAdd(_pBucket->pRight);
}

// ******************************************************************************** //
// Initialisierung mit Startgröße
OrE::ADT::HashMap::HashMap(dword _dwSize, HashMapMode _Mode)
{
	// Einfach nur Speicher bestellen für eine leere Tabelle
	m_pBuckets = (Bucket*)malloc(sizeof(Bucket)*_dwSize);
	if(!m_pBuckets) return;	// TODO report error
	memset(m_pBuckets, 0, sizeof(Bucket)*_dwSize);
	// Statische Größen setzen
	m_dwSize = _dwSize;
	m_dwNumElements = 0;
	m_Mode = _Mode;
}

// ******************************************************************************** //
// Daten löschen
void OrE::ADT::HashMap::RemoveData(BucketP _pBucket)
{
	if(m_pDeleteCallback && _pBucket->pObject) m_pDeleteCallback(_pBucket->pObject);
	if(m_Mode & HM_USE_STRING_MODE)
	{
		char* pcString = (char*)(_pBucket->qwKey>>32);
		if(pcString) free(pcString);
	}
}

// ******************************************************************************** //
// Freigeben alles Ressourcen
void OrE::ADT::HashMap::RecursiveRelease(BucketP _pBucket)
{
	// Daten löschen
	RemoveData(_pBucket);
	// Rekursion
	if(_pBucket->pLeft) RecursiveRelease(_pBucket->pLeft);
	if(_pBucket->pRight) RecursiveRelease(_pBucket->pRight);
	// Eimer selbst löschen (nicht Teil der Liste, sondern eines Binären Baums.
	delete _pBucket;
}

// ******************************************************************************** //
OrE::ADT::HashMap::~HashMap()
{
	// Alle Eimer freigeben. Dazu alle Daten traversieren und löschen.
	for(dword i=0;i<m_dwSize;++i)
	{
		// Daten löschen
		RemoveData(m_pBuckets + i);
		// Rekursionsstart
		if(m_pBuckets[i].pLeft) RecursiveRelease(m_pBuckets[i].pLeft);
		if(m_pBuckets[i].pRight) RecursiveRelease(m_pBuckets[i].pRight);
	}

	// Die Tabelle selbst löschen
	free(m_pBuckets);
	m_pBuckets = 0;
}

// ******************************************************************************** //
// Tabelle neu erzeugen und alle Elemente neu hinzufügen
void OrE::ADT::HashMap::Resize(const dword _dwSize)
{
	Bucket* pOldList = m_pBuckets;
	dword dwOldSize = m_dwSize;
	// Einfach nur Speicher bestellen für eine leere Tabelle
	m_pBuckets = (Bucket*)malloc(sizeof(Bucket)*_dwSize);
	if(!m_pBuckets) return;	// TODO report error
	memset(m_pBuckets, 0, sizeof(Bucket)*_dwSize);
	// Statische Größen setzen
	m_dwSize = _dwSize;
	m_dwNumElements = 0;
	
	// Im Falle einer nicht initialisierten Hashmap initialisiert
	// ReSize einfach die Größe. Der Re-add-Teil ist dann nutzlos/falsch.
	if(pOldList)
	{
		// Alles erneut einfügen
		for(dword i=0;i<dwOldSize;++i)
		{
			if(pOldList[i].pObject) Insert(pOldList[i].pObject, pOldList[i].qwKey);
			if(pOldList[i].pLeft) RecursiveReAdd(pOldList[i].pLeft);
			if(pOldList[i].pRight) RecursiveReAdd(pOldList[i].pRight);
		}

		// Alte Liste löschen
		free(pOldList);
	}
}

// ******************************************************************************** //
// Checks if a resize is required in the current mode
void OrE::ADT::HashMap::TestSize()
{
	// Zur Verbesserung der Performance dynamisch erweitern (verdoppeln?)
	switch(m_Mode & 3)
	{
		// Langsames Wachstum (sqrt); Mittlere Kollisionszahl; Häufiges Resize
		case HM_PREFER_SIZE: 
			if(m_dwNumElements >= m_dwSize*3)
				Resize(m_dwSize+3*(dword)Sqrt((float)m_dwSize));
			break;
		// Mittleres Wachstum (sqrt/linear); Mittlere Kollisionszahl; Häufiges Resize
		case HM_RESIZE_MODERATE:
			if(m_dwNumElements >= (dword)(m_dwSize*1.5f))
				Resize(m_dwSize+Max(6*(dword)Sqrt((float)m_dwSize), (dword)128));
			break;
		// Schnelles Wachstum (exponentiell); Geringe Kollisionszahl; Seltenes Resize
		case HM_PREFER_PERFORMANCE:
			if(m_dwNumElements >= m_dwSize)
				Resize((dword)((m_dwSize+100)*1.5f));
			break;
	}
}

// ******************************************************************************** //
// Standard operation insert
ADTElementP OrE::ADT::HashMap::Insert(void* _pObject, qword _qwKey)
{
#ifdef DEBUG
	if(!m_pBuckets) return nullptr;		// TODO report error
#endif
#ifdef _DEBUG
	int iCollision = 0;
#endif

	TestSize();

	if(m_Mode & HM_USE_STRING_MODE)
		_qwKey &= 0xffffffff;
	
	// Neues Element einsortieren
	dword dwHash = _qwKey%m_dwSize;
	if(m_pBuckets[dwHash].pObject)
	{
		Bucket* pBucket = &m_pBuckets[dwHash];
		while(true)
		{
#ifdef _DEBUG
			++iCollision;
#endif
			// Schlüssel vollständig verlgeichen -> Baumsuche
			if(_qwKey < pBucket->qwKey)
				if(pBucket->pLeft) pBucket = pBucket->pLeft;	// Traversieren
				else {pBucket->pLeft = new Bucket(_pObject, _qwKey, pBucket); ++m_dwNumElements; return pBucket->pLeft;}
			else if(_qwKey > pBucket->qwKey)
				if(pBucket->pRight) pBucket = pBucket->pRight;	// Traversieren
				else {pBucket->pRight = new Bucket(_pObject, _qwKey, pBucket); ++m_dwNumElements; return pBucket->pRight;}
			else {pBucket->AddRef(); return pBucket;}			// Mehr oder weniger ein Fehler, aber der Datensatz existiert schon (wird überschrieben)
		}
	} else
	{
		m_pBuckets[dwHash].pObject = _pObject;
		m_pBuckets[dwHash].qwKey = _qwKey;
		++m_dwNumElements;
		return &m_pBuckets[dwHash];
	}
}

// ******************************************************************************** //
// Standard operation delete
void OrE::ADT::HashMap::Delete(qword _qwKey)
{
	Delete(Search(_qwKey));
}

// ******************************************************************************** //
// Faster operation delete (no search)
void OrE::ADT::HashMap::Delete(ADTElementP _pElement)
{
	if(!_pElement) return;

	// Repariere Baum
	// Wenn _pElement Kinder hat suche nach einer Ersetzung und lösche
	// stattdessen diesen Knoten.
	if((BucketP(_pElement))->pLeft)
	{
		// Es existiert ein linker Teilbaum -> maximum davon erhält Baumeigenschaft
		BucketP pBuck = (BucketP(_pElement))->pLeft;
		while(pBuck->pRight) pBuck = pBuck->pRight;
		_pElement->pObject = pBuck->pObject;
		_pElement->qwKey = pBuck->qwKey;
		Delete(pBuck);
	} else if((BucketP(_pElement))->pRight)
	{
		// Es existiert ein rechter Teilbaum -> minimum davon erhält Baumeigenschaft
		BucketP pBuck = (BucketP(_pElement))->pRight;
		while(pBuck->pLeft) pBuck = pBuck->pLeft;
		_pElement->pObject = pBuck->pObject;
		_pElement->qwKey = pBuck->qwKey;
		Delete(pBuck);
	} else
	{
		// Listenelement?
		if(!(BucketP(_pElement))->pParent)
		{
			// Daten löschen
			RemoveData((BucketP)_pElement);
			// Inhalt auf 0 setzen
			_pElement->pObject = nullptr;
			_pElement->qwKey = 0;
		} else
		{
			// Referencen auf diesen Teil des Eimers entfernen
			if(BucketP(_pElement)->pParent->pLeft == _pElement)
				BucketP(_pElement)->pParent->pLeft = nullptr;
			else
				BucketP(_pElement)->pParent->pRight = nullptr;

			// Das Element hat keine Kinder mehr (und wenn dann würden wir die Referenz verlieren)
			// Daher kann es gefahrlos mit dem Standard freigegeben werden
			RecursiveRelease((BucketP)_pElement);
		}
		// Jetzt ist es definitiv weg
		--m_dwNumElements;
	}
}

// ******************************************************************************** //
// Standard search with a key
ADTElementP OrE::ADT::HashMap::Search(qword _qwKey)
{
	// Listeneintrag?
	dword dwHash = (m_Mode & HM_USE_STRING_MODE) ? (_qwKey&0xffffffff)%m_dwSize : _qwKey%m_dwSize;
	Bucket* pBucket = &m_pBuckets[dwHash];
	if(!pBucket->pObject) return 0;
	// Baumsuche
	while(pBucket)
	{
		if(_qwKey==pBucket->qwKey) return pBucket;
		else if(_qwKey<pBucket->qwKey) pBucket = pBucket->pLeft;
		else pBucket = pBucket->pRight;
	}
	return 0;
}

// ******************************************************************************** //
// TODO equality for data is other then in the upper date (last step) -> everything reimplement
// String-Mode functions
// insert using strings
ADTElementP OrE::ADT::HashMap::Insert(void* _pObject, const char* _pcKey)
{
	if(!(m_Mode & HM_USE_STRING_MODE)) return nullptr;
	#ifdef DEBUG
	if(!m_pBuckets) return nullptr;		// TODO report error
	int iCollision = 0;
#endif

	TestSize();
	
	// Neues Element einsortieren
	dword dwH = OrStringHash(_pcKey);
	dword dwHash = dwH%m_dwSize;
	if(m_pBuckets[dwHash].pObject)
	{
		Bucket* pBucket = &m_pBuckets[dwHash];
		bool bAdded = false;
		while(!bAdded)
		{
#ifdef DEBUG
			++iCollision;
#endif
			// String vollständig verlgeichen -> Baumsuche
			char* pcBucketString = (char*)(pBucket->qwKey>>32);
			// Im Stringmodus wird der Zeiger auf den gesamten string in der höheren 32 Bit
			// Word gespeichert. Es können auch Elemente ohne Strings vorkommen. Da aber
			// im Bucket (BinTree) der gesamte 64Bit Wert das Kriterium bildet liegen alle mit
			// Strings weiter rechts.
			if(!pcBucketString) 
			{
				if(pBucket->pRight)
					pBucket = pBucket->pRight;
				else {pBucket->pRight = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pRight; bAdded = true;}
			} else
			{
				int iCmp = strcmp(_pcKey,pcBucketString);
				if(iCmp < 0)
					if(pBucket->pLeft) pBucket = pBucket->pLeft;	// Traversieren
					else {pBucket->pLeft = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pLeft; bAdded = true;}
				else if(iCmp > 0)
					if(pBucket->pRight) pBucket = pBucket->pRight;	// Traversieren
					else {pBucket->pRight = new Bucket(_pObject, 0, pBucket); pBucket = pBucket->pRight; bAdded = true;}
				else {pBucket->AddRef(); return pBucket;}			// Mehr oder weniger ein Fehler, aber der Datensatz existiert schon (wird überschrieben)
			}
		}
		// Schleife wird nur verlassen, wenn wir in einem neuen Knoten sind
		// Es muss nun noch eine Stringkopie erstellt werden.
		int iLen = (int)strlen(_pcKey)+1;
		void* pStr = malloc(iLen);
		memcpy(pStr, _pcKey, iLen);
		pBucket->qwKey = ((((qword)pStr)<<16)<<16) | dwH;
		++m_dwNumElements;
		return pBucket;
	} else
	{
		m_pBuckets[dwHash].pObject = _pObject;
		// Stringkopie
		int iLen = (int)strlen(_pcKey)+1;
		void* pStr = malloc(iLen);
		memcpy(pStr, _pcKey, iLen);
		m_pBuckets[dwHash].qwKey = ((((qword)pStr)<<16)<<16) | dwH;
		++m_dwNumElements;
		return &m_pBuckets[dwHash];
	}

#ifdef _DEBUG
	//return iCollision;
#endif
}

// ******************************************************************************** //
// Standard operation delete for strings
void OrE::ADT::HashMap::Delete(const char* _pcKey)
{
	if(m_Mode & HM_USE_STRING_MODE)
		Delete(Search(_pcKey));
}

// ******************************************************************************** //
// search using strings
ADTElementP OrE::ADT::HashMap::Search(const char* _pcKey)
{
	if(!(m_Mode & HM_USE_STRING_MODE)) return nullptr;
	// Listeneintrag?
	dword dwHash = OrStringHash(_pcKey)%m_dwSize;
	Bucket* pBucket = &m_pBuckets[dwHash];
	if(!pBucket->pObject) return 0;
	// Baumsuche
	while(pBucket)
	{
		char* pcBucketString = (char*)((pBucket->qwKey>>16)>>16);
		if(pcBucketString)
		{
			int iCmp = strcmp(_pcKey,pcBucketString);
			if(iCmp==0) return pBucket;
			else if(iCmp<0) pBucket = pBucket->pLeft;
			else pBucket = pBucket->pRight;
		} else pBucket = pBucket->pRight;
	}
	return 0;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetFirst()
{
	for(dword i=0;i<m_dwSize;++i)
		// Wenn die Stelle im Array leer ist einfach überspringen.
		if(m_pBuckets[i].pObject)
		{
			Bucket* pBuck = m_pBuckets+i;
			// Kleinstes Element
			while(pBuck->pLeft) pBuck = pBuck->pLeft;
			return pBuck;
		}

	return nullptr;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetLast()
{
	for(dword i=m_dwSize-1;i>=0;--i)
		// Wenn die Stelle im Array leer ist einfach überspringen.
		if(m_pBuckets[i].pObject)
		{
			BucketP pBuck = m_pBuckets+i;
			// Größtes Element
			while(pBuck->pRight) pBuck = pBuck->pRight;
			return pBuck;
		}

	return nullptr;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetNext(ADTElementP _pCurrent)
{
	BucketP pBuck = (BucketP)_pCurrent;
	// In Eimern Baumnavigation
	// Inorder traverse -> left site was seen before
	if(pBuck->pRight) 
	{
		pBuck = pBuck->pRight;
		while(pBuck->pLeft) pBuck = pBuck->pLeft;
		return pBuck;
	} else if(pBuck->pParent) {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		while(pBuck->pParent->pRight == pBuck) 
		{
			pBuck = pBuck->pParent;
			if(!pBuck->pParent) goto ListSearch;
		}
		return pBuck->pParent;
	}

ListSearch:
	dword dwIndex = (dword)(pBuck - m_pBuckets);
	++dwIndex;
	if(dwIndex == m_dwSize) return nullptr;	// Am Ende gibt es kein nächstes Element
	while(!m_pBuckets[dwIndex].pObject)
		if(++dwIndex == m_dwSize) return nullptr;	// Am Ende gibt es kein nächstes Element
	// Minimales Element im Eimer
	pBuck = m_pBuckets + dwIndex;
	while(pBuck->pLeft) pBuck = pBuck->pLeft;
	return pBuck;
}

// ******************************************************************************** //
ADTElementP OrE::ADT::HashMap::GetPrevious(ADTElementP _pCurrent)
{
	BucketP pBuck = (BucketP)_pCurrent;
	// In Eimern Baumnavigation
	// Inorder traverse -> left site was seen before
	if(pBuck->pLeft) 
	{
		pBuck = pBuck->pLeft;
		while(pBuck->pRight) pBuck = pBuck->pRight;
		return pBuck;
	} else if(pBuck->pParent) {
		// With no right child we have to move to the parent. We could have seen this,
		// if we are in the right branch now. Then we have to move much more upwards
		// until we come back from an left branch.
		while(pBuck->pParent->pLeft == pBuck) 
		{
			pBuck = pBuck->pParent;
			if(!pBuck->pParent) goto ListSearch;
		}
		return pBuck->pParent;
	}

ListSearch:
	dword dwIndex = pBuck - m_pBuckets;
	--dwIndex;
	if(dwIndex == 0xffffffff) return nullptr;	// Am Ende gibt es kein nächstes Element
	while(!m_pBuckets[dwIndex].pObject)
		if(--dwIndex == 0xffffffff) return nullptr;	// Am Ende gibt es kein nächstes Element
	// Maximales Element im Eimer
	pBuck = m_pBuckets + dwIndex;
	while(pBuck->pRight) pBuck = pBuck->pRight;
	return pBuck;
}

// *************************************EOF**************************************** //