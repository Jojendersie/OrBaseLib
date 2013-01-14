// ******************************************************************************** //
// OrMST.cpp																		//
// =========																		//
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
// This is the implementation of Prim's MST algorithm.								//
// The runtime is O(e+n log n). The implementation is done with a fibonacci heap.	//
// ******************************************************************************** //

#include "..\..\..\include\OrTypeDef.h"
#include "..\..\..\include\OrAssert.h"
#include "..\..\..\include\OrFastMath.h"
#include "..\..\..\include\OrADTObjects.h"
#include "..\..\..\include\OrHash.h"
#include "..\..\..\include\OrVector2.h"
#include "..\..\..\include\OrVector3.h"
#include "..\..\..\include\OrDebug.h"
#include "..\..\..\include\OrGraph.h"
#include "..\..\..\include\OrHeap.h"

// ******************************************************************************** //
struct PrimNodeInfo
{
	// Minimal distance to the MST (0 if part of it)
	float fDist;

	// The parent element. This points to the one in the original graph!
	OrE::ADT::Mesh::PosNodeP pParent;

	// Corresponding node in the new graph!
	OrE::ADT::Mesh::PosNodeP pNode;

	// The node in the heap to change the priority fast.
	OrE::ADT::HeapNodeP pHeapNode;
	
	PrimNodeInfo() :
		fDist( std::numeric_limits<float>::infinity() ),
		pParent( nullptr )	{}
};

// ******************************************************************************** //
static void CopyNodes( const OrE::ADT::Mesh& _Src, OrE::ADT::Mesh& _Dst, PrimNodeInfo* _pInfoDst )
{
	auto It = _Src.GetNodeIterator();
	uint32 i = 0;
	while( It++ )
	{
		// Set Label in original graph
		OrE::ADT::Mesh::PosNodeP(&It)->m_pTmpLabel = &_pInfoDst[i];

		_pInfoDst[i].pNode = _Dst.AddNode<OrE::ADT::Mesh::PosNode>();
		_pInfoDst[i].pNode->SetPos( OrE::ADT::Mesh::PosNodeP( &It )->GetPos() );
		++i;
	}
}

// ******************************************************************************** //
static void FillHeap( const OrE::ADT::Mesh& _Src, OrE::ADT::Heap& _Dst, PrimNodeInfo* _pInfoDst )
{
	// Initialization depends on number only. Each node is not part of any tree.
	auto It = _Src.GetNodeIterator();
	uint32 i = 0;
	while( It++ )
	{
		// Correspondence should not be changed since CopyNode. Check for multithreading
		// caused node insertions/deletions.
		Assert( OrE::ADT::Mesh::PosNodeP(&It)->m_pTmpLabel == &_pInfoDst[i] );
		// Heap
		_pInfoDst[i].pHeapNode = _Dst.Insert( &It, std::numeric_limits<uint64>::max() );
		++i;
	}
}

// TEST
class RndIterator: public OrE::ADT::_Iterator<int, RndIterator>
{
	operator bool () const	{ return true; }
	operator int* () const	{ return nullptr; }
	bool operator == (const RndIterator& T) const { return true; }

	RndIterator operator + (const int64 _rng ) const { return *this; }
};

// ******************************************************************************** //
// Compute the minimal spanning tree for this graph. The result is written to a copy.
OrE::ADT::Mesh OrE::ADT::Mesh::BuildMST() const
{
	// Initialize the distances for all nodes and create a forest in an
	// index structure. (For fast random access; fast heap node access)
	PrimNodeInfo* aInfos = new PrimNodeInfo[GetNumNodes()];

	// Create the new graph as copy
	Mesh M;
	CopyNodes( *this, M, aInfos );
	
	// Fill the Fibonacci heap with all nodes whether they are connected or not.
	Heap FibHeap;
	FillHeap( *this, FibHeap, aInfos );

	// For all nodes
	while( FibHeap.Min() )
	{
		PosNodeP pPivot = reinterpret_cast<PosNodeP>(FibHeap.DeleteMin());
		// If the node has infinite distance it is not part of any tree -> create
		// a new one with only this node. This happens implicit because it has no
		// parent, but updates all neighbours.
		PrimNodeInfo* pPivotInfo = (PrimNodeInfo*)pPivot->m_pTmpLabel;
		pPivotInfo->fDist = 0.0f;
		// Update all neighbour points
		auto AOutIt = GetOutEdgeIterator( pPivot );
		while( AOutIt++ )
		{
			float fDist = WeightedEdgeP( &AOutIt )->GetWeight();
			PosNodeP pNeighboutNode = PosNodeP( WeightedEdgeP( &AOutIt )->GetOther(pPivot) );
			PrimNodeInfo* pNeighbour = ((PrimNodeInfo*)pNeighboutNode->m_pTmpLabel);
			if( pNeighbour->fDist > fDist )
			{
				// This edge is shorter than the one used before.
				pNeighbour->fDist = fDist;
				pNeighbour->pParent = pPivot;
				FibHeap.ChangeKey( pNeighbour->pHeapNode, uint64( fDist * 1000000.0 ) );
			}
		}

		// Create a new edge between parent and the new node in the Graph copy.
		M.AddEdge< WeightedEdge, PosNode >(
			((PrimNodeInfo*)pPivotInfo->pParent->m_pTmpLabel)->pNode,
			pPivotInfo->pNode,
			pPivotInfo->pParent->GetEdge( pPivot )->IsDirected() );

		pPivot->m_pTmpLabel = nullptr;
	}

	delete[] aInfos;
	return M;
}

// ******************************************************************************** //