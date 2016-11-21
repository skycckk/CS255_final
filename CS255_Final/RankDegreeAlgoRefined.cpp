//
//  RankDegreeAlgoRefined.cpp
//  CS255_final
//
//  Created by Wayne Huang on 11/13/16.
//
//

#include <stdio.h>
#include "RankDegreeAlgoRefined.h"
#include "RankDegreeAlgo.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>

#define USE_RANDOM_ALL
#undef USE_K_MEANS

static struct VertexCompGreater
{
    // <id, friend.size>
    inline bool operator() (const std::pair<int, int> &struct1, const std::pair<int, int> &struct2)
    {
        return (struct1.second > struct2.second);
    }
    
} VertexCompGreater;

RankDegreeAlgoRefined::RankDegreeAlgoRefined()
{
    
}

RankDegreeAlgoRefined::~RankDegreeAlgoRefined()
{
    
}

Graph RankDegreeAlgoRefined::Process(UGraph orig_graph, int s, float p, int x)
{
    cout << "Processing..." << endl;
    
    Graph G = orig_graph.GetVertices();
    int number_vertices = G.vertices_number;
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
            std::vector<int> friends_degree;
            for (int z = 0; z < p_w->friends.size(); z++)
            {
                int f_id = p_w->friends[z].id;
                int f_size = (int)(G.p_graph_type->at(f_id)).friends.size();
                std::pair<int, int> test_pair(f_id, f_size);
                friends_vertices_tuple.push_back(test_pair);
                friends_degree.push_back(f_size);
            }
            
#ifdef USE_K_MEANS
            float all_means[4] = {0.f};
            std::vector<int> k_labels = Kmeans1D(friends_degree, 4, all_means);
            int largest_k = 0;
            int smallest_k = 0;
            float max_mean = __FLT_MIN__;
            float min_mean = __FLT_MAX__;
            bool is_valid_kmeans = true;
            for (int j = 0; j < 4; j++)
            {
                if (all_means[j] > max_mean)
                {
                    max_mean = all_means[j];
                    largest_k = j;
                }
                if (all_means[j] < min_mean)
                {
                    min_mean = all_means[j];
                    smallest_k = j;
                }
                
                if ((all_means[j] - 0.f) < __FLT_EPSILON__)
                    is_valid_kmeans = false;
            }
#else
            std::sort(friends_vertices_tuple.begin(), friends_vertices_tuple.end(), VertexCompGreater);
            std::default_random_engine generator;
            std::uniform_int_distribution<int> rand_dist(0, (int)p_w->friends.size() - 1);
#endif

#ifdef USE_K_MEANS
            for (int j = 0; j < p_w->friends.size(); j++)
            {
                if (is_valid_kmeans)
                {
                    if (k_labels[j] == largest_k || k_labels[j] == smallest_k)
                        continue;
                }
#else
            for (int j = 0; j < top_k; j++)
            {
                if (j > 0)
                {
                    int toll = rand_dist(generator);
                    if (toll == 0)
                        continue;
                }
#endif
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
        
        // check if all nodes in {seeds} are leaves (degree = 1), then Repeat Initialization step
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
            for (int i = 0; i < s; i++)
            {
                int id = v_vertices[i];
                seeds.push_back(G.p_graph_type->at(id));
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

void RankDegreeAlgoRefined::RandomShuffle(std::vector<int> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle (v.begin(), v.end(), g);
}

void RankDegreeAlgoRefined::RandomShuffle(std::vector<std::pair<int, int>> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle (v.begin(), v.end(), g);
}

std::vector<int> RankDegreeAlgoRefined::Kmeans1D(std::vector<int> samples, int k, float *p_means)
{
    // samples: <friend size>
    // p_means: output means
    std::vector<int> labels(samples.size(), 0);
    
    if (samples.size() < k)
        return labels;
    
    std::vector<float> means;
    std::vector<float> prev_means;
    float *p_sums = new float[k];
    int *p_count = new int[k];
    float prev_mean = -1.f;
    int ik = 0;
    for (int i = 0; i < samples.size(); i++)
    {
        float mean = samples[i];
        
        if (mean != prev_mean)
        {
            means.push_back(mean);
            prev_means.push_back(mean);
            prev_mean = mean;
            labels[ik] = ik;
            p_sums[ik] = 0;
            p_count[ik] = 0;
            ik++;
        }
        
        if (ik == k)
            break;
    }

    int time_out = 0;
    bool is_ok = false;
    while (!is_ok)
    {
        for (int i = 0; i < samples.size(); i++)
        {
            int current_sample = samples[i];
            int min_dist = INT_MAX;
            int new_k = 0;
            for (int j = 0; j < k; j++)
            {
                int dist = fabs(current_sample - means[j]);
                if (dist < min_dist)
                {
                    min_dist = dist;
                    new_k = j;
                }
            }
            
            labels[i] = new_k;
            p_sums[new_k] += current_sample;
            p_count[new_k]++;
        }
        
        for (int i = 0; i < k; i++)
        {
            means[i] = p_sums[i] / p_count[i];
            p_means[i] = means[i];
            
            p_sums[i] = 0;
            p_count[i] = 0;
        }
        
        float se = 0.f;
        for (int i = 0; i < k; i++)
        {
            se += (means[i] - prev_means[i]) * (means[i] - prev_means[i]);
            prev_means[i] = means[i];
        }
        se = sqrt(se / k);
        
        time_out++;
        if (time_out == 20 || se <= 1)
            is_ok = true;
    }
    
    return labels;
}
