//
//  UGraph.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#include <stdio.h>
#include "UGraph.h"
#include <fstream>

UGraph::UGraph()
{
    printf("UGraph::UGraph()\n");
}

UGraph::~UGraph()
{
    printf("UGraph::~UGraph()\n");
}

bool UGraph::Initialize(string path)
{
    if (!path.c_str())
        return false;
    
    ifstream infile(path.c_str());
    string str;
    string delimiter = " ";
    int total_vertices = 0;
    while (std::getline(infile, str))
    {
        // Process str
        size_t pos = 0;
        string token;
        vector<string> tokens;
        while ((pos = str.find(delimiter)) != std::string::npos)
        {
            token = str.substr(0, pos);
            str.erase(0, pos + delimiter.length());
            tokens.push_back(token);
        }
        token = str;
        tokens.push_back(token);
        
        if (tokens.size() == 2)
        {
            // paired {vertex_id, vertex_id}
            int v1_id = stoi(tokens[0]);
            int v2_id = stoi(tokens[1]);
            
            Vertex v1_vertex;
            v1_vertex.id = v1_id;
            Vertex v2_vertex;
            v2_vertex.id = v2_id;
            if (m_vertices.find(v1_id) == m_vertices.end())
            {
                v1_vertex.friends.push_back(v2_vertex);
                m_vertices.insert({v1_id, v1_vertex});
                m_graph.vertices_number++;
                total_vertices++;
            }
            else
            {
                m_vertices.at(v1_id).friends.push_back(v2_vertex);
            }
            
            if (m_vertices.find(v2_id) == m_vertices.end())
            {
                v2_vertex.friends.push_back(v1_vertex);
                m_vertices.insert({v2_id, v2_vertex});
                m_graph.vertices_number++;
                total_vertices++;
            }
            else
            {
                m_vertices.at(v2_id).friends.push_back(v1_vertex);
            }
        }
    }
    infile.close();
    
    m_graph.vertices_number = total_vertices;
    m_graph.p_graph_type = &m_vertices;
    return true;
}

void UGraph::RemoveEdge(Vertex w, Vertex v)
{
    Vertex *pw_internal = &m_graph.p_graph_type->at(w.id);
    for (int i = 0; i < pw_internal->friends.size(); i++)
    {
        if (pw_internal->friends[i].id == v.id)
        {
            pw_internal->friends.erase(pw_internal->friends.begin() + i);
            break;
        }
    }
}

Graph UGraph::GetVertices()
{
    return m_graph;
}
