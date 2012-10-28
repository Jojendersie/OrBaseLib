// ******************************************************************************** //
// OrBinaryTree.h																	//
// ==============																	//
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
// Base class for trees. Implements a subset of the ADT interface.					//
//																					//
//	Insert()					abstract											//
//	Search()					O(h)	h - height of tree							//
//	Delete(Key)					abstract											//
//	Delete(Element)				abstract											//
//	GetFirst()					O(h)												//
//	GetLast()					O(h)												//
//	GetNext()					O(h)												//
//	GetPrevious()				O(h)												//
// ******************************************************************************** //

#pragma once


namespace OrE {
	// Declarations need to assure safe access
	namespace Algorithm {
		class HuffmanTree_Splay;
		class HuffmanTree_SemiSplay;
	};	// Algorithm
namespace ADT {

// ******************************************************************************** //
// Simple node for all binary trees
class BinaryTreeNode: public ADTElement
{
	// Make nodes unchangeable for all users except the trees. So the trees can return
	// references to non constant nodes without any (large) risk.
	friend class BinaryTree;
	friend class SplayTree;
	friend class AVLTree;
	friend class LinkedAVLTree;
	friend class Algorithm::HuffmanTree_Splay;
	friend class Algorithm::HuffmanTree_SemiSplay;
protected:
	BinaryTreeNode* pLeft;
	BinaryTreeNode* pRight;
	BinaryTreeNode* pParent;
	int iParam;					// Statistic information to mark nodes (use case differs on application)

	BinaryTreeNode(BinaryTreeNode* _pParent, void* _pObj, const qword& _qwKey):ADTElement(_pObj, _qwKey), pLeft(nullptr),pRight(nullptr),pParent(_pParent)	{}

	// Prevent copy constructor and operator = being generated.
	BinaryTreeNode(const BinaryTreeNode&);
	const BinaryTreeNode& operator = (const BinaryTreeNode&);

public:
	// Accessors to traverse the tree
	BinaryTreeNode* GetLeft()		{return pLeft;}
	BinaryTreeNode* GetRight()		{return pRight;}
	BinaryTreeNode* GetParent()		{return pParent;}
};
typedef BinaryTreeNode* BinaryTreeNodeP;
typedef BinaryTreeNode const * BinaryTreeNodePC;

// ******************************************************************************** //
// Interface class for all ADT Trees
class BinaryTree: public ADT
{
private:
	void DeleteAll(BinaryTreeNodeP _pNode);

	// Prevent copy constructor and operator = being generated.
	BinaryTree(const BinaryTree&);
	const BinaryTree& operator = (const BinaryTree&);
protected:
	BinaryTreeNodeP m_pRoot;
	virtual BinaryTreeNodeP SearchNode(qword _qwKey)=0;			// Internal standard search with a key (searches for nearest element)

	// Exchange two nodes by changing there 3 connections.
	// The node will keep its address and its data.
	void Swap(BinaryTreeNodeP _pN1, BinaryTreeNodeP _pN2);

	// Replaces the current node by its left child. Does nothing
	// if there is no child. The return value is the new root
	// of the subtree.
	BinaryTreeNodeP RotateRight(BinaryTreeNodeP _pNode);

	// Replaces the current node by its right child. Does nothing
	// if there is no child. The return value is the new root
	// of the subtree.
	BinaryTreeNodeP RotateLeft(BinaryTreeNodeP _pNode);
public:
	BinaryTree():m_pRoot(nullptr) {}
	virtual ~BinaryTree();

	// Only implement the navigation the rest is overwritten by special trees
	virtual BinaryTreeNodeP Search(qword _qwKey) override;					// Standard search with a key
	virtual BinaryTreeNodeP GetFirst() override;
	virtual BinaryTreeNodeP GetLast() override;
	virtual BinaryTreeNodeP GetNext(ADTElementP _pCurrent) override;		// Slow inorder traverse
	virtual BinaryTreeNodeP GetPrevious(ADTElementP _pCurrent) override;	// Slow inorder traverse

	static bool IsLeftChild(BinaryTreeNodePC _pNode)	{return _pNode->pParent?_pNode->pParent->pLeft==_pNode:false;}
	static bool IsRightChild(BinaryTreeNodePC _pNode)	{return _pNode->pParent?_pNode->pParent->pRight==_pNode:false;}
	static BinaryTreeNodeP Min(BinaryTreeNodeP _pNode)	{while(_pNode->pLeft) _pNode = _pNode->pLeft; return _pNode;}
	static BinaryTreeNodeP Max(BinaryTreeNodeP _pNode)	{while(_pNode->pRight) _pNode = _pNode->pRight; return _pNode;}
};
typedef BinaryTree* BinaryTreeP;


}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //