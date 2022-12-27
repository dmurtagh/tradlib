//
//  Logger.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include "TradlibProperties.hpp"

namespace tradlib
{
    class Logger
    {
    public:
        
        static void log(const std::string & msg);
    };
}
