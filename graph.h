// graph.h <Starter Code>
// < Your name >
//
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Spring 2023
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
 private:
  map<VertexT, map<VertexT, WeightT> > AdjList;
  int numVertices;
  int numEdges;

 public:
  //
  // constructor:
  //
  // Constructs an empty graph where n is the max # of vertices
  // you expect the graph to contain.
  //
  // NOTE: the graph is implemented using an adjacency matrix.
  // If n exceeds the dimensions of this matrix, an exception
  // will be thrown to let you know that this implementation
  // will not suffice.
  //
  graph() {
    numVertices = 0;
    numEdges = 0;
  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const {
    return numVertices;
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    return numEdges;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  
  bool addVertex(VertexT v) {
    if (AdjList.count(v))
      return false;

    AdjList[v];
    numVertices++;
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    if (AdjList.count(from) == 0 || AdjList.count(to) == 0)
      return false;

    if (AdjList[from].count(to) == 0)
      numEdges++;

    AdjList[from][to] = weight;
    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    if (AdjList.count(from) == 0 || AdjList.count(to) == 0) //Vertices don't exist
      return false;

    if (AdjList.at(from).count(to) == 0) //Edge does not exist
      return false;

    weight = AdjList.at(from).at(to);
    return true;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT>  S;
    if (AdjList.count(v) == 0)
      return S;
    for (auto adj : AdjList.at(v)){
      S.insert(adj.first);
    }
    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    for (auto vertex : AdjList){
      vertices.push_back(vertex.first);
    }
    return vertices;
  }

  //
  // dump
  //
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    // output << "**Vertices:" << endl;
    // for (int i = 0; i < AdjList.size(); i++){
    //   output << " " << i << ". " << AdjList.find() << endl; //FIXME
    // }

    output << endl;
    output << "**Edges:" << endl;
    for (auto vertex : AdjList){
      output << vertex.first << ": ";
      for (auto adj : vertex.second){
        output << "(" << vertex.first << "," << adj.first << "," << adj.second << ") ";
      }
      output << endl;
    }
    output << "**************************************************" << endl;
  }
};
