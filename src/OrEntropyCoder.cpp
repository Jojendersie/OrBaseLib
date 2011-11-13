#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"
#include "..\include\OrSplayTree.h"
#include "..\include\OrHuffman.h"
#include <string.h>

// ******************************************************************************** //
OrBitBufferStream::OrBitBufferStream(int _iSize)
{
	m_pBuffer = new unsigned char[_iSize];
	memset(m_pBuffer, 0, _iSize);
	m_iSize = _iSize;
	m_iBitPos = 0;
	m_iBufferPos = 0;
}
OrBitBufferStream::OrBitBufferStream(void* _pBuffer, int _iSize)
{
	m_pBuffer = new unsigned char[_iSize];
	memcpy(m_pBuffer, _pBuffer, _iSize);
	m_iSize = _iSize;
	m_iBitPos = 0;
	m_iBufferPos = 0;
}

OrBitBufferStream::~OrBitBufferStream()
{
	delete[] m_pBuffer;
}

// ******************************************************************************** //
// Writes one bit an moves to the next one. Returns false if buffer overflow.
/*void OrBitBufferStream::SetBit(int _iBit)
{
	m_pBuffer[m_iBufferPos] |= _iBit<<(7-m_iBitPos++);
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
}

// ******************************************************************************** //
// Returns the parity of the next bit and moves the cursors.
int OrBitBufferStream::GetBit()
{
	if(m_iBufferPos>=m_iSize) return -1;
	int iRet = (m_pBuffer[m_iBufferPos] >> (7-m_iBitPos++)) & 1;
	m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
	m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
	return iRet;
}*/

// ******************************************************************************** //
// Writes _iNum bits and move.
bool OrBitBufferStream::SetBits(dword _dwBits, int _iNum)
{
	// Buffer overflow?
	if((m_iBitPos+_iNum)/8+m_iBufferPos >= m_iSize) return false;

	// Copy as much as in the one byte matches
	int iOverfill = _iNum-(8-m_iBitPos);
	if(iOverfill >= 0) {
		m_pBuffer[m_iBufferPos] |= _dwBits>>iOverfill;						// adds 8-m_iBitPos bits
		(*(dword*)&m_pBuffer[m_iBufferPos+1]) |= _dwBits<<(32-iOverfill);	// adds all remaining bits
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
// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
bool OrEntropyCoder::EncodeFile(byte* _pSrc, int _iSize, OrBitBufferStreamP _pDest)
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
int OrEntropyCoder::DecodeFile(OrBitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize)
{
	dword c;
	int i=-1;
	// EOF is given by an return value false from Decode()
	while(Decode(_pSrc, c) && ++i<_iMaxSize)
		_pDest[i] = (unsigned char)c;
	return ++i;	// Do some magic with i, that it is still smaller if all goes korrekt
}



// *************************************EOF**************************************** //