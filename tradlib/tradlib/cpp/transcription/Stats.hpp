//
//  Stats.hpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#pragma once

#include "TradlibCommon.hpp"

namespace tradlib
{
    class Stats
    {
    private:
        SharedFloatVec m_Data;
        int m_Start;
        int m_End;
        
    public:
        Stats(const SharedFloatVec & data);

        SharedFloatVec getData();

        void setData(const SharedFloatVec & data);
        
        float average();
        
        float standardDeviation();
        
        int getStart();

        void setStart(int start);

        int getEnd();

        void setEnd(int end);
    };
}
