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
        
        static void log(const std::string & msg)
        {
            if (TradlibProperties::getString("mode") == "client")
            {
                printf("%s", msg.c_str());
                
                // ToDo: Log to the GUI
                //MattGuiNB.log(msg);
            }
            else
                
            {
                printf("%s", msg.c_str());
            }
        }
    };
}
