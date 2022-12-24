//
//  LoadHelper.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <string>
#include <vector>

namespace tradlib
{
    struct LoadedAudio
    {
        std::vector<std::byte> audioData;
        std::vector<float> signal;
        int sampleRate;
        int numSamples;
        std::string fundamentalNote;
    };

    class AudioLoader
    {
        void loadAudio(const std::string & inputFile,
                       LoadedAudio & output);
    };
}
