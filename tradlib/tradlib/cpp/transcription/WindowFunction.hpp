//
//  WindowFunction.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <cmath>
#include <string>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class WindowFunction
    {
    public:
        enum class Type
        {
            rectangular,
            bartlett,
            hanning,
            hamming,
            blackman,
            kaiser
        };
        
        Type windowType = Type::rectangular;  // defaults to rectangular window
        
        WindowFunction();
        
        void setWindowType(WindowFunction::Type type);
        
        void setWindowType(const std::string s);
        
        WindowFunction::Type getWindowType();
        
        SharedFloatVec generate(int nSamples);
    };
}
