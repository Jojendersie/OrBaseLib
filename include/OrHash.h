// ******************************************************************************** //
// Implementing some hash functions and a hash map.									//
//																					//
// OrHashMap ( OR_HM_PREFER_PERFORMANCE ):											//
//	Insert()					O(1) amort.											//
//	Search()					O(1) amort.											//
//	Delete(Key)					O(1) amort.											//
//	Delete(Element)				O(1) amort.											//
//	GetFirst()					O(1) amort.											//
//	GetLast()					O(1) amort.											//
//	GetNext()					O(1) amort.											//
//	GetPrevious()				O(1) amort.											//
//	In other modes than OR_HM_PREFER_PERFORMANCE only the speed of Insert() differ	//
//	in asymptoticly view. Insert()	O(sqrt(n)) amort.								//
//	All other methods get slightly bigger constant factors.							//
//																					//
// OrHashMap - String-Mode															//
// Im Stringmodus k�nnen Objekte �ber Schl�ssel oder Strings hinzugef�gt werden.	//
// Die Schl�ssel nutzen dann aber nur 32 Bit! Die anderen 32 Bit speichern eine		//
// Stringcopy zum vergleichen auf korrekten match.									//
// Nur die letzten 32 Bit (least significant) k�nnen als Schl�ssel angegeben werden!//
// ******************************************************************************** //

#pragma once

// ******************************************************************************** //
// Die Standart Hashmethoden
dword OrCreateHash32(const void* pData, const int iSize);	// erstellt einen 32 Bit-Hashwert aus einem Datensatz
qword OrCreateHash64(const void* pData, const int iSize);	// erstellt einen 64 Bit-Hashwert aus einem Datensatz
dword OrCreateHash32(const void* pData);					// erstellt einen 32 Bit-Hashwert aus einem nullterminierten Datensatz
qword OrCreateHash64(const void* pData);					// erstellt einen 64 Bit-Hashwert aus einem nullterminierten Datensatz

// ******************************************************************************** //
// CRC - Fehler�berpr�fender Hash
// Die Funktionen ermitteln den Rest bei der Polynomdivision. Dieser
// muss zur Erzeugung den Daten angeh�ngt werden. Beim �berpr�fen
// muss nochmalige Anwendung der Funktion den Rest 0 liefern, ansonsten
// wurden die Daten besch�digt.
// Erzeugt einen CRC Hash mit beliebigen Polynom bis zum Grad 31
dword OrCreateCRCHash(dword dwPolynom, void* pData, dword dwSize);

// Verwendung des Defaultpolynoms: x32? + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
inline dword OrCreateCRC32IEEEHash(void* pData, dword dwSize)	{return OrCreateCRCHash(0x04C11DB7, pData, dwSize);}

// Verwendung des Defaultpolynoms: x24 + x23 + x18 + x17 + x14 + x11 + x10 + x7 + x6 + x5 + x4 + x3 + x + 1
inline dword OrCreateCRC24RadixHash(void* pData, dword dwSize)	{return OrCreateCRCHash(0x864CFB, pData, dwSize);}

// Verwendung des Defaultpolynoms: x16+x12+x5+1
inline dword OrCreateCRCCCITT16Hash(void* pData, dword dwSize)	{return OrCreateCRCHash(0x00011022, pData, dwSize);}

// Verwendung des Defaultpolynoms: x16+x15+x2+1
inline dword OrCreateCRC16Hash(void* pData, dword dwSize)	{return OrCreateCRCHash(0x00018006, pData, dwSize);}

// ******************************************************************************** //
enum OrHashMapMode
{
	OR_HM_NO_RESIZE = 0,			// Konstante Gr��e (keine Autoresize beim Hinzuf�gen); Hohe Kollisionszahl
	OR_HM_PREFER_SIZE = 1,			// Langsames Wachstum (sqrt); Mittlere Kollisionszahl; H�ufiges Resize
	OR_HM_RESIZE_MODERATE = 2,		// Mittleres Wachstum (sqrt/linear); Mittlere Kollisionszahl; H�ufiges Resize
	OR_HM_PREFER_PERFORMANCE = 3,	// Schnelles Wachstum (exponentiell); Geringe Kollisionszahl; Seltenes Resize
	OR_HM_USE_STRING_MODE = 8		// Special mode for strings as keys
};

// ******************************************************************************** //
// Hashmap - Datenstruktur zum Schnellen finden von Elementen.
struct OrBucket: public OrADTElement
{
	OrBucket(void* _pObj, const qword& _qwKey, OrBucket* _pParent) :
			OrADTElement(_pObj, _qwKey),
			pLeft(nullptr),
			pRight(nullptr),
			pParent(_pParent) {}
private:
	OrBucket* pLeft;
	OrBucket* pRight;
	OrBucket* pParent;

	friend class OrHashMap;
};
typedef OrBucket* OrBucketP;

// ******************************************************************************** //
class OrHashMap: public OrADT
{
	OrBucketP			m_pBuckets;				// Ein Array mit Eimern (Bin-Trees)
	dword				m_dwSize;				// Gr��e des Arrays
	dword				m_dwNumElements;		// Anzahl Elemente in der Hashmap (kann gr��er als Arraygr��e sein)
	OrHashMapMode		m_Mode;					// Resizemodus und optinal String Modus

	void RemoveData(OrBucketP _pBucket);
	void RecursiveReAdd(OrBucketP _pBucket);
	void RecursiveRelease(OrBucketP _pBucket);
	void TestSize();							// Checks if a resize is required in the current mode
public:
	OrHashMap(dword _dwSize, OrHashMapMode _Mode);
	~OrHashMap();

	void Resize(const dword _dwSize);								// Tabelle neu erzeugen und alle Elemente neu hinzuf�gen

	OrADTElementP Insert(void* _pObject, qword _qwKey) override;	// Standard operation insert; If already exists the object is NOT overwritten, but referenccounter is increased
	void Delete(qword _qwKey) override;								// Standard operation delete
	void Delete(OrADTElementP _pElement) override;					// Faster operation delete (no search)
	OrADTElementP Search(qword _qwKey) override;					// Standard search with a key

	// String-Mode functions
	OrADTElementP Insert(void* _pObject, const char* _pcKey);		// insert using strings; If already exists the object is NOT overwritten, but referenccounter is increased
	void Delete(const char* _pcKey);								// Standard operation delete for strings
	OrADTElementP Search(const char* _pcKey);						// search using strings

	OrADTElementP GetFirst() override;								// Erstes Objekt aus dem ersten nichtleeren Eimer
	OrADTElementP GetLast() override;
	OrADTElementP GetNext(OrADTElementP _pCurrent) override;
	OrADTElementP GetPrevious(OrADTElementP _pCurrent) override;
};
typedef OrHashMap* OrHashMapP;

// *************************************EOF**************************************** //