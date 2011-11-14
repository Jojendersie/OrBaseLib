// ******************************************************************************** //
// Huffman-Codec with different trees (Entropie coding)								//
//																					//
// Two adaptive approaches with self organisating search trees. Do not need an		//
// initialisation header.															//
// ******************************************************************************** //

#pragma once

// ******************************************************************************** //
// Coding relavant data structs. Defining an alphabet with an arbitary number of	//
// characters																		//
/*class OrChar : public OrHashObject
{
public:
	// This is more or less just a structure -> no private
	unsigned int uiChar;
	unsigned __int64 GetHash()	{return uiChar;}
	OrChar(unsigned int _uiC):uiChar(_uiC)	{}
};*/

/*class OrAlphabet
{
private:
	int m_iNumChars;
	//OrChar* m_pChars;
public:
	OrAlphabet(int _iSize)	{m_iNumChars = _iSize;}
	int GetNumChars() const	{return m_iNumChars;}
	//OrChar GetChar(int i)	{return m_pChars[i];}
};*/

// ******************************************************************************** //
// A littel Streamer buffer to make bitoperations easyer
class OrBitBufferStream
{
private:
	int m_iBufferPos;			// Position in full bytes
	int m_iBitPos;				// i€[0..7] -th bit of the current byte
	int m_iSize;				// Number of bytes in the buffer
	unsigned char* m_pBuffer;
public:
	OrBitBufferStream(int _iSize);
	OrBitBufferStream(void* _pBuffer, int _iSize);
	~OrBitBufferStream();

	inline void SetBit(int _iBit)		// Writes one bit an moves to the next one. Returns false if buffer overflow. _iBit in [0,1]
	{
		m_pBuffer[m_iBufferPos] |= _iBit<<(7-m_iBitPos++);
		m_iBufferPos += (m_iBitPos>>3);		// Adds 1 if m_iBitPos==8
		m_iBitPos &= 7;						// Sets _iBit to 0 if m_iBitPos=8
	}

	inline int GetBit()					// Returns the parity of the next bit and moves the cursors.
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
typedef OrBitBufferStream* OrBitBufferStreamP;

// ******************************************************************************** //
// Interface for entropy coder
class OrEntropyCoder
{
protected:
	virtual bool Encode(dword _c, OrBitBufferStreamP _pDest) = 0;
	virtual bool Decode(OrBitBufferStreamP _pSrc, dword& _Dest) = 0;

public:
	// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
	virtual bool EncodeFile(byte* _pSrc, int _iSize, OrBitBufferStreamP _pDest);

	// Decode into a buffer
	// Parameter:
	//	_pSrc - A Bit-Buffer containing the file data.
	//	_pDest - Already allocated buffer with at least _iMaxSize bytes.
	//	_iMaxSize - Size of uncompressed file.
	virtual int DecodeFile(OrBitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize);
};

// ******************************************************************************** //
// Adaptive variante 1: using splay trees
class OrHuffmanTree_Splay : public OrSplayTree, public OrEntropyCoder
{
private:
	// Build an AVL Tree recursive so that the characters are leaf orientated data.
	// inclusive the character _uiFrom
	// exclusive _uiRange
	OrBinaryTreeNodeP Init(unsigned int _uiFrom, unsigned int _uiRange);
protected:
	bool Encode(dword _c, OrBitBufferStreamP _pDest) override;
	bool Decode(OrBitBufferStreamP _pSrc, dword& _Dest) override;
public:
	OrHuffmanTree_Splay(unsigned int _uiNumCharacters);
};
typedef OrHuffmanTree_Splay* OrHuffmanTree_SplayP;

// ******************************************************************************** //
// Adaptive variante 2: using semi-splay trees
class OrHuffmanTree_SemiSplay : public OrHuffmanTree_Splay
{
protected:
	bool Encode(dword _c, OrBitBufferStreamP _pDest) override;
	bool Decode(OrBitBufferStreamP _pSrc, dword& _Dest) override;
public:
	OrHuffmanTree_SemiSplay(unsigned int _uiNumCharacters) : OrHuffmanTree_Splay(_uiNumCharacters)	{}
};
typedef OrHuffmanTree_SemiSplay* OrHuffmanTree_SemiSplayP;


// ******************************************************************************** //
// Combiened dictionary+entropy coding (AVC - adaptive vector codec)
class OrTrie;
class OrAVCoder : public OrHuffmanTree_Splay
{
private:
	//OrTrie* m_pDict;		// The Dictionary of vectors

public:
	OrAVCoder();
	~OrAVCoder();

	// Encodes the given buffer into a new one, returns false if encoding is larger then the buffer
	bool EncodeFile(byte* _pSrc, int _iSize, OrBitBufferStreamP _pDest) override;

	// Decode into a buffer
	// Parameter:
	//	_pSrc - A Bit-Buffer containing the file data.
	//	_pDest - Already allocated buffer with at least _iMaxSize bytes.
	//	_iMaxSize - Size of uncompressed file.
	int DecodeFile(OrBitBufferStreamP _pSrc, byte* _pDest, int _iMaxSize) override;
};
typedef OrAVCoder* OrAVCoderP;

// *************************************EOF**************************************** //