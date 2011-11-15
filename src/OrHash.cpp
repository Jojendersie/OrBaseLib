#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"

#include <stdlib.h>
#include <string.h>

// ******************************************************************************** //
// erstellt einen 32 Bit-Hashwert aus einem Datensatz
dword OrCreateHash32(const void* pData, const int iSize)
{
	qword qwHash = OrCreateHash64(pData, iSize);
	return ((dword)qwHash) ^ ((dword)(qwHash >> 32));
}

// ******************************************************************************** //
// erstellt einen 64 Bit-Hashwert aus einem Datensatz
qword OrCreateHash64(const void* pData, const int iSize)
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
dword OrCreateHash32(const void* pData)
{
	return ((dword)OrCreateHash64(pData)) ^ ((dword)OrCreateHash64(pData) >> 32);
}

// ******************************************************************************** //
// erstellt einen 64 Bit-Hashwert aus einem nullterminierten Datensatz
qword OrCreateHash64(const void* pData)
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
dword OrCreateCRCHash(dword dwPolynom, void* pData, dword dwSize)
{
	dword dwCRC = 0;	// Das Schieberegister
	for(dword i=0; i<dwSize*8; ++i)
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

// ******************************************************************** //
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
// TODO
void OrHashMap::RecursiveReAdd(OrBucket* _pBucket)
{
	// During resize it is nessecary to re-add every thing into the new copie
	if(_pBucket->pObject) Insert(_pBucket->pObject, _pBucket->qwKey);	// If the key contains a char* pointer it is unique and not touched through reinsertion
	if(_pBucket->pLeft) RecursiveReAdd(_pBucket->pLeft);
	if(_pBucket->pRight) RecursiveReAdd(_pBucket->pRight);
}

// ******************************************************************************** //
// TODO
OrHashMap::OrHashMap(dword _dwSize, OrHashMapResizeMode _Mode, bool _bUsingStringMode)
{
}

// ******************************************************************************** //
// TODO
OrHashMap::~OrHashMap()
{
	// Alle Eimer freigeben. Dazu alle Daten traversieren und löschen.
	for(dword i=0;i<m_dwSize;++i)
	{
		if(m_pDeleteCallback && m_pBuckets[i].pObject) m_pDeleteCallback(m_pBuckets[i].pObject);
		if(m_pBuckets[i].pLeft) RecursiveRelease(m_pBuckets[i].pLeft);
		if(m_pBuckets[i].pRight) RecursiveRelease(m_pBuckets[i].pRight);
	}

	// Die Tabelle selbst löschen
	free(m_pBuckets);
	m_pBuckets = 0;
}

// ******************************************************************************** //
// TODO
// Tabelle neu erzeugen und alle Elemente neu hinzufügen
void OrHashMap::Resize(const dword _dwSize)
{
	OrBucket* pOldList = m_pBuckets;
	dword dwOldSize = m_dwSize;
	// Einfach nur Speicher bestellen für eine leere Tabelle
	m_pBuckets = (OrBucket*)malloc(sizeof(OrBucket)*_dwSize);
	if(!m_pBuckets) return;	// TODO report error
	memset(m_pBuckets, 0, sizeof(OrBucket)*_dwSize);
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
// TODO
// Standard operation insert
OrADTElementP OrHashMap::Insert(void* _pObject, qword _qwKey)
{
}

// ******************************************************************************** //
// TODO
// Standard operation delete
void OrHashMap::Delete(qword _qwKey)
{
}

// ******************************************************************************** //
// TODO
// Faster operation delete (no search)
void OrHashMap::Delete(OrADTElementP _pElement)
{
}

// ******************************************************************************** //
// TODO
// Standard search with a key
OrADTElementP OrHashMap::Search(qword _qwKey)
{
}

// ******************************************************************************** //
// TODO equality for data is other then in the upper date (last step) -> everything reimplement
// String-Mode functions
// insert using strings
OrADTElementP OrHashMap::Insert(void* _pObject, const char* _pcKey)
{
}

// ******************************************************************************** //
// search using strings
// TODO
OrADTElementP OrHashMap::Search(const char* _pcKey)
{
}

// ******************************************************************************** //
// TODO
OrADTElementP OrHashMap::GetFirst()
{
}

// ******************************************************************************** //
// TODO
OrADTElementP OrHashMap::GetLast()
{
}

// ******************************************************************************** //
// TODO
OrADTElementP OrHashMap::GetNext(OrADTElementP _pCurrent)
{
}

// ******************************************************************************** //
// TODO
OrADTElementP OrHashMap::GetPrevious(OrADTElementP _pCurrent)
{
}

// *************************************EOF**************************************** //