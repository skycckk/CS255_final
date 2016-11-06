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

using namespace std;

int main(int argc, const char * argv[])
{
    // insert code here...
    cout << "Hello, World!\n";
    
    if (argc > 1)
        cout << argv[1] << endl;
    
    UGraph fb_graph;
    fb_graph.Initialize("../../dataset/facebook_combined.txt");
    
    RankDegreeAlgo DD = RankDegreeAlgo();
    DD.Process(fb_graph, 10, 0.5, 88234 / 2);
    
    return 0;
}
