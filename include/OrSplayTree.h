// ******************************************************************************** //
// OrSplayTree.h																	//
// =============																	//
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
// Splay Tree: self organizing search tree.											//
//																					//
// Type of a binary tree with average costs of O(log n) for each Operation.			//
// Idea: Splay the last seen node to the root. Where splay is a speciall			//
//	rotation case.																	//
//																					//
// Implementation: Bottom-Up-Splaying												//
//																					//																					//
//	Insert()					O(log n)											//
//	Search()					O(log n)											//
//	Delete(Key)					O(log n)											//
//	Delete(Element)				O(log n)											//
//	GetFirst()					O(log n)											//
//	GetLast()					O(log n)											//
//	GetNext()					O(log n)											//
//	GetPrevious()				O(log n)											//
// ******************************************************************************** //

#pragma once

namespace OrE {
namespace ADT {

	// ******************************************************************************** //
	// Override abstract binary Tree
	class SplayTree : public BinaryTree
	{
	private:
		// Prevent copy constructor and operator = being generated.
		SplayTree(const SplayTree&);
		const SplayTree& operator = (const SplayTree&);
	protected:
		void Splay(BinaryTreeNodeP _pNode);										// Splay: reorganization through rotations (move the node to the root)
		void SemiSplay(BinaryTreeNodeP _pNode);									// Semi-Splay: reorganization through rotations (half the path length to the root)
		virtual BinaryTreeNodeP SearchNode(qword _qwKey) override;				// Search with a key and splay (searches for nearest element)
	public:
		SplayTree():BinaryTree()	{}
		virtual BinaryTreeNodeP Insert(void* _pObject, qword _qwKey) override;	// Insert operation with splay
		virtual void Delete(qword _qwKey) override;								// Standard operation delete
		virtual void Delete(ADTElementP _pNode) override;						// Delete operation without search (not much faster)
	};

}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //