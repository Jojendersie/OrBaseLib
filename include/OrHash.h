// ******************************************************************************** //
// OrHash.h																			//
// ========																			//
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
//																					//
// ******************************************************************************** //
// Implementing some hash functions and a hash map.									//
//																					//
// HashMap ( HM_PREFER_PERFORMANCE ):												//
//	Insert()					O(1) amort.											//
//	Search()					O(1) amort.											//
//	Delete(Key)					O(1) amort.											//
//	Delete(Element)				O(1) amort.											//
//	GetFirst()					O(1) amort.											//
//	GetLast()					O(1) amort.											//
//	GetNext()					O(1) amort.											//
//	GetPrevious()				O(1) amort.											//
//	In other modes than HM_PREFER_PERFORMANCE only the speed of Insert() differ		//
//	in asymptoticly view. Insert()	O(sqrt(n)) amort.								//
//	All other methods get slightly bigger constant factors.							//
//																					//
// HashMap - String-Mode															//
// In the string mode the objects can added with strings as keys. These strings are	//
// copied internal and the reference is saved in the higher 32 Bit part of the key.	//
// It is still possible to add objects with other keys to the same map, but they	//
// can only use the lower 32 Bit part.												//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Algorithm {

// ******************************************************************************** //
// Standard hash for any data. Should result in nice uniform distributions mostly.
dword CreateHash32(const void* pData, const int iSize);	// create a 32 bit hash value of a data set
qword CreateHash64(const void* pData, const int iSize);	// create a 64 bit hash value of a data set
dword CreateHash32(const void* pData);					// create a 32 bit hash value of a 0-terminated data set (e.g. strings)
qword CreateHash64(const void* pData);					// create a 64 bit hash value of a 0-terminated data set (e.g. strings)

// ******************************************************************************** //
// CRC - Error proving hash (cyclic redundancy check)
// Die Funktionen ermitteln den Rest bei der Polynomdivision. Dieser
// muss zur Erzeugung den Daten angehängt werden. Beim Überprüfen
// muss nochmalige Anwendung der Funktion den Rest 0 liefern, ansonsten
// wurden die Daten beschädigt.
// Erzeugt einen CRC Hash mit beliebigen Polynom bis zum Grad 31
dword CreateCRCHash(dword dwPolynom, void* pData, dword dwSize);

// Verwendung des Defaultpolynoms: x32? + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
inline dword CreateCRC32IEEEHash(void* pData, dword dwSize)	{return CreateCRCHash(0x04C11DB7, pData, dwSize);}

// Verwendung des Defaultpolynoms: x24 + x23 + x18 + x17 + x14 + x11 + x10 + x7 + x6 + x5 + x4 + x3 + x + 1
inline dword CreateCRC24RadixHash(void* pData, dword dwSize)	{return CreateCRCHash(0x864CFB, pData, dwSize);}

// Verwendung des Defaultpolynoms: x16+x12+x5+1
inline dword CreateCRCCCITT16Hash(void* pData, dword dwSize)	{return CreateCRCHash(0x00011022, pData, dwSize);}

// Verwendung des Defaultpolynoms: x16+x15+x2+1
inline dword CreateCRC16Hash(void* pData, dword dwSize)	{return CreateCRCHash(0x00018006, pData, dwSize);}

}; // namespace Algorithm
namespace ADT {

// ******************************************************************************** //
enum HashMapMode
{
	HM_NO_RESIZE = 0,			// Konstante Größe (keine Autoresize beim Hinzufügen); Hohe Kollisionszahl
	HM_PREFER_SIZE = 1,			// Langsames Wachstum (sqrt); Mittlere Kollisionszahl; Häufiges Resize
	HM_RESIZE_MODERATE = 2,		// Mittleres Wachstum (sqrt/linear); Mittlere Kollisionszahl; Häufiges Resize
	HM_PREFER_PERFORMANCE = 3,	// Schnelles Wachstum (exponentiell); Geringe Kollisionszahl; Seltenes Resize
	HM_USE_STRING_MODE = 8		// Special mode for strings as keys
};

// ******************************************************************************** //
// The buckets are a very simple binary trees without any optimation.
class Bucket: public ADTElement
{
	Bucket(void* _pObj, const qword& _qwKey, Bucket* _pParent) :
			ADTElement(_pObj, _qwKey),
			pLeft(nullptr),
			pRight(nullptr),
			pParent(_pParent) {}
private:
	Bucket* pLeft;
	Bucket* pRight;
	Bucket* pParent;

	friend class HashMap;

	// Prevent copy constructor and operator = being generated.
	Bucket(const Bucket&);
	const Bucket& operator = (const Bucket&);
};
typedef Bucket* BucketP;

// ******************************************************************************** //
// The hash map is a structur to store and find data in nearly constant time (stochasticly).
class HashMap: public ADT
{
private:
	BucketP*		m_apBuckets;				// Ein Array mit Eimern (Bin-Trees)
	dword			m_dwSize;					// Größe des Arrays
	dword			m_dwNumElements;			// Anzahl Elemente in der Hashmap (kann größer als Arraygröße sein)
	HashMapMode		m_Mode;						// Resizemodus und optinal String Modus

	void RemoveData(BucketP _pBucket);
	void RecursiveReAdd(BucketP _pBucket);
	void RecursiveRelease(BucketP _pBucket);
	void TestSize();							// Checks if a resize is required in the current mode
	int ListIndex(BucketP _pBucket);			// If _pBucket points to the initial list this will calculate the index, and -1 if not

	// Prevent copy constructor and operator = being generated.
	HashMap(const HashMap&);
	const HashMap& operator = (const HashMap&);
public:
	HashMap(dword _dwSize, HashMapMode _Mode);
	virtual ~HashMap();

	void Resize(const dword _dwSize);							// Tabelle neu erzeugen und alle Elemente neu hinzufügen

	ADTElementP Insert(void* _pObject, qword _qwKey) override;	// Standard operation insert; If already exists the object is NOT overwritten, but referenccounter is increased
	void Delete(qword _qwKey) override;							// Standard operation delete
	void Delete(ADTElementP _pElement) override;				// Faster operation delete (no search)
	ADTElementP Search(qword _qwKey) override;					// Standard search with a key

	// String-Mode functions
	ADTElementP Insert(void* _pObject, const char* _pcKey);		// insert using strings; If already exists the object is NOT overwritten, but referenccounter is increased
	void Delete(const char* _pcKey);							// Standard operation delete for strings
	ADTElementP Search(const char* _pcKey);						// search using strings

	ADTElementP GetFirst() override;							// Erstes Objekt aus dem ersten nichtleeren Eimer
	ADTElementP GetLast() override;
	ADTElementP GetNext(ADTElementP _pCurrent) override;
	ADTElementP GetPrevious(ADTElementP _pCurrent) override;
};
typedef HashMap* HashMapP;

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //