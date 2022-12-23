//
//  FuzzyHistogram.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <stdio.h>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class FuzzyHistogram
    {
    public:
        static float calculatePeek(const SharedFloatVec & data, float fuzz);
    };
}
