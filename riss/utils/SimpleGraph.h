/*
 * Graph.h, LGPL v2, see LICENSE
 */

#ifndef RISS_COPROCESSOR_SIMPLEGRAPH_H_
#define RISS_COPROCESSOR_SIMPLEGRAPH_H_

#include <iterator>
#include <iostream>
#include <vector>
#include "riss/core/SolverTypes.h"
#include <set>



//using namespace std;

typedef std::pair<int, double> edge;
typedef std::vector<edge> adjacencyList;

/**
 * undirected weighted graph.
 */


class SimpleGraph
{
  private:
    void doPageRank();
    //int recursive_treewidth(const std::vector<int>& Lset, const std::vector<int>& component);
    //void compute_G_plus(SimpleGraph*& Graph_plus, const std::vector<int>& set);
    std::vector<std::vector<int>> getConnectedComponents(const std::vector<int>& set);
    void getallcombinations(const std::vector<int>& nodes,std::vector<int> set,std::vector<std::vector<int>>& sets, int k, int position);
   // bool improved_recursive_treewidth(int k);
    void findtree(std::vector<std::pair<std::vector<int>, std::vector<int>>>& bags, Riss::MarkArray& visited);
    std::vector<std::vector<int>> getSets(const std::vector<int>& nodes, int k);
    void controllbagsandadd(std::vector<std::pair<std::vector<int>, std::vector<int>>>& bags, Riss::MarkArray& visited);
    

    bool mergeAtTheEnd; // do not detect duplicate entries during the creation of the graph
    bool intermediateSort;  // remove duplicates in adjacency lists already during the algorithm execution
    int intermediateSorts; // statistics
    bool addedDirectedEdge, addedUndirectedEdge; // bools to make sure in one graph not two different edge types have been added
    std::vector<double> getDistances(int nod);
    /** sort the adjacencyList and remove duplicate entries */
    uint64_t sortAdjacencyList(adjacencyList& aList);
   
   
    
    
    
    
    
protected:
    std::vector <double> narity;
	size_t size;
    std::vector<adjacencyList> node;
    std::vector<int> nodeDeg;
    std::vector<std::set<int>> communityneighbors;
    std::vector<std::vector<int>> bridgenodes;
    double precision = 0.000001;
    //vectors for communityclassifying
    std::vector<std::vector<int>> comm;
    std::vector<int> n2c;
    std::vector<double> pagerank;
    std::vector<int> articulationpoints;

  public:
    void computeCommunityNeighbors();
    void computeCommunityBridgeNodes();
    std::vector<std::vector<int>> getCommunityForEachNode(double prec);
    void getCommunities(double precision);
    void getDimension();
    double getWeight(int nodeA, int nodeB);
    double arity(int x);
    double arity();
    double getPageRank(int node);
    //int gettreewidth();
    void DepthFirstSearch(std::vector<int>& stack, Riss::MarkArray& visited, std::vector<int>& articulationspoints);
    bool thereisanedge(int nodeA, int nodeB);
    std::vector<int> getArticulationPoints();
    double getExzentricity(int nod);
    double getRadius();
    double getDiameter();
    void completeSingleVIG();
    
    std::vector<int> getAdjacency(int adjnode); /// collect all edges without weight (performs copy!)
    void getAdjacency(int adjnode, std::vector<int>& adj); /// collect all edges without weight (performs copy, but uses the given vector)
    
    SimpleGraph(size_t nodes, bool computingDerivative);
    SimpleGraph(size_t nodes, bool merge, bool computingDerivative);
    virtual ~SimpleGraph();
    void addUndirectedEdge(int nodeA, int nodeB);
    void addUndirectedEdge(int nodeA, int nodeB, double weight);
    void addDirectedEdge(int nodeA, int nodeB, double weight);
    void addDirectedEdgeAndInvertedEdge(int nodeA, int nodeB, double weight);
    uint64_t addAndCountUndirectedEdge(int nodeA, int nodeB, double weight);
    int getDegree(int node);
    
    std::vector<int> sortSize;  // for each adjacencyList store the size when its re-sorted
    void setIntermediateSort(bool newValue);

    uint64_t computeStatistics(int quantilesCount);
    uint64_t computeNmergeStatistics(int quantilesCount);
    uint64_t computeOnlyStatistics(int quantilesCount);

    /** finalize adjacency lists after all nodes have been added to the graph */
    void finalizeGraph();

    int getSize(){return size;}
    static bool nodesComparator(edge e1, edge e2)
     {
    return (e1.first < e2.first);
     } 
   
     //--------------- TOOL FOR COMPUTING DIMENSIONS --------------------------------------------------
     
    std::pair< double, double > regresion(std::vector< std::pair< double, double > >& v)
{
//-----------------------------------------------------------------------------
//Given a vector of points, computes the alpha and beta of a regression
//-----------------------------------------------------------------------------
  double Sx = 0, Sy = 0, Sxx = 0, Syy = 0, Sxy = 0;
  for (std::vector<std::pair <double,double> >::iterator it=v.begin(); it != v.end(); it++) {
    double x = it->first;
    double y = it->second;
    Sx += x;
    Sy += y;
    Sxx += x * x;
    Syy += y * y;
    Sxy += x * y;
  }
  
  double alpha = (Sx * Sy - v.size() * Sxy)/( Sx * Sx - v.size() * Sxx);
  double beta = Sy / v.size() - alpha * Sx / v.size();
  return std::pair <double,double>(alpha,beta);
}

    
    
    
    
   //--------------- ITERATOR ON EDGES --------------------------------------------------

	typedef struct {int orig; int dest; double weight;} edgeNewDef; 
    
  class EdgeIter;                          // Iterator to traverse all EDGES of the Graph
  friend class EdgeIter;
	
  class EdgeIter : public std::iterator<std::input_iterator_tag, edgeNewDef> {
		SimpleGraph &g;
		std::vector<edge>::iterator it;
		int node;
		SimpleGraph::edgeNewDef e;

	public:
                
		EdgeIter(SimpleGraph &x) : g(x){}
		EdgeIter(SimpleGraph &x, std::vector<edge>::iterator y, int n): g(x), it(y), node(n){ }

		EdgeIter (const EdgeIter &x) : g(x.g), it(x.it), node(x.node) {}

		EdgeIter& operator++() {
		 
		 
			assert(node >= 0 && node < g.getSize());
			assert(node < g.getSize()-1 || it != g.node[node].end()); 
			do {
				if (++it == g.node[node].end() && node < g.getSize()-1) {
					do {
					 
						node++; 
						it = g.node[node].begin(); 
						
						
					}
					while (it == g.node[node].end() && node < g.getSize()-1); //skip nodes without neighs
				}
			}
			while (!(it == g.node[node].end() && node == g.getSize()-1)   //skip when orig > dest
				&& node > it->first);
			return *this;
		}
		EdgeIter& operator++(int) {
		   
			EdgeIter clone(*this);
			assert(node >= 0 && node < g.getSize());
			assert(node < g.getSize()-1 || it != g.node[node].end()); 
			do {
			 
				if (++it == g.node[node].end() && node < g.getSize()-1) {
				  
					do {
					  
						node++; 
						it = g.node[node].begin(); 
					}
					while (it == g.node[node].end() && node < g.getSize()-1); //skip nodes without neighs
				}
			}
			while (!(it == g.node[node].end() && node == g.getSize()-1)   //skip when orig > dest
				&& node > it->first);
			return *this;
		}

		bool operator==(const EdgeIter &rhs) {return it==rhs.it;}
		bool operator!=(const EdgeIter &rhs) {return it!=rhs.it;}

		SimpleGraph::edgeNewDef& operator*() {
			std::cerr<<"ENTER *\n";
			
			e.orig = node; 
			e.dest = it->first; 
			e.weight = it->second; 
			return e;
		}
		SimpleGraph::edgeNewDef *operator->() {
		         e.orig = node; 
			e.dest = it->first; 
			e.weight = it->second; 
			 
			return &e;
		}

	};

	EdgeIter begin() {
		int nod = 0;
		while (this->node[nod].size()==0 && nod < size) nod++;
		return EdgeIter(*this, this->node[nod].begin(), nod);  
	// First neight of first node
	}

	EdgeIter end() {
		return EdgeIter(*this, this->node[size-1].end(), size);  
	// Last neight of last node
	}
    
    //--------------- ITERATOR ON NEIGHBORS --------------------------------------------------

	class NeighIter : public std::vector<int>::iterator {
	        
		std::vector<edge>::iterator it;
		SimpleGraph::edgeNewDef e;
		
		
	public:
               
	    	NeighIter(std::vector<edge>::iterator x) : it(x){}
		
		SimpleGraph::edgeNewDef *operator->() {
			e.dest = it->first; 
			e.weight = it->second; 
			return &e;
		}
		NeighIter& operator++() {
			++it;
			return *this;
		}
		NeighIter& operator++(int) {
			it++;
			return *this;
		}
		bool operator==(const NeighIter& x) {return it==x.it;}
		bool operator!=(const NeighIter& x) {return it!=x.it;}
	};

	NeighIter begin(int x) { 
	        
		assert(x>=0 && x<= size-1); 
		return NeighIter(node[x].begin()); 
	}

	NeighIter end(int x) {
	  
		assert(x>=0 && x<= size-1); 
		  
		return NeighIter(node[x].end()); 
	}
	
	
};

#endif /* GRAPH_H_ */
