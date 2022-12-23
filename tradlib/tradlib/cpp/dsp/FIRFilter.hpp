//
//  FIRFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <vector>
#include <memory>
#include "TradlibCommon.hpp"

using namespace std;

namespace tradlib
{
    /**
     * FIR = Finite Impulse Response
     * Currently used by Set Finder
     */
    class FIRFilter
    {
    public:
        static const int m_LP = 1;
        static const int m_BP = 2;
        static const int m_HP = 3;
        
    private:
        
        static int m_Taps;
        
        vector<float> m_A = vector<float>(m_Taps);
        int m_FilterType;
        
    public:
        
        void setFilterType(int ft);
        
        // ToDo: Maybe find a better solution here. Should I modify ip in place? Or return a move syntax object?
        SharedFloatVec filter(const vector<float> & ip);
    };
}
