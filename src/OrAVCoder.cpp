// ******************************************************************************** //
// OrAVCoder.cpp																	//
// =============																	//
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
// Combined dictionary+entropy coding (AVC - adaptive vector codec)					//
// Idea: Johannes Jendersie															//
//	For each character c:															//
//		* If vector beginning in c matches a part of the dictionary safe a			//
//		  reference #. Otherwise use adaptive Huffman to safe the character.		//
//		* Fill dictionary with a 1024 Byte vector beginning at this char.			//
//		* Remove oldest vector (in next step more than 1024 bytes in past) from		//
//		  dictionary. (To avoid long references)									//
//																					//
//	# vector references:															//
//		First a meta character is saved with adaptive Huffman to mark next value	//
//		as vector reference. There are 3 classes of vectors:						//
//		'256' - short vector (7 Bit Offset; 3 Bit Length)							//
//			used if distance is in [1,128] and length of vector in [3,10] bytes.	//
//		'257' - middle vector (9 Bit Offset; 6 Bit Length)							//
//			distance: [1,512]														//
//			length: [4,67]															//
//		'258' - range vector (10 Bit Offset; 10 Bit Length)							//
//			distance: [1,1024]														//
//			length: [6,1029]														//
// ******************************************************************************** //

#include "../include/OrTypeDef.h"
#include "../include/OrADTObjects.h"
#include "../include/OrBinaryTree.h"
#include "../include/OrSplayTree.h"
#include "../include/OrTrie.h"
#include "../include/OrFastMath.h"
#include "../include/OrHuffman.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <memory>

using namespace OrE::Algorithm;

const uint32 OR_AVC_NUMVEC			= 5;
const uint32 OR_AVC_OFFSET_BITS[]	= {4,7,10,12,14};
const uint32 OR_AVC_LENGTH_BITS[]	= {1,3,4,7,8};
const uint32 OR_AVC_LENGTH_OFFSETS[]	= {2,3,4,5,5};
const uint32 OR_AVC_MAX_OFFSET		= 16384;//(1<<OR_AVC_OFFSET_BITS[OR_AVC_NUMVEC-1]);
const uint32 OR_AVC_MAX_LENGTH		= (1<<OR_AVC_LENGTH_BITS[OR_AVC_NUMVEC-1])-1+OR_AVC_LENGTH_OFFSETS[OR_AVC_NUMVEC-1];
// Test results:
//	test.bmp - 982162/956131/956075/954565/948540/947984/948054/952318/937.../929187/917727
//	test2.bmp - 
//	test.txt - 142
//	test.mp3 - 
//	test.cpp - 4358/4330/4305/4272/4264/4257/4178/4175/4031/3991/4014
//	orcodermain.cpp - 1819/1804/1789/1783/1786/1785/1770/1760/1773/1782
//	test.exe - 25811/25772/25885/25729/25737/25492/25610/25372
//	test.pdf - 1098117/1097731/1095218/1091566/1075793/1074379/1052953

static uint32 Statistic_Entro = 0;
static uint32 Statistic_Vec[OR_AVC_NUMVEC] = {0};
static uint32 Statistic_VecLen[OR_AVC_NUMVEC] = {0};
static uint32 Statistic_VecOff[OR_AVC_NUMVEC] = {0};

// ******************************************************************************** //
// Pattern matching with Knuth-Morris-Pratt-Algorithmus
void OrKMPCreatePrefixTable(byte* _pIn, int _iLen, int* _piOut)
{
	int i = 0;		// Text position
	int j = -1;		// Length of matching prefix
	_piOut[0] = -1;	// First entry
	while(i<_iLen)
	{
		while(j>=0 && _pIn[i]!=_pIn[j])
			j = _piOut[j];			// jump to next prefix

		assert(j==-1 || _pIn[i]==_pIn[j]);

		// Match case move both pointers and save match length
		_piOut[++i] = ++j;
	}
}

// ******************************************************************************** //
void OrKMPMatch(byte* _pText, byte* _pPattern, int _iTextLen, int _iPatternLen, int* _piPrefix, int& _iOutL, int& _iOutO)
{
	// Create prefix table for pattern
	OrKMPCreatePrefixTable(_pPattern, _iPatternLen, _piPrefix);

	int i = 0;		// Text position
	int j = -1;		// Length of matching prefix
	_iOutL = -1;	// No match yet
	while(i<_iTextLen && j<_iPatternLen)
	{
		while(j>=0 && _pText[i]!=_pPattern[j])
			j = _piPrefix[j];			// jump to next prefix

		assert(j==-1 || _pText[i]==_pPattern[j]);

		// Match case move both pointers
		++i; ++j;

		// Save longest match
		if(j > _iOutL) {
			_iOutL = j;		// current max match
			_iOutO = i-j;	// the match began j bytes earlier
		}
	}
}

// ******************************************************************************** //
// Constructor: initialize Huffman tree for 256+ characters (OR_AVC_NUMVEC Vector-Meta-Characters)
OrE::Algorithm::AVCoder::AVCoder() : HuffmanTree_Splay(256+OR_AVC_NUMVEC)
{
	//m_pDict = new Trie();
}

// ******************************************************************************** //
OrE::Algorithm::AVCoder::~AVCoder()
{
	// Delete self created resources
	//delete m_pDict;
}

struct OrDictCharEntry
{
	OrDictCharEntry* pNext;
	byte*			 pFile;
};

// ******************************************************************************** //
// Implements description at the beginning of the file
bool OrE::Algorithm::AVCoder::EncodeFile(byte* _pSrc, int _iSize, BitBufferStreamP _pDest)
{
	// Create free list dictionary for the first chars
	OrDictCharEntry* pDict[256];			// The dictionary of vectors
	// Memory management
	// Use a scoped pointer to avoid memory leaks in the many return cases
	std::unique_ptr<OrDictCharEntry> pScopedPointerToList( new OrDictCharEntry[OR_AVC_MAX_OFFSET+1] );
	OrDictCharEntry* pFreeList = pScopedPointerToList.get();
	OrDictCharEntry* pNextFree = pScopedPointerToList.get();
	// Initialize free list
	memset(pDict, 0, sizeof(pDict));
	for(int i=0; i<OR_AVC_MAX_OFFSET; ++i)
		pFreeList[i].pNext = &pFreeList[i+1];
	pFreeList[OR_AVC_MAX_OFFSET].pNext = nullptr;

	// If a vector was saved the following chars are already in _pDest. We have to
	// look at them to build our dictionary, but we don't have to code them again!
	uint32 dwNoCode = 0;
	// Encode each character
	for(uint32 i=0; i<(uint32)_iSize; ++i)
	{
		uint32 dwMaxVSize = Math::Min(_iSize-i, OR_AVC_MAX_LENGTH);
		if(dwNoCode) --dwNoCode;
		else
		{
			// 1. Search for longest match
			uint32 dwLength=0, dwOffset=1000000;
			OrDictCharEntry* pCur = pDict[_pSrc[i]];
			while(pCur)
			{
				// Begin to count matches starting in pCur
				byte* pBytePast = pCur->pFile;
				byte* pByteCurrent = &_pSrc[i];
				uint32 dwL = 1;
				while(pBytePast[dwL] == pByteCurrent[dwL] && (dwL<dwMaxVSize)) ++dwL;
				if(dwL>dwLength) {
					dwLength = dwL;
					dwOffset = (uint32)(&_pSrc[i]-pBytePast);
				}
				pCur = pCur->pNext;
			}
			//dwLength = Minu(dwLength, dwMaxVSize);
			/*TrieString Cursor = TrieString((char*)&_pSrc[i], dwMaxVSize);
			TrieNodeP pMatchV = m_pDict->Match(Cursor);
			uint32 dwLength = dwMaxVSize-Cursor.m_dwLen; //(uint32)Cursor.m_pcString - (uint32)&_pSrc[i];
			uint32 dwOffset = pMatchV?(i + 1 - (uint32)pMatchV->pData):1000000;*/
			// There is the possibility, that the vector has even an longer match than in dict (could be used for 1024-1029 bytes (gaining 5 extra bytes)

			// 2. Classify match and safe character/vector
			for(int j=0; j<OR_AVC_NUMVEC; ++j)
			{
				if(    (dwLength >= OR_AVC_LENGTH_OFFSETS[j])
					&& (dwLength < ((1<<OR_AVC_LENGTH_BITS[j]) + OR_AVC_LENGTH_OFFSETS[j]))
					&& (dwOffset <= (1U<<OR_AVC_OFFSET_BITS[j]))  )
				{
					Statistic_Vec[j]++;
					Statistic_VecLen[j] += dwLength;
					Statistic_VecOff[j] += dwOffset;
					// if there is a vector match we don't code the coming bytes from the vector again
					dwNoCode = dwLength-1;

					// Write vector now (Class + Offset + Length)
					if(!Encode(256+j, _pDest))
						return false;
					dwLength -= OR_AVC_LENGTH_OFFSETS[j]; --dwOffset;

					// Stop if we could use more bits than remaining buffer.
					if(_pDest->GetUsedSize()+(OR_AVC_OFFSET_BITS[j]+OR_AVC_LENGTH_BITS[j])/8+1 >= (uint32)_pDest->GetSize())
						return false;

					_pDest->SetBits(dwOffset, OR_AVC_OFFSET_BITS[j]);
					_pDest->SetBits(dwLength, OR_AVC_LENGTH_BITS[j]);
					/*for(uint32 k=0; k<OR_AVC_OFFSET_BITS[j]; ++k)
						_pDest->SetBit((dwOffset&(1<<k))?1:0);
					for(uint32 k=0; k<OR_AVC_LENGTH_BITS[j]; ++k)
						_pDest->SetBit((dwLength&(1<<k))?1:0);*/
					goto SkipSingleChar;
				}
			}

			dwNoCode = 0;
			// Pure character
			Statistic_Entro++;
			if(!Encode(_pSrc[i], _pDest))
				return false;

SkipSingleChar:;
		}

		// 3. Add new vector
		// Remember next free entry
		OrDictCharEntry* pNext = pNextFree->pNext;
		// Append vector in the list
		pNextFree->pFile = &_pSrc[i];
		pNextFree->pNext = pDict[_pSrc[i]];
		pDict[_pSrc[i]] = pNextFree;
		// Move to next free entry
		pNextFree = pNext;

		// 4. Remove oldest vector
		if(i >= OR_AVC_MAX_OFFSET)
		{
			// The last reference in the char list is that char of this position
			OrDictCharEntry* pTemp = pDict[_pSrc[i-OR_AVC_MAX_OFFSET]];
			OrDictCharEntry* pPrev = nullptr;
			// Remove from dictionary
			while(pTemp->pNext) {pPrev = pTemp; pTemp = pTemp->pNext;}
			if(!pPrev) pDict[_pSrc[i-OR_AVC_MAX_OFFSET]] = nullptr;
			else pPrev->pNext = nullptr;
			// Remove from free list
			pPrev = pNextFree;	// temp copy
			pNextFree = pTemp;
			pNextFree->pNext = pPrev;
		}
	}

	for(int i=0;i<OR_AVC_NUMVEC;++i)
	{
		Statistic_VecLen[i] /= Statistic_Vec[i];
		Statistic_VecOff[i] /= Statistic_Vec[i];
	}
	uint32 dwNonEntro = _iSize - Statistic_Entro;
	return true;
}

// ******************************************************************************** //
// Decode into a buffer
// Parameter:
//	_pSrc - A Bit-Buffer containing the file data.
//	_pDest - Already allocated buffer with at least _iMaxSize bytes.
//	_iMaxSize - Size of uncompressed file.
int OrE::Algorithm::AVCoder::DecodeFile(BitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize)
{
	uint32 c;
	int i=-1;
	// EOF is given by an return value false from Decode()
	while(Decode(_pSrc, c) && ++i<_iMaxSize)
	{
		// Distinguish between vector and pure character
		if(c>255)
		{
			// How long is the vector code?
			c -= 256;

			// Read vector reference
			uint32 dwLength = 0, dwOffset = 0;
			if(!_pSrc->GetBits(dwOffset, OR_AVC_OFFSET_BITS[c])) return false;
			if(!_pSrc->GetBits(dwLength, OR_AVC_LENGTH_BITS[c])) return false;
			dwLength += OR_AVC_LENGTH_OFFSETS[c]; ++dwOffset;

			// Read vector (byte by byte since offset could be one -> copy of the last copied char)
			for(uint32 k=0; k<dwLength; ++k)
			{
				if(i+(int)k >= _iMaxSize)
					return -1;
				_pDest[i+k] = _pDest[i+k-dwOffset];
			}
			i += dwLength-1;
		} else
			_pDest[i] = (unsigned char)c;
	}
	return ++i;	// Do some magic with i, that it is still smaller if all goes correct
}

// *************************************EOF**************************************** //