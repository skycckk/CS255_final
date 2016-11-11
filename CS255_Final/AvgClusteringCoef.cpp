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

float AvgClusteringCoef::ProcessNMSE(Graph *p_g1, Graph *p_g2)
{
    float acc1 = GenerateACC(p_g1);
    float acc2 = GenerateACC(p_g2);
    
    float mse = ComputeMSE(acc1, acc2);
    float ground_mse = ComputeMSE(acc1, 0);
    float nmse = mse / ground_mse;
    return nmse;
}

float AvgClusteringCoef::GenerateACC(Graph *p_g)
{
    if (p_g->vertices_number < 1)
        return 0.f;
    
    float acc = 0.f;
    int vertices_count = 0;
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
        vertices_count++;
    }
    
    if (vertices_count != p_g->vertices_number)
    {
        printf("ERROR: Wrong Vertices\n");
        return 0.f;
    }
    
    acc = acc / vertices_count;
    return acc;
}

float AvgClusteringCoef::ComputeMSE(float x, float y)
{
    return (x - y) * (x - y);
}

