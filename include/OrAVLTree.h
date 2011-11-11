// ******************************************************************************** //
// AVL: balanced binary search tree.												//
//																					//
// Type of a binary tree with worstcase costs of O(log n) for each Operation.		//
// Idea: The height of the two subtrees of any node differs by at most one.			//
//																					//
// Using OrBinaryTreeNode.iParam to save the height of the subtree.					//
// NOT TESTED
// ******************************************************************************** //

#pragma once

// ******************************************************************************** //
// Override abstract binary Tree
class OrAVLTree : public OrBinaryTree
{
protected:
	void Rebalance(OrBinaryTreeNodeP _pNode);				// Rebuild AVL property, if and only if _pNode changed it's height by one. (_pNode has to be the parent of the inserted or deleted node)
	OrBinaryTreeNodeP SearchNode(qword _qwKey) override;				// Search with a key (searches for nearest element)
public:
	OrAVLTree():OrBinaryTree()	{}
	OrBinaryTreeNodeP Insert(void* _pObject, qword _qwKey) override;	// Insert operation
	void Delete(qword _qwKey) override;								// Standard operation delete
	void Delete(OrADTElementP _pNode) override;					// Faster delete operation without search

	static int GetHeight(OrBinaryTreeNodeP _pNode)		{return _pNode?_pNode->iParam:0;}
};

// ******************************************************************************** //
// Advanced node for binary trees (containing a double linked list)
struct OrBinaryTreeLinkedNode: public OrBinaryTreeNode
{
	// Make nodes uneditable for all users except the trees. So the trees kann return
	// references to non constant nodes without any (large) risk.
	friend class OrLinkedAVLTree;
private:
	OrBinaryTreeLinkedNode* pNext;
	OrBinaryTreeLinkedNode* pPrev;

	OrBinaryTreeLinkedNode(OrBinaryTreeLinkedNode* _pParent, void* _pObj, const qword& _qwKey):OrBinaryTreeNode(_pParent, _pObj, _qwKey), pNext(nullptr), pPrev(nullptr)	{}
};
typedef OrBinaryTreeLinkedNode* OrBinaryTreeLinkedNodeP;
typedef OrBinaryTreeLinkedNode const* OrBinaryTreeLinkedNodePC;

// ******************************************************************************** //
// AVL Tree with more informations in the nodes (double linked list of the nodes)
// Allows faster navigation by iterators.
class OrLinkedAVLTree : public OrAVLTree
{
public:
	OrLinkedAVLTree():OrAVLTree()	{}
	virtual OrBinaryTreeLinkedNodeP Insert(void* _pObject, qword _qwKey) override;	// Insert operation
	virtual void Delete(OrADTElementP _pNode) override;								// Faster delete operation without search

	virtual OrBinaryTreeLinkedNodeP GetNext(OrADTElementP _pCurrent) override;		// Fast inorder traverse
	virtual OrBinaryTreeLinkedNodeP GetPrevious(OrADTElementP _pCurrent) override;	// Fast inorder traverse
};

// *************************************EOF**************************************** //