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

	bool SetBit(int _iBit);		// Writes one bit an moves to the next one. Returns false if buffer overflow.
	int GetBit();				// Returns the parity of the next bit and moves the cursors.
	void const* GetBuffer()		{return m_pBuffer;}
	int GetSize()				{return m_iSize;}
	int GetUsedSize()			{return m_iBufferPos+(m_iBitPos?1:0);}
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
	bool EncodeFile(unsigned char* _pSrc, int _iSize, OrBitBufferStreamP _pDest);

	// Decode into a buffer
	int DecodeFile(OrBitBufferStreamP _pSrc, unsigned char* _pDest, int _iMaxSize);
};

// ******************************************************************************** //
// Adaptive variante 1: using splay trees
class OrHuffmanTree_Splay : public OrSplayTree, public OrEntropyCoder
{
private:
	OrBinaryTreeNodeP Init(unsigned int _uiFrom, unsigned int _uiRange);
protected:
	bool Encode(dword _c, OrBitBufferStreamP _pDest);
	bool Decode(OrBitBufferStreamP _pSrc, dword& _Dest);
public:
	OrHuffmanTree_Splay(unsigned int _uiNumCharacters);
};
typedef OrHuffmanTree_Splay* OrHuffmanTree_SplayP;

// ******************************************************************************** //
// Adaptive variante 2: using semi-splay trees
class OrHuffmanTree_SemiSplay : public OrHuffmanTree_Splay
{
protected:
	bool Encode(dword _c, OrBitBufferStreamP _pDest);
	bool Decode(OrBitBufferStreamP _pSrc, dword& _Dest);
public:
	OrHuffmanTree_SemiSplay(unsigned int _uiNumCharacters) : OrHuffmanTree_Splay(_uiNumCharacters)	{}
};
typedef OrHuffmanTree_SemiSplay* OrHuffmanTree_SemiSplayP;


// *************************************EOF**************************************** //