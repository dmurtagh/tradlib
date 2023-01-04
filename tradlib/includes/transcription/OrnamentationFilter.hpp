//
//  OrnamentationFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "FileLogger.hpp"
#include "TradlibCommon.hpp"

namespace tradlib
{
    class OrnamentationFilter
    {
    private:
        
        SharedTranscribedNotesVec m_TranscribedNotes;
        int m_SampleRate;
        SharedFloatVec m_Signal;
        
        FileLogger m_FileLogger;
        
    public:
        OrnamentationFilter(const SharedTranscribedNotesVec & transcribedNotes, int sampleRate, const SharedFloatVec & signal);
        
        SharedTranscribedNotesVec getTranscribedNotes();
        
        void setTranscribedNotes(SharedTranscribedNotesVec transcribedNotes);
        
        // Remove false positives
        SharedTranscribedNotesVec filter();
        
        static int calculateNearestMultiple(float duration, float standard);
        
        int getSampleRate();
        
        void setSampleRate(int sampleRate);
    };
}
