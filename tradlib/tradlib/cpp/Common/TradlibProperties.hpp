//
//  TradLibProperties.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <stdio.h>
#include <string>

namespace tradlib
{
    class TradlibProperties
    {
        
    public:
        
        static float getInt(const std::string & key)
        {
            // needs implementation
            return 1;
        }
        
        static float getFloat(const std::string & key)
        {
            // needs implementation
            return 1.0f;
        }
        
        static bool getBool(const std::string & key)
        {
            // needs implementation
            return true;
        }
        
        static std::string getString(const std::string & key)
        {
            // needs implementation
            return "";
        }
        
        static void setString(const std::string & key, const std::string & value)
        {
            // needs implementation
        }
    };
}
