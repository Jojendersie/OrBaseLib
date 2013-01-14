// ******************************************************************************** //
// OrGraph.h																		//
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
//																					//
// ******************************************************************************** //
// A simple graph.																	//
// Each node can have arbitrary parameters and functions. Therefor a Graph subclass	//
// has to be implemented.															//
// Undirected and directed graphs, or even mixed ones are possible.					//
// ******************************************************************************** //

#pragma once

#include <vector>

namespace OrE {
namespace ADT {

	// ******************************************************************************** //
	class Graph
	{
	public:
		class Edge;
		typedef Edge* EdgeP;
		typedef const Edge* EdgeCP;
		class Node;
		typedef Node* NodeP;
		typedef const Node* NodeCP;

		// ******************************************************************************** //
		// The node class with basic functionality for a graph.
		class Node
		{
		private:
			// The internal hash map maps all adjacent nodes to edges. These can be
			// ingoing, outgoing or undirected edges.
			// Node* -> Edge*
			HashMap m_Adjacence;

			// Statistical information for the node.
			// Ingoing edges and undirected edges
			uint32 m_uiInDegree;
			// Outgoing edges and undirected edges
			uint32 m_uiOutDegree;
			// m_uiInDegree + m_uiOutDegree - undirected edges
			uint32 m_uiDegree;

			friend class Graph;
		public:
			Node() : m_Adjacence( 8, HM_RESIZE_MODERATE )	{}

			// Virtual default destructor.
			virtual ~Node()	{}

			// Ingoing edges and undirected edges
			uint32 GetInDegree() const	{ return m_uiInDegree; }

			// Outgoing edges and undirected edges
			uint32 GetOutDegree() const	{ return m_uiOutDegree; }

			// Number of adjacent nodes: m_uiInDegree + m_uiOutDegree - undirected edges
			uint32 GetDegree() const	{ return m_uiDegree; }

			// Checks if a node is adjacent to this one. The edge direction is
			// ignored.
			bool IsAdjacentTo( NodeCP _pNode ) const;

			// Checks, if there is an edge from '_pNode' to 'this'. This can be an
			// undirected or directed edge. Directed edges with src='this' are ignored.
			bool IsSuccessorOf( NodeCP _pNode ) const;

			// Checks, if there is an edge from 'this' to '_pNode'. This can be an
			// undirected or directed edge. Directed edges with src='_pNode' are ignored.
			bool IsPredecessorOf( NodeCP _pNode ) const;

			// Return the edge object between two nodes. If there is no edge with this
			// node as source the method returns nullptr.
			EdgeP GetEdge( NodeCP _pNode );
			EdgeCP GetEdge( NodeCP _pNode ) const;
		};

		// ******************************************************************************** //
		// The edge class with basic functionality for directed and undirected graphs.
		class Edge
		{
		private:
			NodeP m_pSource;
			NodeP m_pDestination;
			bool m_bDirected;
			
			friend class Graph;

		protected:
			Edge( NodeP _pSrc, NodeP _pDst, bool _bDirected ) :
					m_pSource( _pSrc ),
					m_pDestination( _pDst ),
					m_bDirected( _bDirected )	{}
		public:
			// Virtual default destructor.
			virtual ~Edge()	{}

			NodeP GetSrc()				{return m_pSource;}
			NodeP GetDst()				{return m_pDestination;}
			NodeCP GetSrc() const		{return m_pSource;}
			NodeCP GetDst() const		{return m_pDestination;}

			// Return the other node if the edge, which is not the current one.
			// If the edge goes from _pN to _pN (selfloop) than _pN is returned.
			NodeP GetOther( NodeCP _pN )
			{
				Assert( _pN == m_pSource || _pN == m_pDestination );

				return (_pN == m_pSource ) ? m_pDestination : m_pSource;
			}
			
			// For undirected edges a node can be source and destination
			// respectivly (always true).
			bool IsSrc( NodeCP _pNode ) const		{ return m_bDirected ? m_pSource == _pNode : true; }
			bool IsDst( NodeCP _pNode ) const		{ return m_bDirected ? m_pDestination == _pNode : true; }
			bool IsDirected() const					{ return m_bDirected; }
		};
		
	private:
		// Lists of Edge and Node data. These lists store each Edge and Node
		// exactly once. These lists are used to iterate and to clear the memory
		// in the graph destructor.
		// TODO: Test performance with Hashmaps for dynamic and static case.
		std::vector<NodeP> m_Nodes;
		std::vector<EdgeP> m_Edges;
		
		// Delete copy constructor
		Graph( const Graph& );
		Graph& operator = (const Graph&);
	public:
		// Constructor creates a graph with preallocated space for _uiNum nodes and edges.
		// The graph can grow beyond this number or remain smaller. The speed gets
		// better, if the number is already known. Than no resizing is necessary
		Graph( uint32 _uiNumNodes = 32, uint32 _uiNumEdges = 64 ) : 
			m_Nodes(),
			m_Edges()
		{
			m_Nodes.reserve( _uiNumNodes );
			m_Edges.reserve( _uiNumEdges );
		}

		// Destructor deletes all edges, nodes and adjacence objects. Each reference
		// to a node or edge is invalid afterwards.
		virtual ~Graph();

		// ******************************************************************************** //
		// Graph modification methods

		// Create a new node and return its reference. 0 if out of memory.
		template<typename _NodeType> _NodeType* AddNode()
		{
			// If you end here with the error 'no appropriate default constructor available' or
			// similar you have added some data values to the nodes declaration., which have no
			// default constructor. Therefor the compiler cannot create a default ctor for 
			// _NodeType. Implement it manually.
			_NodeType* pN = new _NodeType;
			// This line causes an error, if your _NodeType is not derived from Graph::Node!
			m_Nodes.push_back( pN );
			return pN;
		}

		// Create a new edge and return its reference. 0 if out of memory.
		// This does not check for other existing edges. There are cases where the graph
		// can have multiple edges between two nodes. If you wanna have unique edges check
		// with ... before. TODO: aktuell ist nur eine Kante möglich!
		template<typename _EdgeType, typename _NodeType> _EdgeType* AddEdge( _NodeType* _pNSrc, _NodeType* _pNDst, bool _bDirected = true )
		{
			// If you end here with the error 'no constructor available' or 'function does not
			// take...' or similar you have not implemented the 3-Param-Edge-Ctor for your
			// derived class, or _EdgeType is even not of type Edge.
			_EdgeType* pR = new _EdgeType( _pNSrc, _pNDst, _bDirected );
			// This line causes an error, if your _EdgeType is not derived from Graph::Edge!
			EdgeP pE = pR;
			m_Edges.push_back( pE );
			// Add to adjacence lists of both nodes.
			_pNSrc->m_Adjacence.Insert( pE, uint64( _pNDst ) );
			++_pNSrc->m_uiDegree; ++_pNSrc->m_uiOutDegree;
			_pNDst->m_Adjacence.Insert( pE, uint64( _pNSrc ) );
			++_pNDst->m_uiDegree; ++_pNDst->m_uiInDegree;
			if( !_bDirected ) {
				++_pNSrc->m_uiInDegree;
				++_pNDst->m_uiOutDegree;
			}
			return pR;
		}

		// Remove a node and all edges to that node.
		// This is currently very slow: O(n*Degree)
		void Delete( Node* _pNode );

		// Remove a single edge
		// This is currently very slow: O(n)
		void Delete( Edge* _pEdge );

		// ******************************************************************************** //
		// Node and edges properties
		uint32 GetNumNodes() const		{ return uint32(m_Nodes.size()); }
		uint32 GetNumEdges() const		{ return uint32(m_Edges.size()); }

		// Index access to the nodes.
		NodeP GetNode( uint32 _uiIdx )			{ return _uiIdx<m_Nodes.size() ? m_Nodes[_uiIdx] : nullptr; }
		NodeCP GetNode( uint32 _uiIdx ) const	{ return _uiIdx<m_Nodes.size() ? m_Nodes[_uiIdx] : nullptr; }

		// ******************************************************************************** //
		// Iterators
		// All iterators behave exactly the same way as the container OrE::ADT::Iterator,
		// except the initialization. Write Graph::...Iterator = G.Get...Iterator(); for
		// initialization.

		// Nodes
		class NodeIterator
		{
		private:
			uint32 m_uiIndex;
			const Graph* m_G;

			NodeIterator( const Graph* _pGraph ) : m_uiIndex(_pGraph->GetNumNodes()), m_G(_pGraph)	{}
			friend class Graph;

		public:
			operator bool () const		{ return m_uiIndex >= 0 && m_uiIndex<m_G->GetNumNodes(); }

			// The functions are unsafe/they crash if Iterator not valid.
			// Make sure your iterator is valid with if(iter).
			operator Node* () const		{ return m_G->m_Nodes[m_uiIndex]; }
			Node& operator*() const		{ return *m_G->m_Nodes[m_uiIndex]; }
			Node* operator->() const	{ return m_G->m_Nodes[m_uiIndex]; }
			Node* operator&() const		{ return m_G->m_Nodes[m_uiIndex]; }

			bool operator==(const NodeIterator& T) const	{ return m_G == T.m_G && m_uiIndex == T.m_uiIndex; }
			bool operator!=(const NodeIterator& T) const	{ return m_G != T.m_G || m_uiIndex != T.m_uiIndex; }

			NodeIterator& operator++()			{ m_uiIndex = (m_uiIndex+1) % (m_G->GetNumNodes()+1); return *this; }
			NodeIterator& operator--()			{ m_uiIndex = Math::Min(m_uiIndex-1, m_G->GetNumNodes()); return *this; }
			const NodeIterator operator++(int)	{ NodeIterator temp = *this; ++*this; return temp; }
			const NodeIterator operator--(int)	{ NodeIterator temp = *this; --*this; return temp; }
		};

		NodeIterator GetNodeIterator() const	{ return NodeIterator( this ); }

		// Edges of the graph
		class EdgeIterator
		{
		private:
			uint32 m_uiIndex;
			const Graph* m_G;

			EdgeIterator( const Graph* _pGraph ) : m_uiIndex(_pGraph->GetNumEdges()), m_G(_pGraph)	{}
			friend class Graph;

		public:
			operator bool () const		{ return m_uiIndex >= 0 && m_uiIndex<m_G->GetNumEdges(); }

			// The functions are unsafe/they crash if Iterator not valid.
			// Make sure your iterator is valid with if(iter).
			operator Edge* () const		{ return m_G->m_Edges[m_uiIndex]; }
			Edge& operator*() const		{ return *m_G->m_Edges[m_uiIndex]; }
			Edge* operator->() const	{ return m_G->m_Edges[m_uiIndex]; }
			Edge* operator&() const		{ return m_G->m_Edges[m_uiIndex]; }

			bool operator==(const EdgeIterator& T) const	{ return m_G == T.m_G && m_uiIndex == T.m_uiIndex; }
			bool operator!=(const EdgeIterator& T) const	{ return m_G != T.m_G || m_uiIndex != T.m_uiIndex; }

			EdgeIterator& operator++()			{ m_uiIndex = (m_uiIndex+1) % (m_G->GetNumEdges()+1); return *this; }
			EdgeIterator& operator--()			{ m_uiIndex = Math::Min(m_uiIndex-1, m_G->GetNumEdges()); return *this; }
			const EdgeIterator operator++(int)	{ EdgeIterator temp = *this; ++*this; return temp; }
			const EdgeIterator operator--(int)	{ EdgeIterator temp = *this; --*this; return temp; }

			// TODO +, - operator
		};

		EdgeIterator GetEdgeIterator() const	{ return EdgeIterator( this ); }

		// Outgoing edges of a single node. Outgoing edges are all which have
		// the node as source (subset of directed + all undirected).
		// TODO: move to node???
		class OutEdgeIterator
		{
		private:
			HashMap::Iterator I;
			NodeCP m_pNode;

			OutEdgeIterator( NodeP _pNode ) : I(&_pNode->m_Adjacence), m_pNode(_pNode)	{}
			friend class Graph;

		public:
			operator bool () const		{ return bool( I ); }

			// The functions are unsafe/they crash if Iterator not valid.
			// Make sure your iterator is valid with if(iter).
			operator Edge* () const		{ return EdgeP( I->pObject ); }
			Edge& operator*() const		{ return *EdgeP( I->pObject ); }
			Edge* operator->() const	{ return EdgeP( I->pObject ); }
			Edge* operator&() const		{ return EdgeP( I->pObject ); }

			bool operator==(const OutEdgeIterator& T) const	{ return I == T.I; }
			bool operator!=(const OutEdgeIterator& T) const	{ return I != T.I; }

			OutEdgeIterator& operator++()			{ do ++I; while( I && !EdgeP(I->pObject)->IsSrc(m_pNode) ); return *this; }
			OutEdgeIterator& operator--()			{ do --I; while( I && !EdgeP(I->pObject)->IsSrc(m_pNode) ); return *this; }
			const OutEdgeIterator operator++(int)	{ OutEdgeIterator temp = *this; ++*this; return temp; }
			const OutEdgeIterator operator--(int)	{ OutEdgeIterator temp = *this; --*this; return temp; }

			// TODO +, - operator
		};
		
		static OutEdgeIterator GetOutEdgeIterator( NodeP _pNode )	{ return OutEdgeIterator( _pNode ); }
		// TODO: ingoing edges
		// TODO: all adjacent edges exactly once. In the case there is an edge
		// in both directions only the one starting at v is enumerated.
	};







	// ******************************************************************************** //
	// The Mesh is a graph, where each node has a position and each edge has a weight,
	// which safes the distance between the two node's positions.
	class Mesh: public Graph
	{
	private:
		// Delete copy constructor
		Mesh( const Mesh& );
		Mesh& operator = (const Mesh&);

	public:
		class WeightedEdge;
		typedef WeightedEdge* WeightedEdgeP;
		typedef const WeightedEdge* WeightedEdgeCP;
		class PosNode;
		typedef PosNode* PosNodeP;
		typedef const PosNode* PosNodeCP;

		// ******************************************************************************** //
		// The node class with basic functionality for a graph.
		class PosNode: public Node
		{
		private:
			Math::Vec3 m_vPos;

			friend class Mesh;

		public:
			// This label is always undefined. It can be used for any graph algorithm.
			// If you call any graph algorithm assume this will be overwritten (It is mentioned
			// if a call changes the labels).
			void* m_pTmpLabel;

		public:

			PosNode() : Node()	{}

			// Virtual default destructor.
			virtual ~PosNode()	{}

			const Math::Vec3& GetPos() const	{ return m_vPos; }

			// Sets the new position and updates all adjacent edge weights
			void SetPos( const Math::Vec3& _vPos );
		};

		// ******************************************************************************** //
		// The edge class with basic functionality for directed and undirected graphs.
		class WeightedEdge: public Edge
		{
		private:
			float m_fWeight;

			WeightedEdge( PosNodeP _pSrc, PosNodeP _pDst, bool _bDirected ) :
				Edge( _pSrc, _pDst, _bDirected )
			{
				m_fWeight = (_pSrc->GetPos() - _pDst->GetPos()).Length();
			}
			
			friend class Graph;
			friend class Mesh;
			friend class PosNode;
		public:
			// Virtual default destructor.
			virtual ~WeightedEdge()	{}

			float GetWeight() const		{ return m_fWeight; }
		};
		

		// Constructor creates a graph with preallocated space for _uiNum nodes and edges.
		// The graph can grow beyond this number or remain smaller. The speed gets
		// better, if the number is already known. Than no resizing is necessary
		Mesh( uint32 _uiNumNodes = 32, uint32 _uiNumEdges = 64 ) : 
			Graph( _uiNumNodes, _uiNumEdges )
		{}

		// Destructor deletes all edges, nodes and adjacence objects. Each reference
		// to a node or edge is invalid afterwards.
		virtual ~Mesh();

		// ******************************************************************************** //
		// Graph modification methods

		// Compute the minimal spanning tree for this graph. The result is written to a copy.
		// The graph must not be connected. If there are more than one cluster the MST for
		// each cluster is created.
		// The algorithm changes m_pTmpLabel. It is set to 0 afterwards.
		// Runtime: O(e+n log n)	(Prim; uses fibonacci heap)
		Mesh BuildMST() const;

		// ******************************************************************************** //
		// Iterators
		// All iterators behave exactly the same way as the container OrE::ADT::Iterator,
		// except the initialization. Write Graph::...Iterator = G.Get...Iterator(); for
		// initialization.
	};

};	// namespace ADT
};	// namespace OrE