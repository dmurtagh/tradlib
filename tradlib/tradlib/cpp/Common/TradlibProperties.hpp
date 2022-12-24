//
//  TradLibProperties.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <stdio.h>
#include <string>
#include <unordered_map>
#include "Logger.hpp"

using namespace std;

namespace tradlib
{
    class TradlibProperties
    {
    private:
        static unordered_map<string,string> m_Properties;
        
    public:
        
        static float getInt(const std::string & key);
        
        static float getFloat(const std::string & key);
        
        static bool getBool(const std::string & key);
        
        static std::string getString(const std::string & key);
        
        static void setString(const std::string & key, const std::string & value);
        
        void setInt(const std::string & key, int value);

        void setFloat(const std::string & key, float value);

        void setBool(const std::string & key, bool value);
    };
}
