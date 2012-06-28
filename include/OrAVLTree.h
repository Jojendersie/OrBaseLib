// ******************************************************************************** //
// OrAVLTree.h																		//
// ===========																		//
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
//																					//
// ******************************************************************************** //
// AVL: balanced binary search tree.												//
//																					//
// Type of a binary tree with worstcase costs of O(log n) for each Operation.		//
// Idea: The height of the two subtrees of any node differs by at most one.			//
//																					//
// Using BinaryTreeNode.iParam to save the height of the subtree.					//
//																					//
// AVLTree:																			//
//	Insert()					O(log n)											//
//	Search()					O(log n)											//
//	Delete(Key)					O(log n)											//
//	Delete(Element)				O(log n)											//
//	GetFirst()					O(log n)											//
//	GetLast()					O(log n)											//
//	GetNext()					O(log n)											//
//	GetPrevious()				O(log n)											//
//																					//
// LinkedAVLTree:																	//
//	Insert()					O(log n)											//
//	Search()					O(log n)											//
//	Delete(Key)					O(log n)											//
//	Delete(Element)				O(log n)											//
//	GetFirst()					O(log n)											//
//	GetLast()					O(log n)											//
//	GetNext()					O(1)												//
//	GetPrevious()				O(1)												//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

// ******************************************************************************** //
// Override abstract binary Tree
class AVLTree : public BinaryTree
{
protected:
	// Sets the iParam - Height value of _pNode and in the path
	// to the root.
	void RepairHeightProperty(BinaryTreeNodeP _pNode);

	void Rebalance(BinaryTreeNodeP _pNode);							// Rebuild AVL property, if and only if _pNode changed it's height by one. (_pNode has to be the parent of the inserted or deleted node)
	BinaryTreeNodeP SearchNode(qword _qwKey) override;				// Search with a key (searches for nearest element)
	void RecursiveDelete(BinaryTreeNodeP _pNode);					// Internal called from clear
	static int GetHeight(BinaryTreeNodeP _pNode)		{return _pNode?_pNode->iParam:0;}

	// Prevent copy constructor and operator = being generated.
	AVLTree(const AVLTree&);
	const AVLTree& operator = (const AVLTree&);
public:
	AVLTree():BinaryTree()	{}
	BinaryTreeNodeP Insert(void* _pObject, qword _qwKey) override;	// Insert operation
	void Delete(qword _qwKey) override;								// Standard operation delete
	void Delete(ADTElementP _pNode) override;						// Faster delete operation without search
	void Clear() override;											// The remove everything method

	int GetHeight()		{return GetHeight( m_pRoot );}
};

// ******************************************************************************** //
// Advanced node for binary trees (containing a double linked list)
class BinaryTreeLinkedNode: public BinaryTreeNode
{
	// Make nodes uneditable for all users except the trees. So the trees kann return
	// references to non constant nodes without any (large) risk.
	friend class LinkedAVLTree;
private:
	BinaryTreeLinkedNode* pNext;
	BinaryTreeLinkedNode* pPrev;

	BinaryTreeLinkedNode(BinaryTreeLinkedNode* _pParent, void* _pObj, const qword& _qwKey):BinaryTreeNode(_pParent, _pObj, _qwKey), pNext(nullptr), pPrev(nullptr)	{}

	// Prevent copy constructor and operator = being generated.
	BinaryTreeLinkedNode(const BinaryTreeLinkedNode&);
	const BinaryTreeLinkedNode& operator = (const BinaryTreeLinkedNode&);
};
typedef BinaryTreeLinkedNode* BinaryTreeLinkedNodeP;
typedef BinaryTreeLinkedNode const* BinaryTreeLinkedNodePC;

// ******************************************************************************** //
// AVL Tree with more informations in the nodes (double linked list of the nodes)
// Allows faster navigation by iterators.
class LinkedAVLTree : public AVLTree
{
	// Prevent copy constructor and operator = being generated.
	LinkedAVLTree(const LinkedAVLTree&);
	const LinkedAVLTree& operator = (const LinkedAVLTree&);
public:
	LinkedAVLTree():AVLTree()	{}
	BinaryTreeLinkedNodeP Insert(void* _pObject, qword _qwKey) override;	// Insert operation
	void Delete(qword _qwKey) override	{AVLTree::Delete( _qwKey );}		// Standard operation delete
	void Delete(ADTElementP _pNode) override;								// Faster delete operation without search

	BinaryTreeLinkedNodeP GetNext(ADTElementP _pCurrent) override;			// Fast inorder traverse
	BinaryTreeLinkedNodeP GetPrevious(ADTElementP _pCurrent) override;		// Fast inorder traverse
};

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //