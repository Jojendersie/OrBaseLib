// ******************************************************************************** //
// OrHuffman.h																		//
// ===========																		//
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
// Huffman-Codec with different trees (Entropie coding)								//
//																					//
// Two adaptive approaches with self organisating search trees. Do not need an		//
// initialisation header.															//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace Algorithm {

// ******************************************************************************** //
// A littel Streamer buffer to make bitoperations easyer
class BitBufferStream
{
private:
	int m_iBufferPos;			// Position in full bytes
	int m_iBitPos;				// i€[0..7] -th bit of the current byte
	int m_iSize;				// Number of bytes in the buffer
	unsigned char* m_pBuffer;
public:
	BitBufferStream(int _iSize);
	BitBufferStream(void* _pBuffer, int _iSize);
	~BitBufferStream();

	inline void SetBit(int _iBit)			// Writes one bit an moves to the next one. Returns false if buffer overflow. _iBit in [0,1]
	{
		m_pBuffer[m_iBufferPos] |= _iBit<<(7-m_iBitPos++);
		m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
		m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
	}

	inline int GetBit()						// Returns the parity of the next bit and moves the cursors.
	{
		if(m_iBufferPos>=m_iSize) return -1;
		int iRet = (m_pBuffer[m_iBufferPos] >> (7-m_iBitPos++)) & 1;
		m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
		m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
		return iRet;
	}

	bool SetBits(dword _dwBits, int _iNum);		// Writes _iNum bits and move.
	bool GetBits(dword& _dwBits, int _iNum);	// Reads iNum bits and move !Only use _iNum <= 25!
	void const* GetBuffer()		{return m_pBuffer;}
	int GetSize()				{return m_iSize;}
	int GetUsedSize()			{return m_iBufferPos+(m_iBitPos?1:0);}
	bool IsEof()				{return m_iBufferPos>=m_iSize;}
};
typedef BitBufferStream* BitBufferStreamP;

// ******************************************************************************** //
// Interface for entropy coder
class EntropyCoder
{
protected:
	virtual bool Encode(dword _c, BitBufferStreamP _pDest) = 0;
	virtual bool Decode(BitBufferStreamP _pSrc, dword& _Dest) = 0;

public:
	// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
	virtual bool EncodeFile(byte* _pSrc, int _iSize, BitBufferStreamP _pDest);

	// Decode into a buffer
	// Input:
	//	_pSrc - A Bit-Buffer containing the file data.
	//	_pDest - Already allocated buffer with at least _iMaxSize bytes.
	//	_iMaxSize - Size of uncompressed file.
	virtual int DecodeFile(BitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize);
};

// ******************************************************************************** //
// Adaptive variante 1: using splay trees
class HuffmanTree_Splay : public OrE::ADT::SplayTree, public EntropyCoder
{
private:
	// Build an AVL Tree recursive so that the characters are leaf orientated data.
	// inclusive the character _uiFrom
	// exclusive _uiRange
	OrE::ADT::BinaryTreeNodeP Init(unsigned int _uiFrom, unsigned int _uiRange);
protected:
	bool Encode(dword _c, BitBufferStreamP _pDest) override;
	bool Decode(BitBufferStreamP _pSrc, dword& _Dest) override;
public:
	HuffmanTree_Splay(unsigned int _uiNumCharacters);
};
typedef HuffmanTree_Splay* HuffmanTree_SplayP;

// ******************************************************************************** //
// Adaptive variante 2: using semi-splay trees
class HuffmanTree_SemiSplay : public HuffmanTree_Splay
{
protected:
	bool Encode(dword _c, BitBufferStreamP _pDest) override;
	bool Decode(BitBufferStreamP _pSrc, dword& _Dest) override;
public:
	HuffmanTree_SemiSplay(unsigned int _uiNumCharacters) : HuffmanTree_Splay(_uiNumCharacters)	{}
};
typedef HuffmanTree_SemiSplay* HuffmanTree_SemiSplayP;


// ******************************************************************************** //
// Combiened dictionary+entropy coding (AVC - adaptive vector codec)
class Trie;
class AVCoder : public HuffmanTree_Splay
{
private:
	//Trie* m_pDict;		// The Dictionary of vectors

public:
	AVCoder();
	~AVCoder();

	// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
	bool EncodeFile(byte* _pSrc, int _iSize, BitBufferStreamP _pDest) override;

	// Decode into a buffer
	// Parameter:
	//	_pSrc - A Bit-Buffer containing the file data.
	//	_pDest - Already allocated buffer with at least _iMaxSize bytes.
	//	_iMaxSize - Size of uncompressed file.
	int DecodeFile(BitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize) override;
};
typedef AVCoder* AVCoderP;

}; // namespace OrE
}; // namespace Algorithm
// *************************************EOF**************************************** //