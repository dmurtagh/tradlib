//
//  TestData.hpp
//  Trad App
//
//  Created by damien murtagh on 1/2/23.
//

#pragma once

#include "TradlibCommon.hpp"

namespace tradlib
{
    class TestData
    {
    public:
        static SharedFloatVec readTestSignal(const std::string & file);
    };
}
