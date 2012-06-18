// ******************************************************************************** //
// OrEntropyCoder.cpp																//
// ==================																//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"
#include "..\include\OrSplayTree.h"
#include "..\include\OrHuffman.h"
#include <string.h>

// ******************************************************************************** //
OrE::Algorithm::BitBufferStream::BitBufferStream(int _iSize)
{
	m_pBuffer = new unsigned char[_iSize];
	memset(m_pBuffer, 0, _iSize);
	m_iSize = _iSize;
	m_iBitPos = 0;
	m_iBufferPos = 0;
}

OrE::Algorithm::BitBufferStream::BitBufferStream(void* _pBuffer, int _iSize)
{
	m_pBuffer = new unsigned char[_iSize];
	memcpy(m_pBuffer, _pBuffer, _iSize);
	m_iSize = _iSize;
	m_iBitPos = 0;
	m_iBufferPos = 0;
}

OrE::Algorithm::BitBufferStream::~BitBufferStream()
{
	delete[] m_pBuffer;
}

// ******************************************************************************** //
// Writes one bit an moves to the next one. Returns false if buffer overflow.
/*void OrE::Algorithm::BitBufferStream::SetBit(int _iBit)
{
	m_pBuffer[m_iBufferPos] |= _iBit<<(7-m_iBitPos++);
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
}

// ******************************************************************************** //
// Returns the parity of the next bit and moves the cursors.
int OrE::Algorithm::BitBufferStream::GetBit()
{
	if(m_iBufferPos>=m_iSize) return -1;
	int iRet = (m_pBuffer[m_iBufferPos] >> (7-m_iBitPos++)) & 1;
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
	return iRet;
}*/

// ******************************************************************************** //
// Writes _iNum bits and move.
bool OrE::Algorithm::BitBufferStream::SetBits(dword _dwBits, int _iNum)
{
	// Buffer overflow?
	if(((m_iBitPos+_iNum)>>3)+m_iBufferPos >= m_iSize) return false;

	// Copy as much as in the one byte matches
	int iOverfill = _iNum-(8-m_iBitPos);
	if(iOverfill >= 0) {
		m_pBuffer[m_iBufferPos] |= iOverfill<0?_dwBits<<-iOverfill:_dwBits>>iOverfill;						// adds 8-m_iBitPos bits
		m_pBuffer[m_iBufferPos+1] |= iOverfill-8<0?_dwBits<<-(iOverfill-8):(_dwBits>>(iOverfill-8));
		m_pBuffer[m_iBufferPos+2] |= iOverfill-16<0?_dwBits<<-(iOverfill-16):(_dwBits>>(iOverfill-16));
		m_pBuffer[m_iBufferPos+3] |= iOverfill-24<0?_dwBits<<-(iOverfill-24):(_dwBits>>(iOverfill-24));
//		(*(dword*)&m_pBuffer[m_iBufferPos+1]) |= _dwBits<<(32-iOverfill);	// adds all remaining bits
	} else
	{
		m_pBuffer[m_iBufferPos] |= _dwBits<<-iOverfill;						// copies all new bits
	}

	m_iBitPos += _iNum;					// Adds number of new bits
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 to 4 if m_iBitPos>=8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8

	return true;
}

// ******************************************************************************* //
// Reads iNum bits and move
bool OrE::Algorithm::BitBufferStream::GetBits(dword& _dwBits, int _iNum)
{
	// Buffer overflow?
	if(((m_iBitPos+_iNum)>>3)+m_iBufferPos >= m_iSize) return false;

	int iOverfill = _iNum-(8-m_iBitPos);
	_dwBits =  iOverfill<0?m_pBuffer[m_iBufferPos]>>-iOverfill : (m_pBuffer[m_iBufferPos] & ((256>>m_iBitPos)-1)) << iOverfill;
	iOverfill -= 8;
	_dwBits |= iOverfill<0?m_pBuffer[m_iBufferPos+1]>>-iOverfill: m_pBuffer[m_iBufferPos+1]<<iOverfill;
	iOverfill -= 8;
	_dwBits |= iOverfill<0?m_pBuffer[m_iBufferPos+2]>>-iOverfill: m_pBuffer[m_iBufferPos+1]<<iOverfill;
	iOverfill -= 8;
	_dwBits |= iOverfill<0?m_pBuffer[m_iBufferPos+3]>>-iOverfill: m_pBuffer[m_iBufferPos+1]<<iOverfill;
	_dwBits &= ((1<<_iNum) - 1);
	// Copy 25-32 bits and mask all unnessesary
	//_dwBits = (m_pBuffer[m_iBufferPos]) << m_iBitPos) & ((1<<_iNum) - 1);
	//_dwBits = (*(dword*)&m_pBuffer[m_iBufferPos] << m_iBitPos) >> (32-_iNum);

	m_iBitPos += _iNum;					// Adds number of new bits
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 to 4 if m_iBitPos>=8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8

	return true;
}

// ******************************************************************************* //
// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
bool OrE::Algorithm::EntropyCoder::EncodeFile(byte* _pSrc, int _iSize, BitBufferStreamP _pDest)
{
	bool bNoError = true;
	dword c;
	for(int i=0;i<_iSize && bNoError;++i)
	{
		c = _pSrc[i];
		bNoError = Encode(c, _pDest);
	}

	return bNoError;
}

// ******************************************************************************* //
// Decode into a buffer
int OrE::Algorithm::EntropyCoder::DecodeFile(BitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize)
{
	dword c;
	int i=-1;
	// EOF is given by an return value false from Decode()
	while(Decode(_pSrc, c) && ++i<_iMaxSize)
		_pDest[i] = (unsigned char)c;
	return ++i;	// Do some magic with i, that it is still smaller if all goes korrekt
}



// *************************************EOF**************************************** //