#pragma once

// ******************************************************************************** //
// Simple node for all binary trees
struct OrBinaryTreeNode: public OrADTElement
{
	// Make nodes uneditable for all users except the trees. So the trees kann return
	// references to non constant nodes without any (large) risk.
	friend class OrBinaryTree;
	friend class OrSplayTree;
	friend class OrAVLTree;
	friend class OrLinkedAVLTree;
	friend class OrHuffmanTree_Splay;
	friend class OrHuffmanTree_SemiSplay;
protected:
	OrBinaryTreeNode* pLeft;
	OrBinaryTreeNode* pRight;
	OrBinaryTreeNode* pParent;
	int iParam;					// Statistic information to mark nodes (usecase differs on application)

	OrBinaryTreeNode(OrBinaryTreeNode* _pParent, void* _pObj, const qword& _qwKey):OrADTElement(_pObj, _qwKey), pLeft(nullptr),pRight(nullptr),pParent(_pParent)	{}
};
typedef OrBinaryTreeNode* OrBinaryTreeNodeP;
typedef OrBinaryTreeNode const * OrBinaryTreeNodePC;

// ******************************************************************************** //
// Interface class for all ADT Trees
class OrBinaryTree: public OrADT
{
private:
	void DeleteAll(OrBinaryTreeNodeP _pNode);
protected:
	OrBinaryTreeNodeP m_pRoot;
	OrADTDeleteObjectCallbackP m_pDeleteCallback;
	virtual OrBinaryTreeNodeP SearchNode(qword _qwKey)=0;			// Internal standard search with a key (searches for nearest element)

	void Swap(OrBinaryTreeNodeP _pN1, OrBinaryTreeNodeP _pN2);		// Exchange two nodes
	OrBinaryTreeNodeP RotateRight(OrBinaryTreeNodeP _pNode);		// Replaces the current node by its left child. Does nothing if there is no child.
	OrBinaryTreeNodeP RotateLeft(OrBinaryTreeNodeP _pNode);			// Replaces the current node by its right child. Does nothing if there is no child.
public:
	OrBinaryTree():m_pRoot(nullptr),m_pDeleteCallback(nullptr) {}
	~OrBinaryTree();

	// Only implement the navigation the rest is overwritten by special trees
	virtual OrBinaryTreeNodeP Search(qword _qwKey) override;		// Standard search with a key
	virtual OrBinaryTreeNodeP GetFirst() override;
	virtual OrBinaryTreeNodeP GetLast() override;
	virtual OrBinaryTreeNodeP GetNext(OrADTElementP _pCurrent) override;		// Slow inorder traverse
	virtual OrBinaryTreeNodeP GetPrevious(OrADTElementP _pCurrent) override;	// Slow inorder traverse

	static bool IsLeftChild(OrBinaryTreeNodePC _pNode)		{return _pNode->pParent?_pNode->pParent->pLeft==_pNode:false;}
	static bool IsRightChild(OrBinaryTreeNodePC _pNode)		{return _pNode->pParent?_pNode->pParent->pRight==_pNode:false;}
	static OrBinaryTreeNodeP Min(OrBinaryTreeNodeP _pNode)	{while(_pNode->pLeft) _pNode = _pNode->pLeft; return _pNode;}
	static OrBinaryTreeNodeP Max(OrBinaryTreeNodeP _pNode)	{while(_pNode->pRight) _pNode = _pNode->pRight; return _pNode;}
	void SetDeleteCallback(OrADTDeleteObjectCallbackP _pDeleteCallback)	{m_pDeleteCallback = _pDeleteCallback;}
};
typedef OrBinaryTree* OrBinaryTreeP;


// *************************************EOF**************************************** //