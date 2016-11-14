//
//  RankDegreeAlgoRefined.h
//  CS255_final
//
//  Created by Wayne Huang on 11/13/16.
//
//

#ifndef RankDegreeAlgoRefined_h
#define RankDegreeAlgoRefined_h

#include "UGraph.h"

class RankDegreeAlgoRefined
{
public:
    RankDegreeAlgoRefined();
    ~RankDegreeAlgoRefined();
    
    Graph Process(UGraph orig_graph, int s, float p, int x);
    
private:
    Graph m_sub_graph;
    GraphType m_sub_vertices;
    
    void RandomShuffle(std::vector<int> &v);
    void RandomShuffle(std::vector<std::pair<int, int>> &v);
};


#endif /* RankDegreeAlgoRefined_h */
