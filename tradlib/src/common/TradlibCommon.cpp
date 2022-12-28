//
//  TradlibCommonx.cpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#include "TradlibCommon.hpp"

using namespace tradlib;


SharedFloatVec tradlib::makeSharedFloatVec(int size)
{
    assert(size >= 0);
    return makeSharedFloatVec((size_t) size);
}

SharedFloatVec tradlib::makeSharedFloatVec(size_t size)
{
    SharedFloatVec vector = makeSharedFloatVec(std::vector<float>(size));
    return vector;
}

SharedFloatVec tradlib::makeSharedFloatVec(int size, float value)
{
    assert(size >= 0);
    return makeSharedFloatVec((size_t) size, value);
}

SharedFloatVec tradlib::makeSharedFloatVec(size_t size, float value)
{
    SharedFloatVec vector = makeSharedFloatVec(std::vector<float>(size,value));
    return vector;
}

SharedFloatVec tradlib::makeSharedFloatVec(const std::vector<float> & floatVec)
{
    return std::make_shared<std::vector<float>>(floatVec);
}

SharedIntVec tradlib::makeSharedIntVec(int size)
{
    assert(size >= 0);
    return makeSharedIntVec((size_t) size);
}

SharedIntVec tradlib::makeSharedIntVec(size_t size)
{
    assert(size >= 0);
    SharedIntVec vector = makeSharedIntVec(std::vector<int>(size));
    return vector;
}

SharedIntVec tradlib::makeSharedIntVec(int size, float value)
{
    assert(size >= 0);
    return makeSharedIntVec((size_t) size, value);
}

SharedIntVec tradlib::makeSharedIntVec(size_t size, float value)
{
    SharedIntVec vector = makeSharedIntVec(std::vector<int>(size, value));
    return vector;
}

SharedIntVec tradlib::makeSharedIntVec(const std::vector<int> & intVec)
{
    return std::make_shared<std::vector<int>>(intVec);
}

SharedTranscribedNotesVec tradlib::makeSharedTranscribedNotesVec(size_t size)
{
    assert(size >= 0);
    SharedTranscribedNotesVec vector = std::make_shared<std::vector<TranscribedNote>>(size);
    return vector;
}
