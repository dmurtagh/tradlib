//
//  TradlibInterfaceDelegate.cpp
//  Trad App
//
//  Created by damien murtagh on 1/10/23.
//

#include "TradlibiOSDelegateShim.hpp"
#include "Logger.hpp"

using namespace tradlib;


void TradlibiOSDelegateShim::SetCurrentActionUpdateDelegateFunction(std::function<void(const struct TradlibState state)> func)
{
    _onCurrentActionUpdateDelegateFunction = func;
}

void TradlibiOSDelegateShim::SetCurrentActionCompleteDelegateFunction(std::function<void(const struct TradlibState state)> func)
{
    _onCurrentActionCompleteDelegateFunction = func;
}

void TradlibiOSDelegateShim::onCurrentActionUpdate(const struct TradlibState state)
{
    Logger::log("TradlibiOSDelegateShim::onCurrentActionUpdate()");
    _onCurrentActionUpdateDelegateFunction(state);
}

void TradlibiOSDelegateShim::onCurrentActionComplete(const struct TradlibState state)
{
    Logger::log("TradlibiOSDelegateShim::onCurrentActionComplete()");
    _onCurrentActionCompleteDelegateFunction(state);
}

