//
//  DSPCommon.hpp
//  tradlib
//
//  Created by damien murtagh on 12/20/22.
//

#pragma once

#include <vector>
#include <memory>
#include "TranscribedNote.hpp"

using namespace std;

/**
 Convenience typedefs (using statements) and functions for working with vectors of ints, floats and other
 common data types such as TranscribedNote
 */
namespace tradlib
{
    using SharedFloatVec = shared_ptr<vector<float>>;

    using SharedIntVec = shared_ptr<vector<int>>;

    using SharedTranscribedNotesVec = std::shared_ptr<std::vector<TranscribedNote>>;

    using SharedByteVec = std::shared_ptr<std::vector<byte>>;

    /**
        Makes a shared_ptr<vector<float>> with the specified size
     */
    SharedFloatVec makeSharedFloatVec(int size);
    SharedFloatVec makeSharedFloatVec(size_t size);

    /**
        Makes a shared_ptr<vector<float>> with the specified size and initial value
     */
    SharedFloatVec makeSharedFloatVec(int size, float value);
    SharedFloatVec makeSharedFloatVec(size_t size, float value);

    /**
        Makes a shared float vec from an existing vector<float>
     */
    SharedFloatVec makeSharedFloatVec(const std::vector<float> & floatVec);

    /**
        Makes a shared_ptr<vector<int>> with the specified size
     */
    SharedIntVec makeSharedIntVec(int size);
    SharedIntVec makeSharedIntVec(size_t size);

    /**
        Makes a shared_ptr<vector<int>> with the specified size and initial value
     */
    SharedIntVec makeSharedIntVec(int size, float value);
    SharedIntVec makeSharedIntVec(size_t size, float value);

    /**
        Makes a shared int vec from an existing vector<int>
     */
    SharedIntVec makeSharedIntVec(const std::vector<int> & intVec);

    /**
        Makes a shared_ptr<vector<TranscribedNote>> with the specified size
     */
    SharedTranscribedNotesVec makeSharedTranscribedNotesVec(size_t size);
}
