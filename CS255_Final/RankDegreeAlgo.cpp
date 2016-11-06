//
//  RankDegreeAlgo.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#include "RankDegreeAlgo.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>

struct VertexCompGreater
{
    inline bool operator() (const Vertex& struct1, const Vertex& struct2)
    {
        return (struct1.id > struct2.id);
    }
} VertexCompGreater;

RankDegreeAlgo::RankDegreeAlgo()
{
    
}

RankDegreeAlgo::~RankDegreeAlgo()
{
    
}

Graph RankDegreeAlgo::Process(UGraph orig_graph, int s, float p, int x)
{
    cout << "Processing..." << endl;
    
    Graph G = orig_graph.GetVertices();
    int number_vertices = G.vertices_number;
    srand (1); // TEST BY FIXING 1 AS SEED
    std::vector<Vertex> seeds;
    
    // Initialize seeds
    std::vector<int> v_vertices;
    for (int i = 0; i < number_vertices; i++)
        v_vertices.push_back(i);
    
    RandomShuffle(v_vertices);
    for (int i = 0; i < s; i++)
    {
//        int id = rand() % number_vertices;
        int id = v_vertices[i];
        seeds.push_back(G.p_graph_type->at(id));
    }
    
    int sample_size = 0;
    while (sample_size < x)
    {
        std::vector<Vertex> new_seeds;
        std::vector<int> pending_friends;
        cout << "--------seed size " << seeds.size() << endl;
        for (int i = 0; i < seeds.size(); i++)
        {
            Vertex *p_w = &(seeds[i]);
            
            Vertex w;
            w.id = p_w->id;
            if (m_sub_vertices.find(w.id) == m_sub_vertices.end())
            {
                m_sub_vertices.insert({w.id, w});
                m_sub_graph.vertices_number++;
                sample_size++;
            }
            
            // Find w's friends degree in top-k
            int number_friends = (int)(p_w->friends.size());
            int top_k = roundf(number_friends * p);
            std::sort(p_w->friends.begin(), p_w->friends.end(), VertexCompGreater);
            for (int j = 0; j < top_k; j++)
            {
                Vertex friend_v = p_w->friends[j];
                
                // check dupcliated
                bool has_duplicated = false;
                for (int k = 0; k < m_sub_vertices.at(w.id).friends.size(); k++)
                {
                    if (m_sub_vertices.at(w.id).friends[k].id == friend_v.id)
                    {
                        has_duplicated = true;
                        break;
                    }
                }
                
                if (!has_duplicated)
                {
                    m_sub_vertices.at(w.id).friends.push_back(friend_v); // {w, v}
                    orig_graph.RemoveEdge(w, friend_v);
                    
                    if (m_sub_vertices.find(friend_v.id) == m_sub_vertices.end())
                    {
                        m_sub_vertices.insert({friend_v.id, friend_v});
                        m_sub_graph.vertices_number++;
                        pending_friends.push_back(friend_v.id);
                        
                        sample_size++;
                        
                        new_seeds.push_back(G.p_graph_type->at(friend_v.id));
                    }
                    
                    m_sub_vertices.at(friend_v.id).friends.push_back(w);
                    orig_graph.RemoveEdge(friend_v, w);
                }
            }
        }
        
        // handle situation that the sample of final round is exceeded to desired sample size.
        // by using random pick-up for "pending friends" to remove from sub-graph
        if (sample_size > x)
        {
            int to_be_removed_sample_size = sample_size - x;
            cout << "TBRemove: " << to_be_removed_sample_size << " PenF: "  << pending_friends.size() << endl;
            RandomShuffle(pending_friends);
            for (int i = 0; i < to_be_removed_sample_size; i++)
            {
                int remove_id = pending_friends[i];
                if (m_sub_vertices.at(remove_id).friends.size() == 0)
                {
                    // error handling
                    cout << "WRONG......." << endl;
                }
                else
                {
                    // remove "removed vertex" adjacent edges
                    std::vector<Vertex> *p_remove_friends = &(m_sub_vertices.at(remove_id).friends);
                    for (int j = 0; j < p_remove_friends->size(); j++)
                    {
                        int remove_friend_id = (*p_remove_friends)[j].id;
                        std::vector<Vertex> *p_remove_ffs = &(m_sub_vertices.at(remove_friend_id).friends);
                        for (int k = 0; k < p_remove_ffs->size(); k++)
                        {
                            if ((*p_remove_ffs)[k].id == remove_id)
                            {
                                p_remove_ffs->erase(p_remove_ffs->begin() + k);
                                break;
                            }
                        }
                    }
                    
                    m_sub_vertices.erase(remove_id);
                    m_sub_graph.vertices_number--;
                }
            }
        }
        
        // check if all nodes in {seeds} are leaves (degree = 1), then Repeat Initialization step
        bool need_random_jump = true;
        for (int i = 0; i < new_seeds.size(); i++)
        {
            if (new_seeds[i].friends.size() > 1)
                need_random_jump = false;
        }
        
        seeds.clear();
        cout << "new seed size: " << new_seeds.size() << endl;
        if (need_random_jump)
        {
            for (int i = 0; i < s; i++)
            {
                int id = rand() % number_vertices;
                seeds.push_back(G.p_graph_type->at(id));
            }
        }
        else
        {
            seeds = new_seeds;
        }
    }
    
    cout << "NewGraphSize: " << m_sub_vertices.size() << " and " << m_sub_graph.vertices_number << endl;
    m_sub_graph.p_graph_type = &m_sub_vertices;
    return m_sub_graph;
}

void RankDegreeAlgo::RandomShuffle(std::vector<int> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle (v.begin(), v.end(), g);
}
