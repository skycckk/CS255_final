//
//  AvgClusteringCoef.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/10/16.
//
//

#include <stdio.h>
#include "AvgClusteringCoef.h"

AvgClusteringCoef::AvgClusteringCoef()
{
    
}

AvgClusteringCoef::~AvgClusteringCoef()
{
    
}

void AvgClusteringCoef::ProcessACC(Graph *p_g1, Graph *p_g2)
{
    float acc1 = GenerateACC(p_g1);
    float acc2 = GenerateACC(p_g2);
}

float AvgClusteringCoef::GenerateACC(Graph *p_g)
{
    if (p_g->vertices_number < 1)
        return 0.f;
    
    float acc = 0.f;
    for (auto it = p_g->p_graph_type->begin(); it != p_g->p_graph_type->end(); it++)
    {
        Vertex vi = it->second;
        
        unsigned int total_friend_edges = 0;
        for (int j = 0; j < vi.friends.size(); j++)
        {
            Vertex vj = p_g->p_graph_type->at(vi.friends[j].id);
            for (int k = 0; k < vj.friends.size(); k++)
            {
                Vertex vk = vj.friends[k];
                for (int t = 0; t < vi.friends.size(); t++)
                    if (vk.id == vi.friends[t].id) total_friend_edges++;
            }
        }
        
        float lcc = (float)total_friend_edges / vi.friends.size();
        acc += lcc;
    }
    
    acc = acc / p_g->vertices_number;
    return acc;
}

