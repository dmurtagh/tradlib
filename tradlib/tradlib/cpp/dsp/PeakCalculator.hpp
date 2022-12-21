//
//  PeakCalculator.hpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#pragma once

#include <stdio.h>
#include <vector>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class PeakCalculator
    {
        
    public:
        
        static SharedIntVec calculatePeaks2(const vector<float> & data, int border, int howFar, float thresholdNormal);
        
        static SharedIntVec calculatePeaks(const vector<float> & data, int border, int howFar, float thresholdNormal);
        
        static float calculateThresholdValue(const vector<float> & data, int howFar, float thresholdNormal);
        
        static SharedIntVec calculateTrough(const vector<float> & data, int border, int howFar, int sj, float thresholdValue);
    };
}
