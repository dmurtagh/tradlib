//
//  TimeDomainCombFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#pragma once

#include "TradlibCommon.hpp"

namespace tradlib
{
    class TimeDomainCombFilter
    {
        
    private:
        
        SharedFloatVec m_Frame;
        int m_Frequency;
        int m_SampleRate;
        int m_Delay;
        
    public:
        
        /** Creates a new instance of CombFilter */
        TimeDomainCombFilter() {}
        
        SharedFloatVec getFrame();
        
        void setFrame(SharedFloatVec frame);
        
        int getFrequency();
        
        void setFrequency(int frequency);
        
        int getSampleRate();
        
        void setSampleRate(int sampleRate);
        
        int getDelay();
        
        float calculateOutputPower();
        
        float calculateHarmonicity();
        
    private:
        
        float calculateInputPower();
    };
}
