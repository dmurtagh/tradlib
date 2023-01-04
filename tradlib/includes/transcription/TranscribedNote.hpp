//
//  TranscribedNote.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <string>

namespace tradlib
{
    class TranscribedNote
    {
    private:
        int m_MidiNote              = 0;
        float m_Frequency           = 0.f;
        float m_Start               = 0.f;
        float m_Duration            = 0.f;
        float m_UnmergedDuration    = 0.f;
        float m_UnmergedStart       = 0.f;
        float m_Energy              = 0.f;
        std::string m_Name          = "NA";
        int m_QuaverQ               = 0;
        int m_Multiple              = 0;
        
    public:
        TranscribedNote();
        
        TranscribedNote(float frequency, float start, float duration);
        
        float getFrequency() const;
        
        void setFrequency(float frequency);
        
        float getStart() const;
        
        void setStart(float start);
        
        float getDuration() const;
        
        void setDuration(float duration);
        
        std::string getSpelling() const;
        
        void setSpelling(std::string name);
        
        std::string toString();
        
        static std::string headersString();
        
        float getEnergy() const;
        
        void setEnergy(float energy);
        
        int getQuaverQ() const;
        
        void setQuaverQ(int quaverQ);
        
        int getMultiple() const;
        
        void setMultiple(int multiple);
        
        float getUnmergedDuration() const;
        
        void setUnmergedDuration(float unmergedDuration);
        
        float getUnmergedStart() const;
        
        void setUnmergedStart(float unmergedStart);
        
        int getMidiNote() const;
        
        void setMidiNote(int midiNote);
    };
}
