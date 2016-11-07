//
//  DegreeSim.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/6/16.
//
//

#include <stdio.h>
#include "DegreeSim.h"

DegreeSim::DegreeSim()
{

}

DegreeSim::~DegreeSim()
{

}

void DegreeSim::GeneratePDF(Graph *p_g, int index)
{
    int min_degree = INT_MAX;
    int max_degree = INT_MIN;
    for (auto it = p_g->p_graph_type->begin(); it != p_g->p_graph_type->end(); it++)
    {
        int degree = (int)(it->second.friends.size());
        if (degree < min_degree)
            min_degree = degree;
        
        if (degree > max_degree)
            max_degree = degree;
    }
    
    for (int i = 0; i < max_degree - min_degree + 1; i++)
        m_degree_hist[index].push_back(0);
    
    for (auto it = p_g->p_graph_type->begin(); it != p_g->p_graph_type->end(); it++)
    {
        int degree = (int)(it->second.friends.size());
        m_degree_hist[index][degree - 1]++;
    }
    
    int number_of_vertices = p_g->vertices_number;
    
    for (int i = 0; i < m_degree_hist[index].size(); i++)
        m_degree_pdf[index].push_back(m_degree_hist[index][i] / (float)number_of_vertices);
}

void DegreeSim::GeneratePDF(Graph *p_g1, Graph *p_g2)
{
    GeneratePDF(p_g1, 0);
    GeneratePDF(p_g2, 1);
    
    GenerateCDF(0);
    GenerateCDF(1);
}

void DegreeSim::GenerateCDF(int index)
{
    m_degree_cdf[index].push_back(m_degree_pdf[index][0]);
    for (int i = 1; i < m_degree_hist[index].size(); i++)
        m_degree_cdf[index].push_back(m_degree_cdf[index][i - 1] + m_degree_pdf[index][i]);
}

