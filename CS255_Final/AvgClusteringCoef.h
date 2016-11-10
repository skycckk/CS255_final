//
//  AvgClusteringCoef.h
//  CS255_final
//
//  Created by Wayne Huang on 11/10/16.
//
//

#ifndef AvgClusteringCoef_h
#define AvgClusteringCoef_h

#include "UGraph.h"

class AvgClusteringCoef
{
public:
    AvgClusteringCoef();
    ~AvgClusteringCoef();
    
    void ProcessACC(Graph *p_g1, Graph *p_g2);
    
private:
    float GenerateACC(Graph *p_g);
};


#endif /* AvgClusteringCoef_h */
