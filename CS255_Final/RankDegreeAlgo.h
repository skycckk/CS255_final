//
//  RankDegreeAlgo.h
//  CS255_final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#ifndef RankDegreeAlgo_h
#define RankDegreeAlgo_h

#include "UGraph.h"

class RankDegreeAlgo
{
public:
    RankDegreeAlgo();
    ~RankDegreeAlgo();
    
    Graph Process(UGraph orig_graph, int s, float p, int x);
    
private:
    Graph m_sub_graph;
    GraphType m_sub_vertices;
};

#endif /* RankDegreeAlgo_h */
