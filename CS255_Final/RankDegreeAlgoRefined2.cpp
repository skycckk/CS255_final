//
//  RankDegreeAlgoRefined2.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/18/16.
//
//

#include <stdio.h>
#include "RankDegreeAlgoRefined2.h"
#include "RankDegreeAlgo.h"
#include "DegreeSim.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>

#define USE_RANDOM_ALL

static struct VertexCompGreater
{
    // <id, friend.size>
    inline bool operator() (const std::pair<int, int> &struct1, const std::pair<int, int> &struct2)
    {
        return (struct1.second > struct2.second);
    }
    
} VertexCompGreater;

RankDegreeAlgoRefined2::RankDegreeAlgoRefined2()
{
    
}

RankDegreeAlgoRefined2::~RankDegreeAlgoRefined2()
{
    
}

Graph RankDegreeAlgoRefined2::Process(UGraph orig_graph, int s, float p, int x)
{
    cout << "Processing..." << endl;
    
    Graph G = orig_graph.GetVertices();
    srand (1); // TEST BY FIXING 1 AS SEED
    std::vector<Vertex> seeds;
    
    // Initialize seeds
    std::vector<int> v_vertices;
    for (auto it = G.p_graph_type->begin(); it != G.p_graph_type->end(); it++)
    {
        std::vector<std::pair<int, int>> friends_vertices_tuple;
        v_vertices.push_back(it->first);
    }
    
    RandomShuffle(v_vertices);
    for (int i = 0; i < s; i++)
    {
#ifdef USE_RANDOM_ALL
        int id = v_vertices[i];
#else
        int id = rand() % number_vertices;
#endif
        seeds.push_back(G.p_graph_type->at(id));
    }
    
    Graph gg;
    unordered_map<int, Vertex> gg_type;
    gg.p_graph_type = &gg_type;
    
    int sample_size = 0;
    while (sample_size < x)
    {
        std::vector<Vertex> new_seeds;
        std::vector<int> pending_friends;
        for (int i = 0; i < seeds.size(); i++)
        {
            Vertex *p_w = &(seeds[i]);
            
            if (p_w->friends.size() == 0)
                continue;
            
            Vertex w;
            w.id = p_w->id;
            if (m_sub_vertices.find(w.id) == m_sub_vertices.end())
            {
                m_sub_vertices.insert({w.id, w});
                m_sub_graph.vertices_number++;
                pending_friends.push_back(w.id);
                sample_size++;
            }
            
            // Find w's friends degree in top-k
            int number_friends = (int)(p_w->friends.size());
            int top_k = max((int)roundf(number_friends * p), 1);
            std::vector<std::pair<int, int>> friends_vertices_tuple;
            for (int z = 0; z < p_w->friends.size(); z++)
            {
                int f_id = p_w->friends[z].id;
                int f_size = (int)(G.p_graph_type->at(f_id)).friends.size();
                std::pair<int, int> test_pair(f_id, f_size);
                friends_vertices_tuple.push_back(test_pair);
            }
            std::sort(friends_vertices_tuple.begin(), friends_vertices_tuple.end(), VertexCompGreater);
            std::default_random_engine generator;
            std::uniform_int_distribution<int> rand_dist(0, (int)p_w->friends.size() - 1);
            for (int j = 0; j < top_k; j++)
            {
                if (j > 0)
                {
                    int toll = rand_dist(generator);
                    if (toll == 0)
                        continue;
                }
                
                Vertex friend_v;
                friend_v.id = G.p_graph_type->at(friends_vertices_tuple[j].first).id;
                
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
#ifdef USE_RANDOM_ALL
            RandomShuffle(pending_friends);
#endif
            for (int i = 0; i < to_be_removed_sample_size; i++)
            {
                int remove_id = pending_friends[i];
                if (m_sub_vertices.at(remove_id).friends.size() != 0)
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
                }
                m_sub_vertices.erase(remove_id);
                m_sub_graph.vertices_number--;
            }
        }
        
        // Gather distribution of new seeds.
        // Compare current distribution and previous distribution:
        // If they are close, tending to randomize since we assume the probability of getting same distribution is
        // lower than getting different one. That is, our program will consider there exist some bias.
        // Otherwise, we keep the current seed friends.
        Graph g1;
        unordered_map<int, Vertex> g1_type;
        for (int i = 0; i < new_seeds.size(); i++)
        {
            Vertex v = new_seeds[i];
            g1_type.insert({v.id, v});
        }
        g1.p_graph_type = &g1_type;
        g1.vertices_number = (int)new_seeds.size();
        DegreeSim degree_sim;
        float deg_sim = degree_sim.ProcessDegSim(&g1, &gg);
        gg_type = g1_type;
        gg.vertices_number = g1.vertices_number;
        
        // Automatically adjust top-k ratio by assumption of comparison on degree distribution
        p = (1.f - deg_sim * 0.8f);
        
        // Deciding for selecting random and existing seeds
        // decay factor: 0.3
        int number_of_random_seeds = round((float)new_seeds.size() * deg_sim * 0.3f);
        std::vector<int> to_be_random_index;
        for (int i = 0; i < new_seeds.size(); i++)
            to_be_random_index.push_back(i);
        RandomShuffle(to_be_random_index);
        RandomShuffle(v_vertices);
        for (int i = 0, j = 0; i < number_of_random_seeds && j < v_vertices.size(); i++, j++)
        {
            int count = 0;
            while (count < v_vertices.size())
            {
                int id = v_vertices[j];
                if (G.p_graph_type->at(id).friends.size() != 0)
                {
                    new_seeds[to_be_random_index[i]] = G.p_graph_type->at(id);
                    break;
                }
                count++;
                j++;
            }
        }
        
        // Check if all nodes in {seeds} are leaves (degree = 1), then Repeat Initialization step
        bool need_random_jump = true;
        for (int i = 0; i < new_seeds.size(); i++)
        {
            if (new_seeds[i].friends.size() > 1)
                need_random_jump = false;
        }
        
        seeds.clear();
        if (need_random_jump)
        {
            RandomShuffle(v_vertices);
            int time_out_count = (int)v_vertices.size();
            int count = 0;
            for (int i = 0; i < s; i++)
            {
                int id = v_vertices[i];
                if (G.p_graph_type->at(id).friends.size() == 0)
                {
                    // Original seeds are selected
                    RandomShuffle(v_vertices);
                    i--;
                    continue;
                }
                seeds.push_back(G.p_graph_type->at(id));
                
                if (count >= time_out_count)
                    break;
                
                count++;
            }
        }
        else
        {
            seeds = new_seeds;
        }
    }
    
    m_sub_graph.p_graph_type = &m_sub_vertices;
    return m_sub_graph;
}

void RankDegreeAlgoRefined2::RandomShuffle(std::vector<int> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle (v.begin(), v.end(), g);
}

void RankDegreeAlgoRefined2::RandomShuffle(std::vector<std::pair<int, int>> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle (v.begin(), v.end(), g);
}
