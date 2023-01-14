//
//  TradlibDelegate.hpp
//  TradApp
//
//  Created by damien murtagh on 1/10/23.
//

#pragma once

#include "TradlibState.h"

namespace tradlib
{
    // Used to update the calling classes on the state of the current tradlib actions in progress
    class TradlibDelegate
    {
    public:
        
        virtual void onCurrentActionUpdate(const struct TradlibState state) = 0;
        
        virtual void onCurrentActionComplete(const struct TradlibState state) = 0;
    };
}
