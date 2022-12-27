//
//  TradlibCommonx.cpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#include "TradlibCommon.hpp"

using namespace tradlib;

SharedFloatVec makeSharedFloatVec(size_t size)
{
    SharedFloatVec vector;
    vector->resize(size);
    return vector;
}

SharedFloatVec makeSharedFloatVec(size_t size, float value)
{
    SharedFloatVec vector;
    vector->resize(size, value);
    return vector;
}

SharedFloatVec makeSharedFloatVec(const std::vector<float> & floatVec)
{
    return std::make_shared<std::vector<float>>(floatVec);
}

SharedIntVec makeSharedIntVec(size_t size)
{
    SharedIntVec vector;
    vector->resize(size);
    return vector;
}

SharedIntVec makeSharedIntVec(size_t size, float value)
{
    SharedIntVec vector;
    vector->resize(size, value);
    return vector;
}

SharedIntVec makeSharedIntVec(const std::vector<int> & intVec)
{
    return std::make_shared<std::vector<int>>(intVec);
}

SharedTranscribedNotesVec makeSharedTranscribedNotesVec(size_t size)
{
    SharedTranscribedNotesVec vector;
    vector->resize(size);
    return vector;
}
