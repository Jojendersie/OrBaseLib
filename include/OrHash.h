// ******************************************************************************** //
// OrHash.h																			//
// ========																			//
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
dword CreateHash32(const void* pData, const int iSize);	// Create a 32 bit hash value of a data set
qword CreateHash64(const void* pData, const int iSize);	// Create a 64 bit hash value of a data set
dword CreateHash32(const void* pData);					// Create a 32 bit hash value of a 0-terminated data set (e.g. strings)
qword CreateHash64(const void* pData);					// Create a 64 bit hash value of a 0-terminated data set (e.g. strings)

// ******************************************************************************** //
// CRC - Error proving hash (cyclic redundancy check)
// The CRC functions determine the remaining of polynomial division.
// This value can be appended to the data. To prove correctness of the data repeat
// the CRC function with the same polynomial. A result of 0 denotes, that no checkable
// error occurred.
// Creates the hash with a arbitrary polynomial with a degree less than 32
dword CreateCRCHash(dword dwPolynom, void* pData, dword dwSize);

// Using of polynomial: x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
inline dword CreateCRC32IEEEHash(void* pData, dword dwSize)		{return CreateCRCHash(0x04C11DB7, pData, dwSize);}

// Using of polynomial: x24 + x23 + x18 + x17 + x14 + x11 + x10 + x7 + x6 + x5 + x4 + x3 + x + 1
inline dword CreateCRC24RadixHash(void* pData, dword dwSize)	{return CreateCRCHash(0x864CFB, pData, dwSize);}

// Using of polynomial: x16+x12+x5+1
inline dword CreateCRCCCITT16Hash(void* pData, dword dwSize)	{return CreateCRCHash(0x00011022, pData, dwSize);}

// Using of polynomial: x16+x15+x2+1
inline dword CreateCRC16Hash(void* pData, dword dwSize)			{return CreateCRCHash(0x00018006, pData, dwSize);}

}; // namespace Algorithm
namespace ADT {

// ******************************************************************************** //
enum HashMapMode
{
	HM_NO_RESIZE = 0,			// Constant size (no reszie during insertion); high collision count; could be verry fast for some purposes
	HM_PREFER_SIZE = 1,			// Resize if {#E>=3*Size} to {Size+3*sqrt(Size)}
	HM_RESIZE_MODERATE = 2,		// Resize if {#E>=1.5*Size} to {Size+Max(128,6*sqrt(Size))}
	HM_PREFER_PERFORMANCE = 3,	// Resize if {#E>=Size} to {(Size+100)*1.5}
	HM_USE_STRING_MODE = 8		// Special mode using strings as keys
};

// ******************************************************************************** //
// The buckets are a very simple binary trees without any optimization.
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
// The hash map is a structure to store and find data in nearly constant time (stochastically).
class HashMap: public ADT
{
private:
	BucketP*		m_apBuckets;				// An array with buckets (binary trees)
	dword			m_dwSize;					// Size of the array and therewith of hash map
	dword			m_dwNumElements;			// Number of elements currently in map (can be larger than array size)
	HashMapMode		m_Mode;						// Modes set in initialization (String mode?, Resize mode?)

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

	// Remove everything
	void Clear();

	// Recreate the table and reinsert all elements
	void Resize(const dword _dwSize);

	// Standard operation insert; If already existing, the object is NOT overwritten,
	// but reference counter is increased.
	ADTElementP Insert(void* _pObject, qword _qwKey) override;

	// Standard operation delete
	void Delete(qword _qwKey) override;

	// Faster operation delete (no search)
	void Delete(ADTElementP _pElement) override;

	// Standard search with a key
	ADTElementP Search(qword _qwKey) override;

	// String-Mode functions
	// insert using strings; If already existing, the object is NOT overwritten,
	// but reference counter is increased
	ADTElementP Insert(void* _pObject, const char* _pcKey);

	// Standard operation delete for strings
	void Delete(const char* _pcKey);

	// search using strings
	ADTElementP Search(const char* _pcKey);

	// The overall first object from the first non-empty bucket
	ADTElementP GetFirst() override;
	ADTElementP GetLast() override;
	ADTElementP GetNext(ADTElementP _pCurrent) override;
	ADTElementP GetPrevious(ADTElementP _pCurrent) override;

	bool IsEmpty()			{return m_dwNumElements==0;}

	// Objects/elements in the hash map. Not its capacity.
	dword GetNumElements()	{return m_dwNumElements;}

	// Size of table array. This is not the number of elements (GetNumElements).
	dword GetSize()			{return m_dwSize;}

	typedef Iterator<ADTElement> Iterator;

#ifdef _DEBUG
	dword m_dwCollsionCounter;
#endif
};
typedef HashMap* HashMapP;

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //