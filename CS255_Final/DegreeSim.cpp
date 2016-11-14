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
    Uninitialize();
}

void DegreeSim::Uninitialize()
{
    for (int i = 0; i < 2; i++)
    {
        m_degree_hist[i].clear();
        m_degree_cdf[i].clear();
        m_degree_pdf[i].clear();
    }
}

float DegreeSim::ProcessDegSim(Graph *p_g1, Graph *p_g2)
{
    GeneratePDF(p_g1, p_g2);
    
    float max_diff = 0.f;
    if (m_degree_cdf[0].size() != 0 &&
        m_degree_cdf[1].size() != 0)
    {
        
        std::vector<float> *p_large_degree_cdf = &(m_degree_cdf[1]);
        std::vector<float> *p_small_degree_cdf = &(m_degree_cdf[0]);
        if (m_degree_cdf[0].size() > m_degree_cdf[1].size())
        {
            p_large_degree_cdf = &(m_degree_cdf[0]);
            p_small_degree_cdf = &(m_degree_cdf[1]);
        }
        
        
        for (int i = 0; i < p_small_degree_cdf->size(); i++)
        {
            float diff = fabs((*p_small_degree_cdf)[i] - (*p_large_degree_cdf)[i]);
            if (diff >= max_diff)
                max_diff = diff;
        }
    }
    return 1.f - max_diff;
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

