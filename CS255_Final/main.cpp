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
    
    RankDegreeAlgo DD = RankDegreeAlgo();
    Graph rd_graph = DD.Process(fb_graph, 10, 0.5, 4039 * 0.5f);

    DegreeSim degree_sim;
    float deg_sim = degree_sim.ProcessDegSim(&orig_graph, &rd_graph);
    printf("DegSim: %f\n", deg_sim);
    
    AvgClusteringCoef acc;
    float nmse = acc.ProcessNMSE(&orig_graph, &rd_graph);
    printf("NMSE: %f\n", nmse);
    
    return 0;
}
