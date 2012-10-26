// ******************************************************************************** //
// OrSplayTree.cpp																	//
// ===============																	//
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

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrBinaryTree.h"
#include "..\include\OrSplayTree.h"

using namespace OrE::ADT;

// ******************************************************************************** //
// Search with a key and splay (make sure the object with uiKey exists)
void OrE::ADT::SplayTree::Splay(BinaryTreeNodeP _pNode)
{
	while(_pNode->pParent)
	{
		if(_pNode->pParent->pLeft == _pNode)							// Left child
		{
			if(!_pNode->pParent->pParent)								// Zig case
				RotateRight(_pNode->pParent);
			else if(_pNode->pParent->pParent->pLeft == _pNode->pParent)	// Zig-Zig case
				RotateRight(RotateRight(_pNode->pParent->pParent));
			else {														// Zig-Zag case
				RotateRight(_pNode->pParent);
				RotateLeft(_pNode->pParent);	// Parent changed between
			}
		} else															// Right child
		{
			if(!_pNode->pParent->pParent)								// Zig case
				RotateLeft(_pNode->pParent);
			else if(_pNode->pParent->pParent->pRight == _pNode->pParent)// Zig-Zig case
				RotateLeft(RotateLeft(_pNode->pParent->pParent));
			else {														// Zig-Zag case
				RotateLeft(_pNode->pParent);
				RotateRight(_pNode->pParent);	// Parent changed between
			}
		}
	}
}

// ******************************************************************************** //
// Semi-Splay: reorganization through rotations (half the path length to the root)
void OrE::ADT::SplayTree::SemiSplay(BinaryTreeNodeP _pNode)
{
	_pNode = _pNode->pParent;
	while(_pNode && _pNode->pParent)
	{
		if(_pNode->pParent->pLeft == _pNode)							// Left child
		{
			if(!_pNode->pParent->pParent)								// Zig case
				RotateRight(_pNode->pParent);
			else if(_pNode->pParent->pParent->pLeft == _pNode->pParent)	// Zig-Zig case
				RotateRight(RotateRight(_pNode->pParent->pParent));
			else {														// Zig-Zag case
				RotateRight(_pNode->pParent);
				RotateLeft(_pNode->pParent);	// Parent changed between
			}
		} else															// Right child
		{
			if(!_pNode->pParent->pParent)								// Zig case
				RotateLeft(_pNode->pParent);
			else if(_pNode->pParent->pParent->pRight == _pNode->pParent)// Zig-Zig case
				RotateLeft(RotateLeft(_pNode->pParent->pParent));
			else {														// Zig-Zag case
				RotateLeft(_pNode->pParent);
				RotateRight(_pNode->pParent);	// Parent changed between
			}
		}
		// Semi splaying: continue with parent
		_pNode = _pNode->pParent;
	}
}


// ******************************************************************************** //
// Search with a key and splay
BinaryTreeNodeP OrE::ADT::SplayTree::SearchNode(qword _qwKey)
{
	// Search normal
	BinaryTreeNodeP pCurrent = m_pRoot;
	while(pCurrent->qwKey!=_qwKey)
	{
		if(pCurrent->qwKey<_qwKey)	// Search in the right tree
		{
			if(!pCurrent->pRight) {Splay(pCurrent); return pCurrent;}	// Return nearest one if not found
			pCurrent = pCurrent->pRight;
		} else {			// Search in the left tree
			if(!pCurrent->pLeft) {Splay(pCurrent); return pCurrent;}	// Return nearest one if not found
			pCurrent = pCurrent->pLeft;
		}
	}

	Splay(pCurrent);
		
	return pCurrent;
}

// ******************************************************************************** //
// Insert operation with splay
BinaryTreeNodeP OrE::ADT::SplayTree::Insert(void* _pObject, qword _qwKey)
{
	// Insert as usual
	BinaryTreeNodeP pCurrent = m_pRoot;
	while(pCurrent->qwKey!=_qwKey)
	{
		if(pCurrent->qwKey<_qwKey)	// Search in the right tree
		{
			if(!pCurrent->pRight)	// Insert as right
				pCurrent->pRight = new BinaryTreeNode(pCurrent, _pObject, _qwKey);
			else if(pCurrent->pRight->qwKey==_qwKey) pCurrent->pRight->AddRef();
			pCurrent = pCurrent->pRight;
		} else {			// Search in the left tree
			if(!pCurrent->pLeft)	// Insert as left
				pCurrent->pLeft = new BinaryTreeNode(pCurrent, _pObject, _qwKey);
			else if(pCurrent->pLeft->qwKey==_qwKey) pCurrent->pLeft->AddRef();
			pCurrent = pCurrent->pLeft;
		}
	}

	// Now pCurrent is an old node with the same hash or the new node
	Splay(pCurrent);
	return pCurrent;
}

// ******************************************************************************** //
// Standard operation delete
void OrE::ADT::SplayTree::Delete(qword _qwKey)
{
	// Search with splay (access)
	SearchNode(_qwKey);
	if((m_pRoot->qwKey == _qwKey) && (m_pRoot->Release() <= 0))
	{
		// Rebuild Tree without the node
		BinaryTreeNodeP pDel = m_pRoot;
		m_pRoot = m_pRoot->pLeft;
		m_pRoot->pParent = nullptr;
		// Splay rightmost node
		SearchNode(_qwKey);
		// The root now has no right child
		m_pRoot->pRight = pDel->pRight;
		// Delete now
		m_pDeleteCallback(pDel->pObject);
		delete pDel;
	}
	// else: Not found -> do nothing
}

// ******************************************************************************** //
// Delete operation without search
void OrE::ADT::SplayTree::Delete(ADTElementP _pNode)
{
	if(!_pNode) return;
	BinaryTreeNodeP pNode = (BinaryTreeNodeP)_pNode;
	// Force Splay
	Splay(pNode);
	
	// now: _pNode==m_pRoot
	if(m_pRoot->Release() <= 0)
	{
		// Rebuild Tree without the node
		m_pRoot = m_pRoot->pLeft;
		m_pRoot->pParent = nullptr;
		// Splay rightmost node
		SearchNode(pNode->qwKey);
		// The root now has no right child
		m_pRoot->pRight = pNode->pRight;
		// Delete now
		m_pDeleteCallback(pNode->pObject);
		delete pNode;
	}
}

// *************************************EOF**************************************** //