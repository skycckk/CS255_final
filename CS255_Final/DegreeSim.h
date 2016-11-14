//
//  DegreeSim.h
//  CS255_final
//
//  Created by Wayne Huang on 11/6/16.
//
//

#ifndef DegreeSim_h
#define DegreeSim_h

#include "UGraph.h"

class DegreeSim
{
public:
    DegreeSim();
    ~DegreeSim();
    
    float ProcessDegSim(Graph *p_g1, Graph *p_g2);
    void Uninitialize();
    
private:
    void GeneratePDF(Graph *p_g1, Graph *p_g2);
    void GeneratePDF(Graph *p_g, int index);
    void GenerateCDF(int index);
    
    std::vector<int> m_degree_hist[2];
    std::vector<float> m_degree_pdf[2];
    std::vector<float> m_degree_cdf[2];
};


#endif /* DegreeSim_h */
