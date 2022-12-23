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

namespace tradlib
{
    using SharedFloatVec = shared_ptr<vector<float>>;

    /**
        Makes a shared_ptr<vector<float>> with the specified size
     */
    SharedFloatVec makeSharedFloatVec(size_t size)
    {
        SharedFloatVec vector;
        vector->resize(size);
        return vector;
    }

    /**
        Makes a shared_ptr<vector<float>> with the specified size and initial value
     */
    SharedFloatVec makeSharedFloatVec(size_t size, float value)
    {
        SharedFloatVec vector;
        vector->resize(size, value);
        return vector;
    }

    using SharedIntVec = shared_ptr<vector<int>>;

    /**
        Makes a shared_ptr<vector<int>> with the specified size
     */
    SharedIntVec makeSharedIntVec(size_t size)
    {
        SharedIntVec vector;
        vector->resize(size);
        return vector;
    }

    /**
        Makes a shared_ptr<vector<int>> with the specified size and initial value
     */
    SharedIntVec makeSharedIntVec(size_t size, float value)
    {
        SharedIntVec vector;
        vector->resize(size, value);
        return vector;
    }

    using SharedTranscribedNotesVec = std::shared_ptr<std::vector<TranscribedNote>>;

    /**
        Makes a shared_ptr<vector<TranscribedNote>> with the specified size
     */
    SharedTranscribedNotesVec makeSharedTranscribedNotesVec(size_t size)
    {
        SharedTranscribedNotesVec vector;
        vector->resize(size);
        return vector;
    }

    using SharedByteVec = std::shared_ptr<std::vector<byte>>;
}
