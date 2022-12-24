//
//  FrequencyDomainCombFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
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
        SharedFloatVec m_FftMag;
        float m_Fundamental;
        int m_SampleRate;
        int m_FrameSize;
        float m_BinSize;
        
        FrequencyDomainCombFilter(float fundamental, int sampleRate);
        
        void calculateBinSize();
        
    public:
        
        FrequencyDomainCombFilter();
        
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
        
        float calculateOutputEnergy();
    };
}
