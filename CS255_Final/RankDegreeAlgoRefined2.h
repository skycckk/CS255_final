//
//  RankDegreeAlgoRefined2.h
//  CS255_final
//
//  Created by Wayne Huang on 11/18/16.
//
//

#ifndef RankDegreeAlgoRefined2_h
#define RankDegreeAlgoRefined2_h

#include "UGraph.h"

class RankDegreeAlgoRefined2
{
public:
    RankDegreeAlgoRefined2();
    ~RankDegreeAlgoRefined2();
    
    Graph Process(UGraph orig_graph, int s, float p, int x);
    
private:
    Graph m_sub_graph;
    GraphType m_sub_vertices;
    
    void RandomShuffle(std::vector<int> &v);
    void RandomShuffle(std::vector<std::pair<int, int>> &v);
};


#endif /* RankDegreeAlgoRefined2_h */
