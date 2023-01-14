//
//  TradlibInterfaceDelegate.hpp
//  Trad App
//
//  Created by damien murtagh on 1/10/23.
//

#pragma once

#include <functional>
#include "TradlibDelegate.hpp"

namespace tradlib
{
    /**
    The purpose of this class is to communicate delegate updates from the c++ layer of tradlib to the iOS objective layer and ultimately to the Swift layer
     */
    class TradlibiOSDelegateShim : public TradlibDelegate
    {
    public:
        
        // Delegate setters which need to be called by the client code
        void SetCurrentActionUpdateDelegateFunction(std::function<void(const struct TradlibState state)> func);
        void SetCurrentActionCompleteDelegateFunction(std::function<void(const struct TradlibState state)> func);
        
        
        // Delegate Functions that the tradlib c++ code will call
        
        virtual void onCurrentActionUpdate(const struct TradlibState state);
        
        virtual void onCurrentActionComplete(const struct TradlibState state);
        
    private:
        
        std::function<void(const struct TradlibState state)> _onCurrentActionUpdateDelegateFunction = nullptr;
        std::function<void(const struct TradlibState state)> _onCurrentActionCompleteDelegateFunction = nullptr;
    };
}
