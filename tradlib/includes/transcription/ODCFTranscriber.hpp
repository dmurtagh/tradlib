//
//  ODCFTranscriber.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <stdio.h>
#include <string>
#include "TimeDomainCombFilter.hpp"
#include "FrequencyDomainCombFilter.hpp"

using namespace std;

namespace tradlib
{
    class ODCFTranscriber
    {
    protected:
        int m_FrameSize;
        int m_HopSize;
        int m_SampleRate;
        SharedFloatVec m_Signal;
        SharedTranscribedNotesVec m_TranscribedNotes;
        
        // ToDo: This should not have GUI code!!!
        //GUI m_GUI;
        
    private:
        int m_NumFilters = 12;
        int m_NumSamples = 0;
        
        vector<float> m_OldPowers = vector<float>(m_NumFilters);
        vector<float> m_Powers = vector<float>(m_NumFilters);
        std::string m_AbcTranscription;
        
        float m_SilenceThreshold = 1500.0f;
        std::string m_DefaultFundamental = "D";
        
        int m_DynamicThresholdTime = 100; // in milliseconds
        
        vector<TimeDomainCombFilter> m_TdFilters = vector<TimeDomainCombFilter>(m_NumFilters);
        vector<FrequencyDomainCombFilter> m_FdFilters = vector<FrequencyDomainCombFilter>(24);
        
        // ToDo: Remove GUI Elements from here...
        //private Graph frameGraph;
        //private Graph signalGraph;
        //private Graph odfGraph;
        
        // ToDo Remove this:
        //SourceDataLine line = null;
        float m_StaticThreshold = 0;
        
    public:
        ODCFTranscriber();
        
        void setAudioData(SharedFloatVec & signal, int sampleRate, const std::string & fundamentalNote);
        
        /* Removes a silent period from the start of the recording */
        void removeSilence();
        
        void transcribe(const std::string & fundamentalNote = "D");
        
        void printNotes();
        
        int getFrameSize();
        
        void setFrameSize(int frameSize);
        
        int getHopSize();
        
        void setHopSize(int hopSize);
        
        int getSampleRate();
        
        void setSampleRate(int sampleRate);
        
        int getNumSamples();
        
        void setNumSamples(int numSamples);
        
        void cleanup();
        
        int getDynamicThresholdTime();
        
        void setDynamicThresholdTime(int dynamicThresholdTime);
        
        SharedTranscribedNotesVec getTranscribedNotes();
        
        SharedFloatVec getSignal();
        
        void setSignal(const SharedFloatVec & signal);
        
        const std::string & getAbcTranscription();
        
        void setAbcTranscription(const std::string & abcTranscription);
        
        /**
         * @return the silenceThreshold
         */
        float getSilenceThreshold();
        
        /**
         * @param silenceThreshold the silenceThreshold to set
         */
        void setSilenceThreshold(float silenceThreshold);
        // End of public:
        
    protected:
        float sampleToSeconds(int sample);
        
    private:
        int odfIndexToSignal(int odfIndex);
        
        void removeSpuriousOnsets(SharedIntVec & onsetsVector, const SharedFloatVec & odfThreshold, const SharedFloatVec & odf) const;
        
        SharedTranscribedNotesVec calculateNotesUsingFFT(const SharedFloatVec & odfSignal, const SharedFloatVec & signal, int sampleRate);
        
        void configureFilters();
        
        SharedFloatVec calculateDynamicThreshold(const SharedFloatVec & odf);
    };
}
