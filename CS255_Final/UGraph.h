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

class UGraph
{
public:
    UGraph();
    ~UGraph();
    
    bool Initialize(string path);
    
private:
    
    unordered_map<int, Vertex> m_vertices;
    
};


#endif /* UGraph_h */
