// ******************************************************************************** //
// OrHuffman.cpp																	//
// =============																	//
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

using namespace OrE::ADT;
using namespace OrE::Algorithm;
using namespace OrE::Math;
//#include <iostream>

// Used to guarantee leaf storage without double using of a character
#define NODE_VALUE_FACTOR 512

// ******************************************************************************** //
// Build an AVL Tree recursive so that the characters are leaf orientated data.
// inclusive the character _uiFrom
// exclusive _uiRange
BinaryTreeNodeP OrE::Algorithm::HuffmanTree_Splay::Init(unsigned int _uiFrom, unsigned int _uiRange)
{
	// End of recursion
	if(_uiRange==1)
	{
	//	std::cout << _uiFrom*NODE_VALUE_FACTOR << "  ";
		return new BinaryTreeNode(nullptr, nullptr, _uiFrom*NODE_VALUE_FACTOR);
	}
	
	// Build two subtrees with Num/2 nodes each
	BinaryTreeNodeP pLeft  = Init(_uiFrom,			_uiRange/2);
	BinaryTreeNodeP pRight = Init(_uiFrom+_uiRange/2, _uiRange-_uiRange/2);

	// Merge
//	std::cout << (pLeft->qwKey + pRight->qwKey)/2 << "  ";
	BinaryTreeNodeP pNew = new BinaryTreeNode(nullptr, nullptr, (Min(pRight)->qwKey+Max(pLeft)->qwKey)/2/*_uiFrom*NODE_VALUE_FACTOR + _uiRange-1*/);
	pLeft->pParent = pNew;	pNew->pLeft = pLeft;
	pRight->pParent = pNew;	pNew->pRight = pRight;
	return pNew;
}

// ******************************************************************************** //
//void OrHSPDeleteObjectCallback(void* _pObject)	{}	// Do nothing the character is saved in the key
OrE::Algorithm::HuffmanTree_Splay::HuffmanTree_Splay(unsigned int _uiNumCharacters):SplayTree()
{
	//m_pDeleteCallback = OrHSPDeleteObjectCallback;

	// Create AVL-Tree as start configuration
	m_pRoot = Init(0, _uiNumCharacters);
}

// ******************************************************************************** //
bool OrE::Algorithm::HuffmanTree_Splay::Encode(dword _c, BitBufferStreamP _pDest)
{
	// Stop if we could use more bits than remaining buffer.
	if(_pDest->GetUsedSize()+70 > _pDest->GetSize()) return false;

	// Search the char in the tree and write down the path.
	BinaryTreeNodeP pCurrent = m_pRoot;
	qword qwKey = _c*NODE_VALUE_FACTOR;
	while(pCurrent->qwKey!=qwKey)
	{
		// It should be impossible, that there is missing one character (initialisation)
		if(pCurrent->qwKey<qwKey)				// Search in the right tree
		{
			_pDest->SetBit(1);					// report buffer overflow
			pCurrent = pCurrent->pRight;
		} else {								// Search in the left tree
			_pDest->SetBit(0);					// report buffer overflow
			pCurrent = pCurrent->pLeft;
		}
	}

	// Adaptive reorganisation
	// The node itself can't be splayed scince it never can be parent of an other node.
	Splay(pCurrent->pParent);
	return true;
}

// ******************************************************************************** //
bool OrE::Algorithm::HuffmanTree_Splay::Decode(BitBufferStreamP _pSrc, dword& _Dest)
{
	// Run through the tree. The bits are showing the way.
	int iBit = -1;//_pSrc->GetBit();
	BinaryTreeNodeP pCurrent = m_pRoot;
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
bool OrE::Algorithm::HuffmanTree_SemiSplay::Encode(dword _c, BitBufferStreamP _pDest)
{
	// Stop if we could use more bits than remaining buffer.
	if(_pDest->GetUsedSize()+70 > _pDest->GetSize()) return false;

	// Search the char in the tree and write down the path.
	BinaryTreeNodeP pCurrent = m_pRoot;
	unsigned __int64 uiKey = _c*NODE_VALUE_FACTOR;
	while(pCurrent->qwKey!=uiKey)
	{
		// It should be impossible, that there is missing one character (initialisation)
		if(pCurrent->qwKey<uiKey)			// Search in the right tree
		{
			_pDest->SetBit(1);				// report buffer overflow
			pCurrent = pCurrent->pRight;
		} else {							// Search in the left tree
			_pDest->SetBit(0);				// report buffer overflow
			pCurrent = pCurrent->pLeft;
		}
	}

	// Adaptive reorganisation
	SemiSplay(pCurrent);
	return true;
}

// ******************************************************************************** //
bool OrE::Algorithm::HuffmanTree_SemiSplay::Decode(BitBufferStreamP _pSrc, dword& _Dest)
{
	// Run through the tree. The bits are showing the way.
	int iBit = -1;//_pSrc->GetBit();
	BinaryTreeNodeP pCurrent = m_pRoot;
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