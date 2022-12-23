//
//  SimpleHistogram.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <unordered_map>
#include <cmath>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class SimpleHistogram
    {
    private:
        SharedFloatVec data;
        
    public:
        
        float calculatePeek();
        
        SharedFloatVec getData();
        
        void setData(const SharedFloatVec & data);
    };
}
