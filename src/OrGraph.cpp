// ******************************************************************************** //
// OrGraph.cpp																		//
// ===========																		//
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

#include "../include/OrTypeDef.h"
#include "../include/OrAssert.h"
#include "../include/OrFastMath.h"
#include "../include/OrADTObjects.h"
#include "../include/OrHash.h"
#include "../include/OrVector2.h"
#include "../include/OrVector3.h"
#include "../include/OrDebug.h"
#include "../include/OrGraph.h"


// ******************************************************************************** //
// Checks if a node is adjacent to this one. The edge direction is ignored.
bool OrE::ADT::Graph::Node::IsAdjacentTo( NodeCP _pNode ) const
{
	return nullptr != m_Adjacence.Search( uint64( _pNode ) );
}

// ******************************************************************************** //
// Checks, if there is an edge from '_pNode' to 'this'. This can be an
// undirected or directed edge. Directed edges with src='this' are ignored.
bool OrE::ADT::Graph::Node::IsSuccessorOf( NodeCP _pNode ) const
{
	EdgeCP pE = (EdgeCP)m_Adjacence.Search( uint64( _pNode ) )->pObject;
	return (pE!=nullptr) && (!pE->IsDirected() || pE->IsSrc( _pNode ));
}

// ******************************************************************************** //
// Checks, if there is an edge from 'this' to '_pNode'. This can be an
// undirected or directed edge. Directed edges with src='_pNode' are ignored.
bool OrE::ADT::Graph::Node::IsPredecessorOf( NodeCP _pNode ) const
{
	EdgeCP pE = (EdgeCP)m_Adjacence.Search( uint64( _pNode ) )->pObject;
	return (pE!=nullptr) && (!pE->IsDirected() || pE->IsDst( _pNode ));
}


// ******************************************************************************** //
// Return the edge object between two nodes. If there is no edge this
// method returns nullptr.
OrE::ADT::Graph::EdgeP OrE::ADT::Graph::Node::GetEdge( NodeCP _pNode )
{
	EdgeP pE = (EdgeP)m_Adjacence.Search( uint64( _pNode ) )->pObject;
	if( pE && pE->IsSrc( this ) )
		return pE;
	else return nullptr;
}

OrE::ADT::Graph::EdgeCP OrE::ADT::Graph::Node::GetEdge( NodeCP _pNode ) const
{
	EdgeCP pE = (EdgeCP)m_Adjacence.Search( uint64( _pNode ) )->pObject;
	if( pE && pE->IsSrc( this ) )
		return pE;
	else return nullptr;
}



// ******************************************************************************** //
// Destructor deletes all edges, nodes and adjacence objects.
OrE::ADT::Graph::~Graph()
{
	// Go through the pointer array and delete everything
	for( unsigned int i=0; i<m_Nodes.size(); ++i )
		delete m_Nodes[i];

	for( unsigned int i=0; i<m_Edges.size(); ++i )
		delete m_Edges[i];
}


// ******************************************************************************** //
// Remove a node and all edges to that node.
void OrE::ADT::Graph::Delete( Node* _pNode )
{
	// Iterate over all neighbours and delete the edges to this node
	OrE::ADT::HashMap::Iterator It( &_pNode->m_Adjacence );
	while( ++It )
	{
		Edge* pE = (Edge*)It->pObject;
		Node* _pAdjNode = pE->GetSrc()==_pNode ? pE->GetDst() : pE->GetSrc();
		// Delete edge from adjacence list and decrease the adjacence count.
		_pAdjNode->m_Adjacence.Delete( uint64(_pNode) );
		--_pAdjNode->m_uiDegree;
		if( !pE->IsDirected() ) { --_pAdjNode->m_uiInDegree; --_pAdjNode->m_uiOutDegree; }
		else if( pE->IsSrc(_pAdjNode) ) { --_pAdjNode->m_uiOutDegree; }
		else --_pAdjNode->m_uiInDegree;
	}

	// Delete the edge objects to this node from m_Edges
	// O(n*e)!
	for( int i=GetNumEdges()-1; i>=0; --i )
	{
		if( m_Edges[i]->GetDst() == _pNode || m_Edges[i]->GetSrc() == _pNode )
		{
			delete m_Edges[i];
			m_Edges.erase( m_Edges.begin()+i );
		}
	}

	// Delete the node itself
	// O(n)!
	for( int i=GetNumNodes()-1; i>=0; --i )
	{
		if( m_Nodes[i] == _pNode )
		{
			delete m_Nodes[i];
			m_Nodes.erase( m_Nodes.begin()+i );
			break;
		}
	}
}


// ******************************************************************************** //
// Remove a single edge
void OrE::ADT::Graph::Delete( Edge* _pEdge )
{
	// Delete from reference list from both nodes.
	_pEdge->GetSrc()->m_Adjacence.Delete( uint64(_pEdge->GetDst()) );
	_pEdge->GetDst()->m_Adjacence.Delete( uint64(_pEdge->GetSrc()) );

	// Make degrees correct
	--_pEdge->GetSrc()->m_uiDegree;  --_pEdge->GetSrc()->m_uiOutDegree;
	--_pEdge->GetDst()->m_uiDegree;  --_pEdge->GetDst()->m_uiInDegree;
	if( !_pEdge->IsDirected() )
	{
		--_pEdge->GetSrc()->m_uiInDegree;
		--_pEdge->GetDst()->m_uiOutDegree;
	}

	// Delete the edge objects from m_Edges
	// O(n)!
	for( int i=GetNumEdges()-1; i>=0; --i )
	{
		if( m_Edges[i] == _pEdge )
		{
			delete m_Edges[i];
			m_Edges.erase( m_Edges.begin()+i );
		}
	}
}



// ******************************************************************************** //
// Part of Mesh graph

// ******************************************************************************** //
OrE::ADT::Graph::NodeP OrE::ADT::Mesh::PosNode::CopyTo( Graph& _pNewGraph ) const
{
	PosNodeP pNew = _pNewGraph.AddNode<PosNode>();
	pNew->m_vPos = m_vPos;
	return pNew;
}

// ******************************************************************************** //
void OrE::ADT::Mesh::PosNode::SetPos( const Math::Vec3& _vPos )
{
	if( m_vPos != _vPos )
	{
		m_vPos = _vPos;
		//TODO auto It = Edges updaten
	}
}