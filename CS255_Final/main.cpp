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

using namespace std;

int main(int argc, const char * argv[])
{
    UGraph fb_graph;
    fb_graph.Initialize("../../dataset/facebook_combined.txt");
    
    Graph orig_graph;
    GraphType orig_vertices;
    orig_graph.p_graph_type = &orig_vertices;
    fb_graph.GetVerticesCopy(orig_graph);
    
    UGraph fb_graph_test;
    fb_graph_test = fb_graph;
    
    RankDegreeAlgo RD = RankDegreeAlgo();
    Graph rd_graph = RD.Process(fb_graph, 10, 0.5, 4039 * 0.5f);

    RankDegreeAlgoRefined RD2 = RankDegreeAlgoRefined();
    Graph rd2_graph = RD2.Process(fb_graph_test, 10, 0.5, 4039 * 0.5f);

    DegreeSim degree_sim;
    float deg_sim = degree_sim.ProcessDegSim(&orig_graph, &rd_graph);
    printf("DegSim: %f\n", deg_sim);
    
    degree_sim.Uninitialize();
    float deg_sim2 = degree_sim.ProcessDegSim(&orig_graph, &rd2_graph);
    printf("DegSim2: %f\n", deg_sim2);
    
    AvgClusteringCoef acc;
    float nmse1 = acc.ProcessNMSE(&orig_graph, &rd_graph);
    printf("NMSE1: %f\n", nmse1);
    
    float nmse2 = acc.ProcessNMSE(&orig_graph, &rd2_graph);
    printf("NMSE2: %f\n", nmse2);
    
    return 0;
}
