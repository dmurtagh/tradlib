//
//  EnergyCalculator.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include "TradlibCommon.hpp"

namespace tradlib
{
    class EnergyCalculator
    {
    private:
        SharedFloatVec m_Signal;
        int m_Start;
        int m_End;
        
    public:
        EnergyCalculator();
        
        static constexpr float SILENCE_THRESHOLD = 600000000.0f;
        
        EnergyCalculator(const SharedFloatVec & signal, int start, int end);
        
        float calculateEnergy();
        
        float calculateAverageEnergy();
        
        float calculateMaxEnergy();
        
        static std::string formatEnergy(float energy);
        
        int getStart();
        
        void setStart(int start);
        
        int getEnd();
        
        void setEnd(int end);
        
        SharedFloatVec getSignal();
        
        void setSignal(const SharedFloatVec & signal);
    };
}
