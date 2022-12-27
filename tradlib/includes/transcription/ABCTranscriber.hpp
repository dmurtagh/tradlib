//
//  ABCTranscriber.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <string>
#include "TradlibCommon.hpp"

namespace tradlib
{
    class ODCFTranscriber;
    
    class ABCTranscriber
    {
    public:
        static constexpr float RATIO = 1.05946309436f;
        
        static constexpr int REEL = 0;
        static constexpr int JIG = 1;
        static constexpr int HORNPIPE = 2;
        static constexpr int MIDI_NOTES = 87;
        static constexpr int MIDI_START = 21;
        
        const int NOTES_PER_BAR[3] = { 8, 6, 8 };
        const int NOTES_PER_PART[3] = { 64, 48, 64 };
        
        enum pitch_model {FLUTE, WHISTLE};
        
        static constexpr int NOTE_NAMES_LEN = 33;
        const std::string kNoteNames[NOTE_NAMES_LEN] = {"D,", "E,", "F,", "G,", "A,", "B,", "C", "C", "D", "E", "F", "G", "A", "B","c", "c", "d", "e", "f", "g", "a", "b", "c'", "c'", "d'", "e'", "f'", "g'", "a'", "b'", "c''", "c''", "d''"};
        static float m_KnownFrequencies[NOTE_NAMES_LEN];
        static float m_MidiNotes[87];
        
        static constexpr int MIDI_OFFSET = 21;
        vector<int> m_QuantizedMidi;
        
        // public static final float D3 = 146.83f;
        // public static final float D4=  293.66f; // Start transcription of the whistle one octive up
        
    private:
        static constexpr float RANGE = 0.1f;
        int m_TuneType = REEL;
        
        pitch_model m_PitchModel;
        float m_MaxEnergy;
        float m_AverageEnergy;
        
        SharedTranscribedNotesVec m_TranscribedNotes;
        ODCFTranscriber * m_Transcriber = nullptr;
        
    public:
        
        ABCTranscriber();
        
        ABCTranscriber(ODCFTranscriber * pTranscriber);
        
        std::string spell(float frequency);
        
        SharedTranscribedNotesVec getTranscribedNotes();
        
        void setTranscribedNotes(const SharedTranscribedNotesVec & transcribedNotes);
        
        pitch_model getPitchModel();
        
        void setPitchModel(pitch_model pitchModel);
        
        void testScale();
        
        bool isWholeToneInterval(int n, int intervals[]);
        
        void makeScale(const std::string & mode); // Todo: Make this an enum
        
    private:
        void makeMidiNotes();
        
    public:
        std::string convertToMidi();
        
        std::string convertToParsons();
        
        std::string convertToABC();
        
    private:
        bool isBreath(const TranscribedNote & note);
        
    public:
        int getTuneType();
        
        void setTuneType(int tuneType);
        
        void printScale();
        
        float calculateStandardNoteDuration();
        
        float getMaxEnergy() const;
        
        void setMaxEnergy(float maxEnergy);
        
        float getAverageEnergy() const;
        
        void setAverageEnergy(float averageEnergy);
        
        void calculatePitchModel();
        
        static void test();
    };
}
