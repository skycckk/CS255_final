//
//  UGraph.h
//  CS255_final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#ifndef UGraph_h
#define UGraph_h

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Vertex
{
    int id;
    vector<Vertex> friends;
};
typedef unordered_map<int, Vertex> GraphType;

struct Graph
{
    unsigned int vertices_number;
    GraphType *p_graph_type;
    
    Graph()
    {
        vertices_number = 0;
        p_graph_type = NULL;
    }
};

class UGraph
{
public:
    UGraph();
    ~UGraph();
    
    bool Initialize(string path);
    Graph GetVertices();
    void GetVerticesCopy(Graph &graph);
    void RemoveEdge(Vertex w, Vertex v);
    
private:
    
    Graph m_graph;
    GraphType m_vertices;
};


#endif /* UGraph_h */
