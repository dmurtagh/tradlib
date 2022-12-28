//
//  Logger.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "Logger.hpp"

using namespace tradlib;

void Logger::log(const std::string & msg)
{
    if (TradlibProperties::getString("mode") == "client")
    {
        printf("%s\n", msg.c_str());
        
        // ToDo: Log to the GUI
        //MattGuiNB.log(msg);
    }
    else
        
    {
        printf("%s\n", msg.c_str());
    }
}
