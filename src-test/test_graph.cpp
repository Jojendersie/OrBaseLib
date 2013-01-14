// ******************************************************************************** //
// test_graph.cpp																	//
// ==============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::Graph.									//
//																					//
// ******************************************************************************** //

#include <iostream>
#include "../include/OrBaseLib.h"
#include "../include/OrDebug.h"

// Derive a node with positions.
class Node: public OrE::ADT::Graph::Node
{
public:
	OrE::Math::Vec3 m_vPos;

	~Node() {}// OrE::ADT::Graph::Node::~Node();}
};

// Derive a weighted edge type as test case.
class WEdge: public OrE::ADT::Graph::Edge
{
	friend class OrE::ADT::Graph;
protected:
	WEdge( Node* _pS, Node* _pD, bool _bD ) :
		Edge( _pS, _pD, _bD )	{}
public:
	float m_fWeight;
};

void test_graph()
{
	OrE::ADT::Graph MyGraph;
	OrE::Utils::TimeQuerySlot Time0;
	OrE::Utils::TimeQuery( Time0 );
	
	std::cout << "******** Start test OrE::ADT::Graph ********\n";

	// Add 100 (informationless) nodes
	for( int i=0; i<100; ++i )
	{
		Node* pN = MyGraph.AddNode<Node>();
		pN->m_vPos = OrE::Math::Vec3::Random();
	}

	// Connect all of them with weighted edges
	OrE::ADT::Graph::NodeIterator It = MyGraph.GetNodeIterator();
	while( ++It )
	{
		OrE::ADT::Graph::NodeIterator It2 = MyGraph.GetNodeIterator();
		while( ++It2 ) if( It != It2 )
		{
			WEdge* pE = MyGraph.AddEdge<WEdge, Node>( ((Node*)&It), ((Node*)&It2), false );
			pE->m_fWeight = (((Node*)&It)->m_vPos - ((Node*)&It2)->m_vPos).Length();
		}
	}

	std::cout << "\tTime to create fully connected graph with 100 nodes: "
			  << OrE::Utils::TimeQuery( Time0 )*1000.0 << " ms\n";

	// Delete the first node
	It++;
	MyGraph.Delete( &It );

	// Delete another edge
	MyGraph.Delete( ++MyGraph.GetEdgeIterator() );
	
	std::cout << '\n';
}