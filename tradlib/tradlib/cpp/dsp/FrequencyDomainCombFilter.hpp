//
//  FrequencyDomainCombFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#pragma once

#include <stdio.h>
#include <vector>
#include "TradlibCommon.hpp"

using namespace std;

namespace tradlib
{
    class FrequencyDomainCombFilter
    {
    private:
        SharedFloatVec fftMag;
        float fundamental;
        int sampleRate;
        int frameSize;
        float binSize;
        
        FrequencyDomainCombFilter();
        
        FrequencyDomainCombFilter(float fundamental, int sampleRate);
        
        void calculateBinSize();
        
    public:
        
        const SharedFloatVec getFftMag();
        
        void setFftMag(const SharedFloatVec & fftMag);
        
        float getFundamental();
        
        void setFundamental(float fundamental);
        
        int getSampleRate();
        
        void setSampleRate(int sampleRate);
        
        float getBinSize();
        
        int getFrameSize();
        
        void setFrameSize(int frameSize);
        
    private:
        
        bool harmonicInRange(float lower, float upper);
        
        float calculateOutputEnergy();    };
}
