//
//  PitchDetector.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <vector>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class PitchDetector
    {
    private:
        
        int maxFrameIndex(const SharedFloatVec & frame);
        
        //-----------calculate the maximum frequency in a frameed data-------------
        float maxFreq(int maxIndex, int sampleRate, int frameSize);
        
        float maxFilter(const SharedFloatVec & fftMag, int sampleRate, int frameSize, float startAt);
        
    public:
        
        float maxPeak(const SharedFloatVec & fftMag, int sampleRate, int frameSize);
        
        float maxHistogram(const SharedFloatVec & fftMag, int sampleRate, int frameSize);
        
        float maxBryanFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize);
        
        float cepstrumFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize);
        
        float mikelsFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize);
    };
}

