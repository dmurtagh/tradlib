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
        static bool isEqual(const std::string & file, const SharedFloatVec & data);
        static bool isEqual(const std::string & file, const SharedIntVec & data);
        static bool isEqual(const std::string & file, const std::string & data);
        
        static SharedFloatVec readFloatVec(const std::string & file);
        
        static SharedIntVec readIntVec(const std::string & file);
    };
}
