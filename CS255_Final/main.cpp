//
//  main.cpp
//  CS255_Final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#include <iostream>
#include "UGraph.h"
#include "RankDegreeAlgo.h"
#include "RankDegreeAlgoRefined.h"
#include "DegreeSim.h"
#include "AvgClusteringCoef.h"

#define DUMP_DETAIL

using namespace std;

void PrintGraph(Graph graph, int original_vertex_size)
{
    for (int i = 0; i < original_vertex_size; i++)
    {
        if (graph.p_graph_type->find(i) != graph.p_graph_type->end())
        {
            printf("%d -> ", graph.p_graph_type->at(i).id);
            for (int j = 0; j < graph.p_graph_type->at(i).friends.size(); j++)
            {
                int f_id = graph.p_graph_type->at(i).friends[j].id;
                printf("%d ", f_id);
            }
            printf("\n");
        }
    }
}

int main(int argc, const char * argv[])
{
    UGraph fb_graph;
    fb_graph.Initialize("../../dataset/facebook_combined.txt");
    const int vertex_size = 4039;
//    fb_graph.Initialize("../../dataset/small_test.txt");
//    const int vertex_size = 16;
    
    const int total_number_random_test = 1;
    const int total_number_sample_size = 5;
    const float sample_size_ratio_list[total_number_sample_size] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
    float deg_sim_avg[total_number_sample_size] = {0.f};
    float deg_sim2_avg[total_number_sample_size] = {0.f};
    float nmse_acc_avg[total_number_sample_size] = {0.f};
    float nmse_acc2_avg[total_number_sample_size] = {0.f};
    
    Graph orig_graph;
    GraphType orig_vertices;
    orig_graph.p_graph_type = &orig_vertices;
    fb_graph.GetVerticesCopy(orig_graph);
    
    for (int si = 0; si < total_number_sample_size; si++)
    {
        float sample_ratio = sample_size_ratio_list[si];
        printf("SampleSize: %d%%...\n", (int)(sample_ratio * 100.f));
        float deg_sim_results[total_number_random_test] = {0.f};
        float deg_sim_results2[total_number_random_test] = {0.f};
        float nmse_acc_results[total_number_random_test] = {0.f};
        float nmse_acc_results2[total_number_random_test] = {0.f};
        for (int i = 0; i < total_number_random_test; i++)
        {
            UGraph fb_graph_test;
            fb_graph_test = fb_graph;
            RankDegreeAlgo RD = RankDegreeAlgo();
            Graph rd_graph = RD.Process(fb_graph_test, max((int)roundf(vertex_size * 0.01f), 1),
                                        0.3, vertex_size * sample_ratio);
            
            fb_graph_test = fb_graph;
            RankDegreeAlgoRefined RD2 = RankDegreeAlgoRefined();
            Graph rd2_graph = RD2.Process(fb_graph_test, max((int)roundf(vertex_size * 0.01f), 1),
                                          0.3, vertex_size * sample_ratio);
            
            
            DegreeSim degree_sim;
            float deg_sim = degree_sim.ProcessDegSim(&orig_graph, &rd_graph);
            deg_sim_results[i] = deg_sim;
            
            degree_sim.Uninitialize();
            float deg_sim2 = degree_sim.ProcessDegSim(&orig_graph, &rd2_graph);
            deg_sim_results2[i] = deg_sim2;
            
            AvgClusteringCoef acc;
            nmse_acc_results[i] = acc.ProcessNMSE(&orig_graph, &rd_graph);
            nmse_acc_results2[i] = acc.ProcessNMSE(&orig_graph, &rd2_graph);
        }
        
        for (int i = 0; i < total_number_random_test; i++)
        {
#ifdef DUMP_DETAIL
            printf("DegSim: %f\n", deg_sim_results[i]);
            printf("NMSE: %f\n", nmse_acc_results[i]);
#endif
            deg_sim_avg[si] += deg_sim_results[i];
            nmse_acc_avg[si] += nmse_acc_results[i];
        }
        for (int i = 0; i < total_number_random_test; i++)
        {
#ifdef DUMP_DETAIL
            printf("DegSim2: %f\n", deg_sim_results2[i]);
            printf("NMSE: %f\n", nmse_acc_results2[i]);
#endif
            deg_sim2_avg[si] += deg_sim_results2[i];
            nmse_acc2_avg[si] += nmse_acc_results2[i];
        }
        deg_sim_avg[si] /= total_number_random_test;
        deg_sim2_avg[si] /= total_number_random_test;
    }
    for (int si = 0; si < total_number_sample_size; si++)
    {
        float sample_ratio = sample_size_ratio_list[si];
        printf("SampleSize: %d%% -> (DegSimAvg, NMSE_ACC): (%f, %f) and (%f, %f)\n",
               (int)(sample_ratio * 100.f), deg_sim_avg[si], nmse_acc_avg[si], deg_sim2_avg[si], nmse_acc2_avg[si]);
    }
    
//    PrintGraph(rd_graph, vertex_size);
//    PrintGraph(rd2_graph, vertex_size);
    
    return 0;
}
