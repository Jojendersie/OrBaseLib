#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"
#include "..\include\OrSplayTree.h"
#include "..\include\OrHuffman.h"

// Used to guarantee leaf storage without double using of a character
#define NODE_VALUE_FACTOR 3

// ******************************************************************************** //
// Build an AVL Tree recursive so that the characters are leaf orientated data.
// inclusive the character _uiFrom
// exclusive _uiRange
OrBinaryTreeNodeP OrHuffmanTree_Splay::Init(unsigned int _uiFrom, unsigned int _uiRange)
{
	// End of recursion
	if(_uiRange==1)
		return new OrBinaryTreeNode(nullptr, nullptr, _uiFrom*NODE_VALUE_FACTOR);
	
	// Build two subtrees with Num/2 nodes each
	OrBinaryTreeNodeP pLeft  = Init(_uiFrom,			_uiRange/2);
	OrBinaryTreeNodeP pRight = Init(_uiFrom+_uiRange/2, _uiRange-_uiRange/2);

	// Merge
	OrBinaryTreeNodeP pNew = new OrBinaryTreeNode(nullptr, nullptr, (pLeft->qwKey + pRight->qwKey)/2);
	pLeft->pParent = pNew;	pNew->pLeft = pLeft;
	pRight->pParent = pNew;	pNew->pRight = pRight;
	return pNew;
}

// ******************************************************************************** //
//void OrHSPDeleteObjectCallback(void* _pObject)	{}	// Do nothing the character is saved in the key
OrHuffmanTree_Splay::OrHuffmanTree_Splay(unsigned int _uiNumCharacters):OrSplayTree()
{
	//m_pDeleteCallback = OrHSPDeleteObjectCallback;

	// Create AVL-Tree as start configuration
	m_pRoot = Init(0, _uiNumCharacters);
}

// ******************************************************************************** //
bool OrHuffmanTree_Splay::Encode(dword _c, OrBitBufferStreamP _pDest)
{
	// Search the char in the tree and write down the path.
	OrBinaryTreeNodeP pCurrent = m_pRoot;
	qword qwKey = _c*NODE_VALUE_FACTOR;
	while(pCurrent->qwKey!=qwKey)
	{
		// It should be impossible, that there is missing one character (initialisation)
		if(pCurrent->qwKey<qwKey)								// Search in the right tree
		{
			if(!_pDest->SetBit(1)) return false;		// report buffer overflow
			pCurrent = pCurrent->pRight;
		} else {										// Search in the left tree
			if(!_pDest->SetBit(0)) return false;		// report buffer overflow
			pCurrent = pCurrent->pLeft;
		}
	}

	// Adaptive reorganisation
	// The node itself can't be splayed scince it never can be parent of an other node.
	Splay(pCurrent->pParent);
	return true;
}

// ******************************************************************************** //
bool OrHuffmanTree_Splay::Decode(OrBitBufferStreamP _pSrc, dword& _Dest)
{
	// Run through the tree. The bits are showing the way.
	int iBit = -1;//_pSrc->GetBit();
	OrBinaryTreeNodeP pCurrent = m_pRoot;
	while(pCurrent->pLeft || pCurrent->pRight)
	{
		iBit = _pSrc->GetBit();
		if(iBit) pCurrent = pCurrent->pRight;
		else pCurrent = pCurrent->pLeft;
	}

	_Dest = (dword)pCurrent->qwKey/NODE_VALUE_FACTOR;

	// Splay last parent to change the tree the same way like in encode.
	Splay(pCurrent->pParent);

	// If buffer is empty the last char was wrong.
	return iBit!=-1;
}





// ******************************************************************************** //
bool OrHuffmanTree_SemiSplay::Encode(dword _c, OrBitBufferStreamP _pDest)
{
	// Search the char in the tree and write down the path.
	OrBinaryTreeNodeP pCurrent = m_pRoot;
	unsigned __int64 uiKey = _c*NODE_VALUE_FACTOR;
	while(pCurrent->qwKey!=uiKey)
	{
		// It should be impossible, that there is missing one character (initialisation)
		if(pCurrent->qwKey<uiKey)								// Search in the right tree
		{
			if(!_pDest->SetBit(1)) return false;		// report buffer overflow
			pCurrent = pCurrent->pRight;
		} else {										// Search in the left tree
			if(!_pDest->SetBit(0)) return false;		// report buffer overflow
			pCurrent = pCurrent->pLeft;
		}
	}

	// Adaptive reorganisation
	SemiSplay(pCurrent);
	return true;
}

// ******************************************************************************** //
bool OrHuffmanTree_SemiSplay::Decode(OrBitBufferStreamP _pSrc, dword& _Dest)
{
	// Run through the tree. The bits are showing the way.
	int iBit = -1;//_pSrc->GetBit();
	OrBinaryTreeNodeP pCurrent = m_pRoot;
	while(pCurrent->pLeft || pCurrent->pRight)
	{
		iBit = _pSrc->GetBit();
		if(iBit) pCurrent = pCurrent->pRight;
		else pCurrent = pCurrent->pLeft;
	}

	_Dest = (dword)pCurrent->qwKey/NODE_VALUE_FACTOR;

	// Splay last parent to change the tree the same way like in encode.
	SemiSplay(pCurrent);

	// If buffer is empty the last char was wrong.
	return iBit!=-1;
}

// *************************************EOF**************************************** //