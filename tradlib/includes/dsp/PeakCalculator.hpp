//
//  PeakCalculator.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
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
        
        static SharedIntVec calculatePeaks2(const SharedFloatVec & data, int border, int howFar, float thresholdNormal);
        
        static SharedIntVec calculatePeaks(const SharedFloatVec & data, int border, int howFar, float thresholdNormal);
        
        static float calculateThresholdValue(const SharedFloatVec & data, int howFar, float thresholdNormal);
        
        static SharedIntVec calculateTrough(const SharedFloatVec & data, int border, int howFar, int sj, float thresholdValue);
    };
}
