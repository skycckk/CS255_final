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
            
            Vertex v_friend;
            v_friend.id = v2_id;
            if (m_vertices.find(v1_id) == m_vertices.end())
            {
                Vertex vertex;
                vertex.id = v1_id;
                vertex.friends.push_back(v_friend);
                
                m_vertices.insert({v1_id, vertex});
            }
            else
            {
                m_vertices.at(v1_id).friends.push_back(v_friend);
            }
        }
    }
    infile.close();
    
    return true;
}

unordered_map<int, Vertex> UGraph::GetVertices()
{
    return m_vertices;
}
